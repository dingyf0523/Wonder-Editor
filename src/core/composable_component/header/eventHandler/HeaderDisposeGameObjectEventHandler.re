open UpdateStore;

module CustomEventHandler = {
  include EmptyEventHandler.EmptyEventHandler;
  type prepareTuple = unit;
  type dataTuple = unit;

  let _checkSceneGraphDataAndDispatch = (dispatchFunc, newSceneGraphArr) => {
    /* WonderLog.Contract.requireCheck(
         () =>
           WonderLog.(
             Contract.(
               test(
                 Log.buildAssertMessage(
                   ~expect=
                     {j|the newSceneGraphArr should equal the sceneGraph from engine|j},
                   ~actual={j|not|j},
                 ),
                 () =>
                 SceneGraphUtils.getSceneGraphDataFromEngine
                 |> StateLogicService.getStateToGetData == newSceneGraphArr
                 |> assertTrue
               )
             )
           ),
         StateEditorService.getStateIsDebug(),
       ); */

    dispatchFunc(
      AppStore.SceneTreeAction(SetSceneGraph(Some(newSceneGraphArr))),
    )
    |> ignore;

    dispatchFunc(AppStore.UpdateAction(Update([|Inspector, SceneTree|])))
    |> ignore;
  };

  let _getRemovedSceneGraphData = sceneGraphArr => {
    let editorState = StateEditorService.getState();

    switch (SceneEditorService.getCurrentSceneTreeNode(editorState)) {
    | None =>
      WonderLog.Log.error(
        WonderLog.Log.buildErrorMessage(
          ~title="_getRemovedSceneGraphData",
          ~description=
            {j|current gameObject should exist, but actual is None|j},
          ~reason="",
          ~solution={j|set current gameObject|j},
          ~params={j||j},
        ),
      );
      (sceneGraphArr, None);

    | Some(gameObject) =>
      let engineState = StateEngineService.unsafeGetState();

      engineState |> CameraEngineService.hasCameraGroup(gameObject) ?
        {
          SceneUtils.doesSceneHasRemoveableCamera() ?
            {
              let (editorState, engineState) =
                engineState
                |> CameraLogicService.handleForRemoveCameraGroup(
                     gameObject,
                     editorState,
                   );

              editorState |> StateEditorService.setState |> ignore;

              engineState |> StateEngineService.setState |> ignore;
            } :
            ();

          let (newSceneGraphArr, removedTreeNode) =
            sceneGraphArr |> SceneGraphUtils.removeDragedTreeNode(gameObject);

          (newSceneGraphArr, removedTreeNode |. Some);
        } :
        {
          let (newSceneGraphArr, removedTreeNode) =
            sceneGraphArr |> SceneGraphUtils.removeDragedTreeNode(gameObject);

          (newSceneGraphArr, removedTreeNode |. Some);
        };
    };
  };

  let _hasLightComponent = (removedTreeNode, engineState) =>
    SceneGraphUtils.getAllGameObjects(removedTreeNode)
    |> SceneEngineService.doesNeedReInitSceneAllLightMaterials(_, engineState);

  let handleSelfLogic = ((store, dispatchFunc), (), ()) => {
    let sceneGraphArr = store |> StoreUtils.unsafeGetSceneGraphDataFromStore;

    let (newSceneGraphArr, removedTreeNode) =
      _getRemovedSceneGraphData(sceneGraphArr);

    switch (removedTreeNode) {
    | None => ()
    | Some(removedTreeNode) =>
      let engineState = StateEngineService.unsafeGetState();

      let hasLightComponent =
        _hasLightComponent(removedTreeNode, engineState);

      engineState |> StateEngineService.setState |> ignore;

      removedTreeNode
      |> CurrentSceneTreeNodeLogicService.disposeCurrentSceneTreeNode;

      StateLogicService.getAndRefreshEngineState();

      hasLightComponent ?
        SceneEngineService.clearShaderCacheAndReInitSceneAllLightMaterials
        |> StateLogicService.getAndSetEngineState :
        ();

      StateLogicService.getAndRefreshEngineState();
    };

    _checkSceneGraphDataAndDispatch(dispatchFunc, newSceneGraphArr);
  };
};

module MakeEventHandler = EventHandler.MakeEventHandler(CustomEventHandler);
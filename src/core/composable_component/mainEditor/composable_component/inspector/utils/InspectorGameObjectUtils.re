open InspectorComponentType;

let buildComponentBox =
    (
      (store, dispatchFunc),
      (name, type_, gameObject),
      isDisposable,
      buildComponentFunc,
    ) =>
  <ComponentBox
    key=(DomHelper.getRandomKey())
    reduxTuple=(store, dispatchFunc)
    header=name
    isDisposable
    type_
    gameObject
    gameObjectUIComponent=(
      buildComponentFunc((store, dispatchFunc), gameObject)
    )
  />;
let _buildTransformFunc = ((store, dispatchFunc), gameObject) =>
  <MainEditorTransform
    key=(DomHelper.getRandomKey())
    store
    dispatchFunc
    transformComponent=(
      GameObjectComponentEngineService.getTransformComponent(gameObject)
      |> StateLogicService.getEngineStateToGetData
    )
  />;

let _buildLightFunc = ((store, dispatchFunc), gameObject) =>
  <MainEditorLight key=(DomHelper.getRandomKey()) store dispatchFunc />;

let _buildSouceInstanceFunc = ((store, dispatchFunc), gameObject) =>
  <div key=(DomHelper.getRandomKey())>
    (DomHelper.textEl("simulate source instance"))
  </div>;

let _buildRenderGroupFunc = ((store, dispatchFunc), gameObject) =>
  <MainEditorRenderGroup
    store 
    dispatchFunc
  />;

let _buildGeometryFunc = ((store, dispatchFunc), gameObject) =>
  <div key=(DomHelper.getRandomKey())>
    (DomHelper.textEl("simulate Geometry"))
  </div>;

let _buildCameraGroupFunc = ((store, dispatchFunc), gameObject) =>
  <MainEditorCameraGroup
    store 
    dispatchFunc
  />
let _buildArcballCamera = ((store, dispatchFunc), gameObject) =>
  <MainEditorArcballCamera
    store
    dispatchFunc
    arcballCameraController=(
      GameObjectComponentEngineService.getArcballCameraControllerComponent(
        gameObject,
      )
      |> StateLogicService.getEngineStateToGetData
    )
  />;
let buildComponentUIComponent = ((store, dispatchFunc), type_, gameObject) =>
  switch (type_) {
  | Transform =>
    _buildTransformFunc
    |> buildComponentBox(
         (store, dispatchFunc),
         ("Transform", type_, gameObject),
         false,
       )

  | Light =>
    _buildLightFunc
    |> buildComponentBox(
         (store, dispatchFunc),
         ("Light", type_, gameObject),
         true,
       )

  | RenderGroup =>
    _buildRenderGroupFunc
    |> buildComponentBox(
         (store, dispatchFunc),
         ("RenderGroup", type_, gameObject),
         true,
       )

  | Geometry =>
    _buildGeometryFunc
    |> buildComponentBox(
         (store, dispatchFunc),
         ("Geometry", type_, gameObject),
         false,
       )

  | SourceInstance => ReasonReact.nullElement
  /* _buildSouceInstanceFunc
     |> buildComponentBox((type_, component), (store, dispatchFunc), true) */

  | CameraGroup =>
    _buildCameraGroupFunc
    |> buildComponentBox(
         (store, dispatchFunc),
         ("Camera Group", type_, gameObject),
         true,
       )

  | ArcballCameraController =>
    _buildArcballCamera
    |> buildComponentBox(
         (store, dispatchFunc),
         ("ArcballCameraController", type_, gameObject),
         true,
       )
  };
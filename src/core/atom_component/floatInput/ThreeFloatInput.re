type state = {
  x: float,
  y: float,
  z: float,
};

type action =
  | BlurX(float)
  | BlurY(float)
  | BlurZ(float);

let component = ReasonReact.reducerComponent("ThreeFloatInput");

let reducer =
    (
      (uiState, dispatchFunc),
      (gameObjectComponent, blurEventFunc),
      action,
      state,
    ) =>
  switch (action) {
  | BlurX(xValue) =>
    blurEventFunc(
      (uiState, dispatchFunc),
      gameObjectComponent,
      (state.x, state.y, state.z),
    );

    ReasonReact.Update({...state, x: xValue});

  | BlurY(yValue) =>
    blurEventFunc(
      (uiState, dispatchFunc),
      gameObjectComponent,
      (state.x, state.y, state.z),
    );

    ReasonReact.Update({...state, y: yValue});

  | BlurZ(zValue) =>
    blurEventFunc(
      (uiState, dispatchFunc),
      gameObjectComponent,
      (state.x, state.y, state.z),
    );

    ReasonReact.Update({...state, z: zValue});
  };

let render =
    (
      (uiState, dispatchFunc),
      (gameObjectComponent, label, canBeZero),
      (dragDropFunc, changeXFunc, changeYFunc, changeZFunc),
      {state, send}: ReasonReact.self('a, 'b, 'c),
    ) =>
  <article className="inspector-item wonder-three-float-input">
    <div className="item-header"> (DomHelper.textEl(label)) </div>
    <div className="item-content">
      <FloatInput
        label="X"
        defaultValue=(state.x |> StringService.floatToString)
        onChange=(changeXFunc(gameObjectComponent))
        onBlur=(value => send(BlurX(value)))
        onDragDrop=dragDropFunc
        canBeZero
      />
      <FloatInput
        label="Y"
        defaultValue=(state.y |> StringService.floatToString)
        onChange=(changeYFunc(gameObjectComponent))
        onBlur=(value => send(BlurY(value)))
        onDragDrop=dragDropFunc
        canBeZero
      />
      <FloatInput
        label="Z"
        defaultValue=(state.z |> StringService.floatToString)
        onChange=(changeZFunc(gameObjectComponent))
        onBlur=(value => send(BlurZ(value)))
        onDragDrop=dragDropFunc
        canBeZero
      />
    </div>
  </article>;

let make =
    (
      ~uiState,
      ~dispatchFunc,
      ~label,
      ~gameObjectComponent,
      ~changeXFunc,
      ~changeYFunc,
      ~changeZFunc,
      ~getDataFunc,
      ~blurEventFunc,
      ~dragDropFunc=e => (),
      ~canBeZero,
      _children,
    ) => {
  ...component,
  initialState: () => {
    let (x, y, z) =
      getDataFunc(gameObjectComponent)
      |> StateLogicService.getEngineStateToGetData;

    {x, y, z};
  },
  reducer:
    reducer((uiState, dispatchFunc), (gameObjectComponent, blurEventFunc)),
  render: self =>
    render(
      (uiState, dispatchFunc),
      (gameObjectComponent, label, canBeZero),
      (dragDropFunc, changeXFunc, changeYFunc, changeZFunc),
      self,
    ),
};
open ConsoleMessageType;

type retainedProps = {updateTypeArr: UpdateStore.updateComponentTypeArr};

module Method = {
  let setCheckedMessageCount = editorState =>
    editorState
    |> MessageArrayConsoleEditorService.getConsoleMessageArrayLen
    |. CheckedCountConsoleEditorService.setConsoleCheckedCount(editorState);

  let clearAllConsoleMessage = dispatchFunc => {
    StateEditorService.getState()
    |> MessageArrayConsoleEditorService.clearConsoleMessageArray
    |> CheckedCountConsoleEditorService.clearConsoleCheckedCount
    |> StateEditorService.setState;

    dispatchFunc(
      AppStore.UpdateAction(
        Update([|UpdateStore.Console, UpdateStore.BottomHeader|]),
      ),
    )
    |> ignore;
  };

  let showConsoleMsgComponent = consoleMessageArr =>
    consoleMessageArr
    |> Js.Array.map(({message, consoleType, traceInfo}) =>
         switch (consoleType) {
         | Error =>
           <ConsoleBaseComponent
             key=(DomHelper.getRandomKey())
             type_="error"
             imageSrc="./public/img/error.png"
             message
             traceInfo
           />
         | Info =>
           <ConsoleBaseComponent
             key=(DomHelper.getRandomKey())
             type_="info"
             imageSrc="./public/img/info.png"
             message
             traceInfo
           />
         | Warn =>
           <ConsoleBaseComponent
             key=(DomHelper.getRandomKey())
             type_="warn"
             imageSrc="./public/img/warn.png"
             message
             traceInfo
           />
         | Debug =>
           <ConsoleBaseComponent
             key=(DomHelper.getRandomKey())
             type_="debug"
             imageSrc="./public/img/debug.png"
             message
             traceInfo
           />
         | Log =>
           <ConsoleBaseComponent
             key=(DomHelper.getRandomKey())
             type_="log"
             imageSrc="./public/img/log.png"
             message
             traceInfo
           />
         | _ => ReasonReact.null
         }
       );
};

let component =
  ReasonReact.statelessComponentWithRetainedProps("MainEditorConsole");

let log11 = () => WonderLog.Log.print("log message") |> ignore;

let info = () => WonderLog.Log.info({j|info message|j});

let warn11 = () => WonderLog.Log.warn({j|warn message|j});

let debug = () =>
  WonderLog.Log.debug(
    LogUtils.buildDebugMessage(
      ~description={j|create test debug|j},
      ~params={j||j},
    ),
    StateEditorService.getStateIsDebug(),
  );

let fatal = () =>
  Console.tryCatch(
    () =>
      WonderLog.Log.fatal(
        LogUtils.buildFatalMessage(
          ~description={j|sss|j},
          ~reason="",
          ~solution={j||j},
          ~params={j||j},
        ),
      ),
    e => Console.throwFatal(e),
  );

let _renderHeader = dispatchFunc =>
  <div className="console-header">
    <button
      className=""
      onClick=(_e => Method.clearAllConsoleMessage(dispatchFunc))>
      (DomHelper.textEl("clear"))
    </button>
    <button className="" onClick=(_e => log11())>
      (DomHelper.textEl("add log"))
    </button>
    <button className="" onClick=(_e => info())>
      (DomHelper.textEl("add info"))
    </button>
    <button className="" onClick=(_e => warn11())>
      (DomHelper.textEl("add warn"))
    </button>
    <button className="" onClick=(_e => debug())>
      (DomHelper.textEl("add debug"))
    </button>
    <button className="" onClick=(_e => fatal())>
      (DomHelper.textEl("add fatal"))
    </button>
  </div>;

let render = (store, dispatchFunc, _self) => {
  let consoleMessageArray =
    StateEditorService.getState()
    |> MessageArrayConsoleEditorService.getConsoleMessageArray;

  store
  |> StoreUtils.getBottomCurrentComponentType
  |> MainEditorBottomComponentUtils.isTypeEqualConsole ?
    Method.setCheckedMessageCount |> StateLogicService.getAndSetEditorState :
    ();

  <article key="console" className="wonder-bottom-console">
    <article className="wonder-console-component">
      (
        StateEditorService.getStateIsDebug() ?
          _renderHeader(dispatchFunc) : ReasonReact.null
      )
      <div className="console-content">
        (
          consoleMessageArray |> ArrayService.hasItem ?
            ReasonReact.array(
              Method.showConsoleMsgComponent(consoleMessageArray),
            ) :
            ReasonReact.null
        )
      </div>
    </article>
  </article>;
};

let shouldUpdate =
    ({newSelf}: ReasonReact.oldNewSelf('a, retainedProps, 'c)) =>
  newSelf.retainedProps.updateTypeArr
  |> StoreUtils.shouldComponentUpdate(UpdateStore.Console);

let make = (~store, ~dispatchFunc, _children) => {
  ...component,
  retainedProps: {
    updateTypeArr: StoreUtils.getUpdateComponentTypeArr(store),
  },
  shouldUpdate,
  render: render(store, dispatchFunc),
};
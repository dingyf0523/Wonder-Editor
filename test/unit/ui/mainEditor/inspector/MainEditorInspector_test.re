open Wonder_jest;

open Expect;

open Expect.Operators;

open Sinon;

external toObject : ReactTestRenderer.t => Js.t({..}) = "%identity";

let _ =
  describe(
    "MainEditorInspector ui component",
    () => {
      let sandbox = getSandboxDefaultVal();
      beforeEach(
        () => {
          sandbox := createSandbox();
          TestToolEngine.prepare(sandbox)
        }
      );
      afterEach(() => restoreSandbox(refJsObjToSandbox(sandbox^)));
      describe(
        "test snapshot",
        () => {
          test(
            "if hasn't currentGameObject, show nothing",
            () => {
              TestToolUI.initMainEditor(sandbox);
              let component =
                ReactTestRenderer.create(
                  <MainEditorInspector
                    store=(TestToolUI.buildEmptyAppState())
                    dispatch=(TestToolUI.getDispatch())
                  />
                );
              let json = ReactTestRenderer.toJSON(component);
              toMatchSnapshot(expect(json))
            }
          );
          test(
            "else, should show transform component of it",
            () => {
              TestToolUI.initMainEditor(sandbox);
              MainEditorSceneToolEditor.recombineSceneChildrenAndSetCurrentGameObject();
              let component =
                ReactTestRenderer.create(
                  <MainEditorInspector
                    store=(TestToolUI.buildEmptyAppState())
                    dispatch=(TestToolUI.getDispatch())
                  />
                );
              let json = ReactTestRenderer.toJSON(component);
              toMatchSnapshot(expect(json))
            }
          )
        }
      )
    }
  );
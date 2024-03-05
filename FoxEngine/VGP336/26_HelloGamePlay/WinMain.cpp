#include<FoxEngine/Inc/FoxEngine.h>
#include "GameState.h"
#include "EditorState.h"
#include "EditTemplateState.h"

using namespace FoxEngine;
using namespace FoxEngine::Core;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appName = L"Hello GamePlay";

	App& myApp = MainApp();
	myApp.AddState<GameState>("GameState");
	myApp.AddState<EditorState>("EditorState");
	myApp.AddState<EditTemplateState>("EditTemplateState");
	myApp.Run(config);

	return 0;
}
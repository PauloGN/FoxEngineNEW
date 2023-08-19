#include<FoxEngine/Inc/FoxEngine.h>
#include"GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Core;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appName = L"Try light";

	App& myApp = MainApp();
	//States
	myApp.AddState<GameState>("GameState");
	myApp.Run(config);

	return 0;
}
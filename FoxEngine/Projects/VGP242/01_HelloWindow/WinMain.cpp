#include "FoxEngine/Inc/FoxEngine.h"
#include "GameState.h"


int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{

	AppConfig app_config;
	app_config.appName = L"Hello Window";

	App myApp;
	myApp.AddState<GameState>("GameState");
	myApp.Run(app_config);

	return 0;
}
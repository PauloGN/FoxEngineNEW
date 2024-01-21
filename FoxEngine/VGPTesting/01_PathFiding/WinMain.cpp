#include<FoxEngine/Inc/FoxEngine.h>
#include"GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Core;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{

	FoxEngine::AppConfig appConfig;
	appConfig.appName = L"Hello Fox";
	appConfig.winWidth = 1280;
	appConfig.winHeight = 720;
	appConfig.debugDrawLimit = 1000000;
	appConfig.textureRoot = "../../Assets/Images";

	auto& app = MainApp();
	app.AddState<GameState>("GameState");
	app.Run(appConfig);

	return 0;
}
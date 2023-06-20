#include<FoxEngine/Inc/FoxEngine.h>
#include"GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Core;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{

	AppConfig config;
	config.winWidth;
	config.winHeight;
	config.appName = L"Hello Window";
	
	App myApp;

	//create and add a State
	myApp.AddState<GameState>("GameState");

	myApp.Run(config);

	LOG("....Window closed...");
	return 0;
}
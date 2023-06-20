#include<FoxEngine/Inc/FoxEngine.h>
#include"GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Core;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{

	AppConfig config;
	config.appName = L"Hello Cube";
	
	App& myApp = MainApp();
	//States
	myApp.AddState<ColorCube>("ColorCube");
	myApp.AddState<GrayCube>("GrayCube");

	myApp.Run(config);

	return 0;
}
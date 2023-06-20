#include<FoxEngine/Inc/FoxEngine.h>
#include"GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Core;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{

	AppConfig config;
	config.winWidth;
	config.winHeight;
	config.appName = L"Hello Texture";
	
	App& myApp = MainApp();
	//States
	myApp.AddState<BasicCubeTextureDebug>("BasicCubeTexture");
	myApp.AddState<BasicSphereTextureDebug>("SkyBox");
	myApp.Run(config);

	return 0;
}
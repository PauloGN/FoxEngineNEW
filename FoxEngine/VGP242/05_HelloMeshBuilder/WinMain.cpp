#include<FoxEngine/Inc/FoxEngine.h>
#include"GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Core;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{

	AppConfig config;
	config.winWidth;
	config.winHeight;
	config.appName = L"Hello MeshBuilder";
	
	App& myApp = MainApp();
	//States
	myApp.AddState<BasicCubeTextureDebug>("BasicCube");
	myApp.AddState<BasicSphereTextureDebug>("BasicRec");
	myApp.AddState<Week4Example>("Week4Example");
	myApp.AddState<BasicCil>("BasicCil");
	myApp.AddState<BasicSphere>("BasicSphere");
	//myApp.ChangeState("OtherState");
	myApp.Run(config);

	return 0;
}
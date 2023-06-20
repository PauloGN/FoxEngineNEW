#include "FoxEngine/Inc/FoxEngine.h"
#include "GameState.h"

//For a shader to work we need two things minimum:

//1  - A vertex shader, this will describe how the vertices are going to be read and pass into the GPU
//2 - A Pixel shader that will basically say "These are the functions that we are going to use"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{

	AppConfig app_config;
	app_config.appName = L"Hello Shapes";

	App& myApp = MainApp();
	myApp.AddState<GameState>("GameState");
	myApp.AddState<Triangle>("Triangle");
	myApp.AddState<Square>("Square");
	myApp.Run(app_config);

	return 0;
}
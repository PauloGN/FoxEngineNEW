#include <FoxEngine/Inc/FoxEngine.h>
#include "GameState.h"

using namespace FoxEngine;
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    AppConfig config;
    config.appName = L"Hello Render obj";

    App& myApp = MainApp();
    myApp.AddState<GameState>("GameState");
    myApp.AddState<SecondGameState>("SecondGameState");
    myApp.Run(config);

    return(0);
}
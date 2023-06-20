#include <FoxEngine/Inc/FoxEngine.h>
#include "GameState.h"

using namespace FoxEngine;
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    AppConfig config;
    config.appName = L"Hello OBJ";

    App& myApp = MainApp();
    myApp.AddState<GameState>("GameState");
    myApp.Run(config);

    return(0);
}
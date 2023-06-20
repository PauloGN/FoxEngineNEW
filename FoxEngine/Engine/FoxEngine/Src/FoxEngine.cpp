#include "Precompiled.h"
#include "FoxEngine.h"

FoxEngine::App& FoxEngine::MainApp()
{
    static App sApp;
    return sApp;
}

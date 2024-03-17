#pragma once
#include "GameWorld.h"
#include <iostream>
#include "MagnetifyComponent.h"

namespace X
{
    
#define LOAD_MAGNETIFY(gm, source, fileName) \
    std::ifstream ifs; \
    ifs.open(fileName); \
    FoxEngine::MagnetifyComponent* MCo = (gm).GetGameObject(source)->GetComponent<FoxEngine::MagnetifyComponent>(); \
    if (ifs.is_open()) \
    { \
        std::string objName; \
        bool hasAttraction =false;\
        while (ifs >> objName && ifs >> hasAttraction) \
        { \
            FoxEngine::GameObject* obj = (gm).GetGameObject(objName); \
            obj->mHasAttraction = hasAttraction;\
            MCo->AddObject(*obj); \
        } \
    } \
    ifs.close();
}
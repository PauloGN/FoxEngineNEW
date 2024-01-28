#pragma once

#include "Common.h"
#include "App.h"
#include "AppState.h"
#include "GameObject.h"
#include "GameObjectHandle.h"
#include "GameObjectFactory.h"
#include "GameWorld.h"
#include "TypeIds.h"

//Components
#include "Component.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
//Services
#include "Service.h"
#include "CameraService.h"

namespace FoxEngine
{
	App& MainApp();
}
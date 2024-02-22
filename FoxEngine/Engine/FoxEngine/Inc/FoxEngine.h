#pragma once

#include "Common.h"
//App headers
#include "App.h"
#include "AppState.h"

//World headers
#include "GameObject.h"
#include "GameObjectHandle.h"
#include "GameObjectFactory.h"
#include "GameWorld.h"
#include "TypeIds.h"

//Components
#include "Component.h"
#include "AnimatorComponent.h"
#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "FPSCameraComponent.h"
#include "MeshComponent.h"
#include "ModelComponent.h"
#include "RigidBodyComponent.h"
#include "SoundBankComponent.h"
#include "SoundEffectComponent.h"
#include "TransformComponent.h"
//Services
#include "Service.h"
#include "CameraService.h"
#include "PhysicsService.h"
#include "UpdateService.h"
#include "RenderService.h"

namespace FoxEngine
{
	App& MainApp();
}
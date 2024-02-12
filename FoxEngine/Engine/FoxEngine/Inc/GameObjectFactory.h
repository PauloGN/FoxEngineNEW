#pragma once
#include "GameObject.h"

namespace FoxEngine
{
	class GameObject;

	using CustomMake = std::function<bool(const char*, const rapidjson::Value&, GameObject&)>;

	namespace GameObjectFactory
	{
		void SetCustomMake(CustomMake customMake);
		void Make(const std::filesystem::path& templatePath, GameObject& gameObject);
	}
}

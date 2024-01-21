#pragma once

namespace FoxEngine
{
	class GameObject;

	namespace GameObjectFactory
	{
		void Make(const std::filesystem::path& templatePath, GameObject& gameObject);
	}
}
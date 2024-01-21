#pragma once
#include "Component.h"

namespace FoxEngine
{
	class GameObject final
	{
	public:

		GameObject() = default;

		void Initialize();
		void Terminate();
		void DebugUI();

		void SetName(std::string& name) { mName = std::move(name); }
		const std::string& GetName() const { return mName; }
		uint32_t GetUniqueId() const { return mUniqueId; }

		template<class ComponentType>
		ComponentType* AddComponent()
		{
			static_assert(std::is_base_of_v<Component, ComponentType>, "Game Object: component type must be of type component");
			ASSERT(!mInitialized, "Game Object: cannot add components once initialized");
			ASSERT(!HasA<ComponentType>(), "Game Object: already has component type");

			auto& newComponent = mComponents.emplace_back(std::make_unique<ComponentType>());
			//std::unique_ptr<ComponentType>& newComponent = mComponents.emplace_back(std::make_unique<ComponentType>());
			newComponent->mOwner = this;
			return static_cast<ComponentType*>(newComponent.get());
		}

		template<class ComponentType>
		bool HasA()
		{
			static_assert(std::is_base_of_v<Component, ComponentType>, "Game Object: component type must be of type component");

			for (auto& Component : mComponents)
			{
				if (Component->GetTypeId() == ComponentType::StaticGetTypeId())
				{
					return true;
				}
			}
			return false;
		}

	private:

		std::string mName = "EMPTY";
		bool mInitialized = false;
		uint32_t mUniqueId = 0;

		using Components = std::vector<std::unique_ptr<Component>>;
		Components mComponents;
	};
}
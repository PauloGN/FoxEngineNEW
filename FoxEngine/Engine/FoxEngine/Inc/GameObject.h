#pragma once
#include "Component.h"
#include "GameObjectHandle.h"

namespace FoxEngine
{
	class GameWorld;

	class GameObject final
	{
	public:

		GameObject() = default;

		void Initialize();
		void Terminate();
		void DebugUI();
		void EditorUI();

		void SetName(std::string& name) { mName = std::move(name); }
		const std::string& GetName() const { return mName; }
		uint32_t GetUniqueId() const { return mUniqueId; }

		void SetTemplate(const std::filesystem::path& tempatePath) { mTemplatePath = tempatePath; }
		const std::filesystem::path& GetTemplatePath() const { return mTemplatePath; }

		GameWorld& GetWorld() { return*mWorld; }
		const GameWorld& GetWorld() const { return* mWorld; }
		const GameObjectHandle& GetHandle() const { return mHandle;}

#pragma region Templatized
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

		//GET COMPONENT
		template<class ComponentType>
		ComponentType* GetComponent()
		{
			static_assert(std::is_base_of_v<Component, ComponentType>, "Game Object: component type must be of type component");

			for (auto& component : mComponents)
			{
				if (component->GetTypeId() == ComponentType::StaticGetTypeId())
				{
					return static_cast<ComponentType*>(component.get());
				}
			}
			return nullptr;
		}

		template<class ComponentType>
		const ComponentType* GetComponent() const
		{
			static_assert(std::is_base_of_v<Component, ComponentType>, "Game Object: component type must be of type component");

			for (auto& component : mComponents)
			{
				if (component->GetTypeId() == ComponentType::StaticGetTypeId())
				{
					return static_cast<const ComponentType*>(component.get());
				}
			}
			return nullptr;
		}
#pragma endregion

		void Serialize(rapidjson::Document& doc);

		// magnetic property
		bool mHasAttraction = false;

	private:

		friend class GameWorld;
		using Components = std::vector<std::unique_ptr<Component>>;

		GameWorld* mWorld = nullptr;
		Components mComponents;
		GameObjectHandle mHandle;

		std::filesystem::path mTemplatePath;
		std::string mName = "EMPTY";
		bool mInitialized = false;
		uint32_t mUniqueId = 0;
	};
}
#pragma once

#include "GameObject.h"
#include "Service.h"

namespace FoxEngine
{
	class GameWorld final
	{
	public:

		void Initialize(uint32_t capacity);
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();

		GameObject* CreateGameObject(const std::filesystem::path& templateFile);
		GameObject* GetGameObject(const GameObjectHandle& handle);
		void DestroyObject(const GameObjectHandle& handle);

		template<class ServiceType>
		ServiceType* AddService()
		{
			static_assert(std::is_base_of_v<Service, ServiceType>, "GameWorld: service must be of type Service");
			ASSERT(!mInitialized, "GameWorld must be initialized");
			auto& newService = mServices.emplace_back(std::make_unique<ServiceType>());
			newService->mWorld = this;
			return static_cast<ServiceType*>(newService.get());
		}

		template<class ServiceType>
		ServiceType* GetService()
		{

			for(auto& s : mServices)
			{
				if (s->GetTypeId() == ServiceType::StaticGetTypeId())
				{
					return static_cast<ServiceType*>(s.get());
				}
			}
			return nullptr;
		}

		template<class ServiceType>
		const ServiceType* GetService() const
		{

			for (auto& s : mServices)
			{
				if (s->GetTypeId() == ServiceType::StaticGetTypeId())
				{
					return static_cast<ServiceType*>(s.get());
				}
			}
			return nullptr;
		}

	private:

		bool IsValid(const GameObjectHandle& handle);
		void ProcessDestroyList();

		struct Slot
		{
			std::unique_ptr<GameObject> gameObject;
			uint32_t generation = 0;
		};

		using Services = std::vector<std::unique_ptr<Service>>;
		using GameObjectSlots = std::vector<Slot>;

		Services mServices;

		GameObjectSlots mGameObjectSlots;
		std::vector<uint32_t> mFreeSlots;
		std::vector<uint32_t> mTobeDestroyed;

		bool mInitialized = false;
		bool mUpdating = false;
	};
}
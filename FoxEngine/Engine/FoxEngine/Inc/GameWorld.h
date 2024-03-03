#pragma once

#include "GameObject.h"
#include "Service.h"

namespace FoxEngine
{
	using CustomService = std::function<bool(const char*, const rapidjson::Value& value, GameWorld&)>;
	class GameWorld final
	{
	public:

		static void SetCustomServiceMake(CustomService customService);

		void Initialize(uint32_t capacity);
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();
		void EditorUI();

		GameObject* CreateGameObject(const std::filesystem::path& templateFile);
		GameObject* GetGameObject(const GameObjectHandle& handle);
		void DestroyObject(const GameObjectHandle& handle);

		void LoadLevel(const std::filesystem::path& levelFile);
		void SaveLevel(const std::filesystem::path& levelFile);
		void SaveTemplate(const std::filesystem::path& templateFile, const GameObjectHandle& handle);

		//SkySphere
		void SetSkySphereRenderCamera(Graphics::Camera& camera);
		void CreateSkySphere(const std::filesystem::path& templateFile, const float radius, const float skyRotationSpeed = 0.005f);

#pragma region templatized

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
#pragma endregion

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

		std::filesystem::path mLevelFile;
		std::vector<uint32_t> mFreeSlots;
		std::vector<uint32_t> mToBeDestroyed;

		bool mInitialized = false;
		bool mUpdating = false;

		//SkySphere

		float mSkyRotationRate = 0.005f;
		bool hasSkySphere = false;
		Graphics::SimpleEffect mSimpleEffect;
		Graphics::RenderObject mSkySphere{};
	};
}
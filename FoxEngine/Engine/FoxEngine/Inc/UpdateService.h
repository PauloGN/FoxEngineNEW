#pragma once

#include "Service.h"

namespace FoxEngine
{
	class Component;

	class UpdateService final : public Service
	{
	public:

		SET_TYPE_ID(ServiceId::Update)

		void Update(float deltaTime)override;

		//Serialize and deserialize
		void Deserialize(rapidjson::Value& value) override;

		void Register(Component* component);
		void Unregister(Component* component);

	private:
		void RegisterPendingUpdates();

		using UpdateComponents = std::vector<Component*>;
		UpdateComponents mUpdateComponent;
		UpdateComponents mPendingUpdateComponent;

		bool mIsUpdating = false;
	};
}
#pragma once

#include "Component.h"

namespace FoxEngine
{
	class TransformComponent;

	class MagnetifyComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Magnetify)

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void UpdateInRangeComponentsList();
		void UpdateOutOfRangeComponentsList();
		void Deserialize(const rapidjson::Value& value)override;

	private:

		std::list<TransformComponent*> mInRangeComponents;
		std::list<TransformComponent*> mOutOfRangeComponents;
		Vector3* mPosition {};

		bool mIsAttractive = true;
		float mMoveSpeed = 1.0f;
		float mEntryRadius = 2.0f;
		float mExitRadius = 10.0f;
	};
}
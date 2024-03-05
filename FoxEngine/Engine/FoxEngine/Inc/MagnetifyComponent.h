#pragma once

#include "Component.h"

namespace FoxEngine
{
	class TransformComponent;
	using CustomEffect = std::function<void(GameObject&)>;

	class MagnetifyComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Magnetify)

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void UpdateInRangeComponentsList();
		void UpdateOutOfRangeComponentsList();


		//void AddObject(GameObject& go);
		//void RemoveObject(GameObject& go);

		void AttractionEffect(const float dt);
		
		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(const rapidjson::Value& value)override;

	private:

		std::list<TransformComponent*> mInRangeComponents;
		std::list<TransformComponent*> mOutOfRangeComponents;
		Vector3* mPosition {};

		bool mIsAttractive = true;
		float mMoveSpeed = 1.0f;

		float minDistance = .5f;
		float mEntryRadius = 2.0f;
		float mExitRadius = 10.0f;

		float timer = 0;
		const float timeUpdateRate = .5f;
	};
}
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

		void AddObject(GameObject& go);
		void RemoveObject(GameObject& go);


		void EditorUI() override;
		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(const rapidjson::Value& value)override;

	private:

		void AttractionEffect(const float dt);
		void UpdateInRangeComponentsList();
		void UpdateOutOfRangeComponentsList();

		std::list<GameObject*> mInRangeComponents;
		std::list<GameObject*> mOutOfRangeComponents;
		Vector3* mPosition {};
		// TRUE = attract -- FALSE = repel
		bool mIsAttractive = false;
		//attraction or repeal speed
		float mMoveSpeed = 1.0f;
		//Min Distance to apply a custom behavior or effect
		float minDistance = .5f;
		//This close consider affect those objects
		float mEntryRadius = 20.0f;
		//This far do not consider affect those objects anymore
		float mExitRadius = 25.0f;

		//Update timer
		float timer = 0;
		const float timeUpdateRate = .5f;
	};
}
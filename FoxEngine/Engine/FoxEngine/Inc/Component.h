#pragma once
#include "TypeIds.h"

namespace FoxEngine
{
	class GameObject;

	class Component 
	{

	public:
		Component() = default;
		virtual~Component() = default;

		Component(const Component&) = delete;
		Component(const Component&&) = delete;
		Component&operator=(const Component&) = delete;
		Component&operator=(const Component&&) = delete;

		virtual void Initialize(){}
		virtual void Terminate(){}
		virtual void Update(const float deltaTime){}
		virtual void DebugUI(){}

		virtual uint32_t GetTypeId() const = 0;

		GameObject& GetOwner() { return*mOwner; }
		const GameObject& GetOwner()  const { return *mOwner; }

		//Serialize and deserialize
		virtual void Serialize(rapidjson::Document& doc, rapidjson::Value& value) {}
		virtual void Deserialize(rapidjson::Value& value){}

	private:

		//Gameobject also has access to private members
		friend class GameObject;
		GameObject* mOwner = nullptr;
	};
}
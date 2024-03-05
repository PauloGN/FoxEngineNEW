#pragma once
#include "TypeIds.h"

/**
 * \To Add a new component
 * New component should be derived from component class,
 * Add new members and implement any overrides
 * Add a new entry to componentId
 * Set the id using SET_TYPE_ID macro
 * Set Deserialize
 * Set Serialize
 * Update GameObjectFactory.cpp
 *
 *\GameObject has a list of components
 *
 */
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
		virtual void EditorUI(){}

		virtual uint32_t GetTypeId() const = 0;

		GameObject& GetOwner() { return *mOwner; }
		const GameObject& GetOwner()  const { return *mOwner; }

		//Serialize and deserialize
		virtual void Serialize(rapidjson::Document& doc, rapidjson::Value& value) {}
		virtual void Deserialize(const rapidjson::Value& value){}

	private:

		//Gameobject also has access to private members then it will assign this pointer to a valid data as soon as it gets attached to a game Object.
		friend class GameObject;
		GameObject* mOwner = nullptr;
	};
}
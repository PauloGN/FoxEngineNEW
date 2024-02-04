#pragma once

#include "TypeIds.h"

namespace FoxEngine
{
	class GameWorld;

	class Service
	{
	public:

		Service() = default;
		virtual ~Service() = default;

		Service(const Service&) = delete;
		Service(const Service&&) = delete;
		Service&operator=(const Service&) = delete;
		Service&operator=(const Service&&) = delete;

		virtual uint32_t GetTypeId() const = 0;

		virtual void Initialize() {}
		virtual void Terminate() {}

		virtual void Update(float deltaTime) {}
		virtual void Render() {}
		virtual void DebugUI() {}
		//Serialize and deserialize
		virtual void Serialize(rapidjson::Document& doc, rapidjson::Value& value) {}
		virtual void Deserialize(rapidjson::Value& value) {}

		GameWorld& GetWorld() { return*mWorld; }
		const GameWorld& GetWorld() const { return*mWorld; }

	private:

		friend class GameWorld;
		GameWorld* mWorld = nullptr;
	};
}
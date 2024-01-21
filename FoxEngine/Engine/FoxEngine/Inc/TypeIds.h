#pragma once

#include"Common.h"

namespace FoxEngine
{
	enum class ComponentId
	{
		Invalide,
		Transform,

		Count
	};

	//enum class NewComponentId
	//{
	//	newId0 = ComponentId::Count


	//};
}

#define SET_TYPE_ID(id)\
static uint32_t StaticGetTypeId() {return static_cast<uint32_t>(id);}\
uint32_t GetTypeId() const override {return StaticGetTypeId();}

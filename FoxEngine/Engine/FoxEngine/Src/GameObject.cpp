#include "Precompiled.h"
#include "GameObject.h"
#include "TransformComponent.h"

#include "GameWorld.h"
#include "FoxEngine.h"

using namespace FoxEngine;

static uint32_t gUniqueId = 0;

void FoxEngine::GameObject::Initialize()
{
	ASSERT(!mInitialized, "Game Object is already initialized");

	for (auto& component : mComponents)
	{
		component->Initialize();
	}

	mUniqueId = ++gUniqueId;
	mInitialized = true;

	ASSERT(HasA<TransformComponent>(), "Game Object must have a trasnform");
}

void FoxEngine::GameObject::Terminate()
{

	for (auto& component : mComponents)
	{
		component->Terminate();
	}
}

void FoxEngine::GameObject::DebugUI()
{
	if(ImGui::CollapsingHeader(mName.c_str(), ImGuiTreeNodeFlags_CollapsingHeader))
	{
		for (auto& component : mComponents)
		{
			component->DebugUI();
		}
	}
}

void GameObject::EditorUI()
{
	if (ImGui::CollapsingHeader(mName.c_str(), ImGuiTreeNodeFlags_CollapsingHeader))
	{
		for (auto& component : mComponents)
		{
			component->EditorUI();
		}

		if (ImGui::Button(("Edit: " + mName).c_str()))
		{
			// global data here to load the template
			MainApp().ChangeState("EditTemplateState");
		}
	}
}

void GameObject::Serialize(rapidjson::Document& doc)
{
	rapidjson::Value components(rapidjson::kObjectType);

	for(auto& component: mComponents)
	{
		component->Serialize(doc, components);
	}
	doc.SetObject();
	doc.AddMember("Components", components, doc.GetAllocator());
}

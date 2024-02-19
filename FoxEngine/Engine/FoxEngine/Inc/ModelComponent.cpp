#include "Precompiled.h"
#include "ModelComponent.h"

#include "GameWorld.h"
#include "RenderService.h"	

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

void ModelComponent::Initialize()
{
	ModelManager* mm = ModelManager::Get();
	mModelId = mm->LoadModel(mFileName);

	RenderService* rs = GetOwner().GetWorld().GetService<RenderService>();
	rs->Register(this);
}

void ModelComponent::Terminate()
{
	RenderService* rs = GetOwner().GetWorld().GetService<RenderService>();
	rs->Unregister(this);
}

void ModelComponent::Deserialize(const rapidjson::Value& value)
{
	if(value.HasMember("FileName"))
	{
		mFileName = value["FileName"].GetString();
	}
}

#include "Precompiled.h"
#include "ModelComponent.h"

#include "GameWorld.h"
#include "RenderService.h"
#include "SaveUtil.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;
using namespace FoxEngine::SaveUtil;

void ModelComponent::Initialize()
{
	ModelManager* mm = ModelManager::Get();

	mModelId = mm->GetModelId(mFileName);

	if(mm->GetModel(mModelId) == nullptr)
	{
		mModelId = mm->LoadModel(mFileName);
		for(const std::string& fileName : mAnimationFileNames)
		{
			mm->AddAnimation(mModelId, fileName);
		}
	}

	RenderService* rs = GetOwner().GetWorld().GetService<RenderService>();
	rs->Register(this);
}

void ModelComponent::Terminate()
{
	RenderService* rs = GetOwner().GetWorld().GetService<RenderService>();
	rs->Unregister(this);
}

void ModelComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	//for anything that is not just one value
	rapidjson::Value componentValue(rapidjson::kObjectType);
	//File Name model location
	SaveString("FileName", mFileName.c_str(), doc, componentValue);
	//Save cast shadow
	SaveBool("CastShadow", mCastShadow, doc, componentValue);
	//save all animations
	SaveStringArray("Animations", mAnimationFileNames, doc, componentValue);

	//Save component name/data 
	value.AddMember("ModelComponent", componentValue, doc.GetAllocator());
}

void ModelComponent::Deserialize(const rapidjson::Value& value)
{
	if(value.HasMember("FileName"))
	{
		mFileName = value["FileName"].GetString();
	}

	if (value.HasMember("Animations"))
	{
		auto animations = value["Animations"].GetArray();
		for (auto& animation : animations)
		{
			mAnimationFileNames.push_back(animation.GetString());
		}
	}

	if (value.HasMember("CastShadow"))
	{
		mCastShadow = value["CastShadow"].GetBool();
	}
}

const Graphics::Model& ModelComponent::GetModel() const
{
	return *Graphics::ModelManager::Get()->GetModel(mModelId);
}

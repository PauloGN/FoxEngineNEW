#include "Precompiled.h"
#include "SoundEffectComponent.h"
#include "GameObject.h"

using namespace FoxEngine;
using namespace FoxEngine::Audio;

void SoundEffectComponent::Initialize()
{
	ASSERT(!mFileName.empty(), "SoundEffectComponent: no sound file loaded");
	mSoundId = SoundEffectManager::Get()->Load(mFileName);
}

void SoundEffectComponent::Terminate()
{
	// should probably remove effect?
}

void SoundEffectComponent::Deserialize(const rapidjson::Value& value)
{
	if(value.HasMember("FileName"))
	{
		mFileName = value["FileName"].GetString();
	}
	if(value.HasMember("Looping"))
	{
		mLooping = value["Looping"].GetBool();
	}
}

void SoundEffectComponent::DebugUI()
{
	std::string buttonName = "PlaySound" + GetOwner().GetName();
	if(ImGui::Button(buttonName.c_str()))
	{
		Play();
	}
}

void SoundEffectComponent::Play()
{
	Stop();
	SoundEffectManager::Get()->Play(mSoundId, mLooping);
}

void SoundEffectComponent::Play(const bool looping)
{
	Stop();
	SoundEffectManager::Get()->Play(mSoundId, looping);
}

void SoundEffectComponent::Stop()
{
	SoundEffectManager::Get()->Stop(mSoundId);
}

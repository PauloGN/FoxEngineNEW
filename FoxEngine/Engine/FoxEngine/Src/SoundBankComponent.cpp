#include "Precompiled.h"
#include "SoundBankComponent.h"
#include "GameObject.h"

using namespace FoxEngine;
using namespace FoxEngine::Audio;

void SoundBankComponent::Initialize()
{
	SoundEffectManager* sm = SoundEffectManager::Get();
	for (auto& soundData : mSoundEffects)
	{
		soundData.second.soundId = sm->Load(soundData.second.fileName);
	}
}

void SoundBankComponent::Terminate()
{
	Component::Terminate();
}

void SoundBankComponent::Deserialize(const rapidjson::Value& value)
{
	if(value.HasMember("SoundEffects"))
	{
		auto soundEffects  = value["SoundEffects"].GetObj();
		for (auto& soundEffect : soundEffects)
		{
			SoundData& soundData = mSoundEffects[soundEffect.name.GetString()];
			if(soundEffect.value.HasMember("FileName"))
			{
				soundData.fileName = soundEffect.value["FileName"].GetString();
			}

			if (soundEffect.value.HasMember("Looping"))
			{
				soundData.isLooping = soundEffect.value["Looping"].GetBool();
			}
		}
	}
}

void SoundBankComponent::DebugUI()
{
	std::string buttonTag = "PlaySound: " + GetOwner().GetName();
	for (auto& soundData : mSoundEffects)
	{
		std::string buttonName = soundData.first + buttonTag;
		if (ImGui::Button(buttonName.c_str()))
		{
			Play(soundData.first);
		}
	}
}

void SoundBankComponent::Play(const std::string& key)
{
	SoundEffectManager* sm = SoundEffectManager::Get();
	auto soundData = mSoundEffects.find(key);

	if (soundData != mSoundEffects.end())
	{
		sm->Stop(soundData->second.soundId);
		sm->Play(soundData->second.soundId, soundData->second.isLooping);
	}
}

void SoundBankComponent::Play(const std::string& key, const bool looping)
{
	SoundEffectManager* sm = SoundEffectManager::Get();
	auto soundData = mSoundEffects.find(key);

	if (soundData != mSoundEffects.end())
	{
		sm->Stop(soundData->second.soundId);
		sm->Play(soundData->second.soundId, looping);
	}
}

void SoundBankComponent::StopAll()
{
	SoundEffectManager* sm = SoundEffectManager::Get();
	for (auto& soundData : mSoundEffects)
	{
		sm->Stop(soundData.second.soundId);
	}
}

void SoundBankComponent::Stop(const std::string& key)
{
	SoundEffectManager* sm = SoundEffectManager::Get();
	auto soundData = mSoundEffects.find(key);

	if(soundData != mSoundEffects.end())
	{
		sm->Stop(soundData->second.soundId);
	}
}

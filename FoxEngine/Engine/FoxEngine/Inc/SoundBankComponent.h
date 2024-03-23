#pragma once

#include "Component.h"

namespace FoxEngine
{
	class SoundBankComponent final : public Component
	{
	public:

		SET_TYPE_ID(ComponentId::SoundBank)

		void Initialize() override;
		void Terminate() override;

		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(const rapidjson::Value& value) override;

		void DebugUI() override;

		void Play(const std::string& key);
		void Play(const std::string& key, const bool looping);

		void StopAll();
		void Stop(const std::string& key);

	private:

		struct SoundData
		{
			std::string fileName;
			bool isLooping = false;
			Audio::SoundId soundId = 0;
		};
		using SoundEffects = std::unordered_map<std::string, SoundData>;
		SoundEffects mSoundEffects;
	};
}
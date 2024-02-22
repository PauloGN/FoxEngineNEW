#pragma once

#include "Component.h"

namespace FoxEngine
{
	class SoundEffectComponent final : public Component
	{
	public:

		SET_TYPE_ID(ComponentId::SoundEffect)

		void Initialize() override;
		void Terminate() override;

		void Deserialize(const rapidjson::Value& value) override;

		void DebugUI() override;

		void Play();
		void Play(const bool looping);
		void Stop();

	private:

		std::string mFileName;
		Audio::SoundId mSoundId = 0;
		bool mLooping = false;
	};
}

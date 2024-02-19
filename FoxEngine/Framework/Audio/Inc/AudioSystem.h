#pragma once


namespace FoxEngine::Audio
{
	class AudioSystem final
	{
	public:

		static void StaticInitialize();
		static void StaticTerminate();
		static AudioSystem* Get();

		AudioSystem();
		~AudioSystem();

		AudioSystem(const AudioSystem&) = delete;
		AudioSystem(const AudioSystem&&) = delete;
		AudioSystem& operator=(const AudioSystem&) = delete;
		AudioSystem& operator=(const AudioSystem&&) = delete;

		void Initialize();
		void Terminate();

		void Update();
		void Suspend();

		DirectX::AudioEngine* GetAudioEngine();

	private:

		DirectX::AudioEngine* mAudioEngine = nullptr;
		//friend class SoundEffectManager;
	};
}

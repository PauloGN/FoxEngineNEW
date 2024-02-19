#include "Precompiled.h"
#include "AudioSystem.h"

using namespace FoxEngine;
using namespace FoxEngine::Audio;
using namespace DirectX;

namespace 
{
	std::unique_ptr<AudioSystem> sAudioSystem;
}

void AudioSystem::StaticInitialize()
{
	ASSERT(sAudioSystem == nullptr, "AudionSystem: has already been initialized");
	sAudioSystem = std::make_unique<AudioSystem>();
	sAudioSystem->Initialize();
}

void AudioSystem::StaticTerminate()
{
	if(sAudioSystem != nullptr)
	{
		sAudioSystem->Terminate();
		sAudioSystem.reset();
	}
}

AudioSystem* AudioSystem::Get()
{
	ASSERT(sAudioSystem != nullptr, "AudionSystem: was not initialized");
	return sAudioSystem.get();
}

AudioSystem::AudioSystem()
	:mAudioEngine(nullptr)
{

}

AudioSystem::~AudioSystem()
{
	ASSERT(mAudioEngine == nullptr, "AudionSystem: Terminate was not called");
}

void AudioSystem::Initialize()
{
	ASSERT(mAudioEngine == nullptr, "AudionSystem: is already initialized");
	AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;

#if defined(_DEBUG)
	flags |= AudioEngine_Debug;
#endif

	mAudioEngine = new AudioEngine(flags);
}

void AudioSystem::Terminate()
{
	if (mAudioEngine != nullptr)
	{
		delete mAudioEngine;
	}
	mAudioEngine = nullptr;
}

void AudioSystem::Update()
{
	if(mAudioEngine != nullptr && mAudioEngine->Update())
	{
		//if not audio device active
		if(mAudioEngine->IsCriticalError())
		{
			LOG("AudioSystem: critical error, shutting down");
			Terminate();
		}
	}
}

void AudioSystem::Suspend()
{
	ASSERT(mAudioEngine != nullptr, "AudionSystem: Audio Engine is not available");
	mAudioEngine->Suspend();
}

DirectX::AudioEngine* AudioSystem::GetAudioEngine()
{
	return mAudioEngine;
}

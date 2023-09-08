#pragma once
#include "FoxEngine/Inc/FoxEngine.h"

class GameState : public FoxEngine::AppState
{
public:

	void Initialize()override;
	void Terminate()override;
	void Render()override;
	void Update(float deltaTime)override;
	void DebugUI() override;

	//consider to move these to AppState
	void EngineCameraController(float deltaTime);
	void EngineFPS(float deltaTime);

protected:
	FoxEngine::Graphics::Camera mCamera;
	FoxEngine::Graphics::DirectionalLight mDirectionalLight;

	FoxEngine::Graphics::StandardEffect mStandardEffect;
	FoxEngine::Graphics::GaussianBlurEffect mGaussianBlurEffect;
	FoxEngine::Graphics::PostProcessingEffect mPostProcessingEffect;

	FoxEngine::Graphics::RenderObject mSunRenderObj;
	FoxEngine::Graphics::RenderObject mEarthRenderObj;
	FoxEngine::Graphics::RenderObject mScreenQuad;

	FoxEngine::Graphics::RenderTarget mBaseRenderTarget;
	FoxEngine::Graphics::RenderTarget mBloomRenderTarget;

	//Earth orbit
	float mEarthOffset = 8.0f;
	float mEarthRotation = 0.0f;
	float mEarthRevolution = 0.0f;

	//FPS
	float mTimePassed = 0.0f;
	float mFPS = 0.0f;
	int mFrameCount = 0;
};
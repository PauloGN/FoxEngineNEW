#pragma once
#include "FoxEngine/Inc/FoxEngine.h"

using namespace FoxEngine;
using namespace FoxEngine::FoxMath;
using namespace FoxEngine::Graphics;

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
	
	Camera mCamera;
	DirectionalLight mDirectionalLight;
	StandardEffect mStandardEffect;

	PostProcessingEffect mPostProcessingEffect;
	RenderTarget mRenderTarget;

	RenderGroup mAlien;
	RenderObject mGround;
	RenderObject mScreenQuad;

	float mTimePassed = 0.0f;
	float mFPS = 0.0f;
	int mFrameCount = 0;
};


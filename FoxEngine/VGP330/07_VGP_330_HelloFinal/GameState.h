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

	void ModelTransform(float deltatime);

protected:

	Camera mCamera;
	DirectionalLight mDirectionalLight;
	StandardEffect mStandardEffect;
	SimpleEffect mSimpleEffect;

	PostProcessingEffect mPostProcessingEffect;;
	FoxEngine::Graphics::TerrainEffect mTerrainEffect;
	RenderTarget mRenderTarget;
	ShadowEffect mShadowEffect;
	//FBX
	RenderGroup mSpaceShip;

	//RenderObject mGround;
	RenderObject mScreenQuad;
	Texture mCombineTexture;
	RenderObject mSkySphere;

	//Terrain
	FoxEngine::Graphics::RenderObject mAlienGround;
	FoxEngine::Graphics::Terrain mTerrain;

	float mTimePassed = 0.0f;
	float mFPS = 0.0f;
	int mFrameCount = 0;
};
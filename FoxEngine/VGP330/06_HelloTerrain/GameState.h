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
	FoxEngine::Graphics::TerrainEffect mTerrainEffect;
	FoxEngine::Graphics::ShadowEffect mShadowEffect;

	FoxEngine::Graphics::RenderGroup mAlien; 	//FBX
	FoxEngine::Graphics::RenderObject mGround;
	FoxEngine::Graphics::Terrain mTerrain;

	float mTimePassed = 0.0f;
	float mFPS = 0.0f;
	int mFrameCount = 0;
};
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

	//Engine Tools
	void EngineCameraController(float deltaTime);
	void EngineFPS(float deltaTime);
	void CharacterAnimationControler(float deltaTime);


protected:
	
	Camera mCamera;
	DirectionalLight mDirectionalLight;
	StandardEffect mStandardEffect;
#pragma region Character
	ModelId mCharacterId;
	Animator mCharacterAnimator;
	RenderGroup mAlien;
	float moveSpeed = 12.0f;

#pragma endregion

	RenderObject mGround;

	float mTimePassed = 0.0f;
	float mFPS = 0.0f;
	int mFrameCount = 0;

#pragma region Skeleton info

	bool mDrawSkeleton = false;
	int mAnimationIndex = -1;
	float mAnimationScale = 1.0f;

#pragma endregion

#pragma region Quaternion info

	float mYaw = 0.0f;//Forward-Z_axis
	float mPitch = 0.0f;//UP-Y_axis
	float mRoll = 0.0f;//Right-X_axis

#pragma endregion

};
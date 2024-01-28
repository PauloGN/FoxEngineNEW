#pragma once

#include "FoxEngine/Inc/FoxEngine.h"

class GameState : public FoxEngine::AppState
{
public:
	
	void Initialize();
	void Terminate();
	void Update(float deltaTime);
	void Render();
	void DebugUI();

	//Engine Tools
	void EngineCameraController(float deltaTime, FoxEngine::Graphics::Camera& camera);
private:
	//Assignment Colors change
	void ChangeScreenColor();

	FoxEngine::GameObject mGameObject;
	FoxEngine::GameObject mCameraGameObject01;
	FoxEngine::GameObject mCameraGameObject02;

	FoxEngine::CameraService mCameraService;
};

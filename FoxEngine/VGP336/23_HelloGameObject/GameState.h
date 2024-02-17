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
	void EngineCameraController(float deltaTime);

private:

	//Assignment Colors change
	void ChangeScreenColor();

	FoxEngine::Graphics::Camera mCamera;
	FoxEngine::GameObject mGameObject;
	FoxEngine::GameObject mGameObject2;
};

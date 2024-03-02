#pragma once

#include "FoxEngine/Inc/FoxEngine.h"

class EditorState : public FoxEngine::AppState
{
public:
	
	void Initialize();
	void Terminate();
	void Update(float deltaTime);
	void Render();
	void DebugUI();

	//Engine Tools
	void SwapCamera();
private:
	//Colors change
	void ChangeScreenColor();

	FoxEngine::GameWorld mGameworld;
};

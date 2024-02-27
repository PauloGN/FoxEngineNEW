#pragma once

#include "FoxEngine/Inc/FoxEngine.h"

class EditTemplateState : public FoxEngine::AppState
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

	FoxEngine::GameWorld mGameworld;
};

#pragma once

#include "FoxEngine/Inc/AppState.h"

class GameState : public FoxEngine::AppState
{
public:
	
	void Initialize();
	void Terminate();
	void Update(float deltaTime);
	void Render();
	void DebugUI();

private:
	//Assignment Colors change
	void ChangeScreenColor();
};

#pragma once
#include "FoxEngine/Inc/FoxEngine.h"


class GameState : public FoxEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	
};


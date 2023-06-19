#pragma once
#include "FoxEngine/Inc/AppState.h"


using namespace FoxEngine;

class GameState : public AppState
{
public:

	void Initialize() override{}
	void Terminate() override{}
	void Render() override{}
	void DebugUI() override{}
	void Update(const float deltaTime) override{}

};
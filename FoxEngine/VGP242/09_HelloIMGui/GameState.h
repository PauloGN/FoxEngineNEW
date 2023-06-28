#pragma once
#include "FoxEngine/Inc/FoxEngine.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

enum class Shape
{
	S_Transform,
	S_Sphere,
	S_AABB,
	S_AABBFilled,
	S_Custom,
	S_MAX
};

class GameState : public FoxEngine::AppState
{
public:

	virtual ~GameState() override = default;

	void Initialize()override;
	void Terminate()override;
	void Update(float deltaTime)override;
	void Render()override;
	void DebugUI()override;
	void EngineCameraController(float deltaTime);

protected:

	Camera mCamera;
	Shape eShape = Shape::S_Transform;

	//ImGui controllers

	float mSphereRadius = 1.0f;
	Color mShpereColor = Colors::Green;
	
};

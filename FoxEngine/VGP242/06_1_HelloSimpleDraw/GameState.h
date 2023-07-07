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
	void EngineCameraController(float deltaTime);

	//
	void DebugUI()
	{
		SimpleDraw::AddAABB(2, 2, 2, 4, 4, 4, Colors::Red);
	}
protected:
	
	Camera mCamera;
	Shape eShape = Shape::S_Transform;
};


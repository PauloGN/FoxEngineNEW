#pragma once
#include "FoxEngine/Inc/FoxEngine.h"

using namespace FoxEngine::Graphics;


class GameState : public FoxEngine::AppState
{
public:

	virtual ~GameState() = default;
	void Initialize() override;
	void DebugUI() override;
	void Render() override;
	void Terminate() override;
	void Update(float deltaTime) override;

private:
	//Controllers
	void EngineCameraController(float deltaTime);

	Camera mCamera;
	RenderObject mRenderObject;
	RenderObject mRenderObject2;

	ConstantBuffer mConstantBuffer;
	VertexShader mVertexShader;
	PixelShader mPixelShader;
};

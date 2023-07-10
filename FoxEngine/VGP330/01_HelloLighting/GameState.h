#pragma once
#include "FoxEngine/Inc/FoxEngine.h"

using namespace FoxEngine;
using namespace FoxEngine::FoxMath;
using namespace FoxEngine::Graphics;

class GameState : public FoxEngine::AppState
{
public:

	void Initialize()override;
	void Terminate()override;
	void Render()override;
	void Update(float deltaTime)override;
	void DebugUI() override;

	void EngineCameraController(float deltaTime);

protected:
	
	//View port Camera/Main Camera
	Camera mCamera;

	//Holds-> ConstantBuffer, VertexShader, PixelShader, Sampler
	FoxEngine::Graphics::StandardEffect mStandardEffect;
	FoxEngine::Graphics::SimpleEffect mStandardEffectSky;

	//Holds-> Transform, MeshBuffer, Texture
	RenderObject mRenderObject;//this is the obj
	RenderObject mRenderSky;//this is the obj

};


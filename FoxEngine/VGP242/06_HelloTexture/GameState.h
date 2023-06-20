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
	void Update(float deltaTime)override;
	void Render()override;
	void FPSControl(float deltaTime);


protected:

	virtual void CreateTexture() = 0;

	Camera mCamera;
	ConstantBuffer mConstantBuffer;
	MeshBuffer mMesheBuffer;
	VertexShader mVertexShader;
	PixelShader mPixelShader;

	// *** Week 07 ***
	Texture mDiffuseTexture;
	Sampler mSampler;

	Texture mDiffuseTexture2;
	Sampler mSampler2;


};

class BasicCubeTextureDebug :public GameState 
{

public:

	void CreateTexture()override;

};

class BasicSphereTextureDebug :public GameState
{

public:

	void Initialize() override;

	void CreateTexture()override;
};


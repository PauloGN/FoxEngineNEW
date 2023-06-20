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


protected:

	virtual void CreateShape() = 0;

	Camera mCamera;
	ConstantBuffer mConstantBuffer;
	MeshBuffer mMesheBuffer;
	VertexShader mVertexShader;
	PixelShader mPixelShader;
};

class BasicCubeTextureDebug :public GameState 
{

public:

	void CreateShape()override;

};

class BasicSphereTextureDebug :public GameState
{

public:

	void CreateShape()override;
};

class Week4Example :public GameState
{

public:

	void CreateShape()override;
};

class BasicCil :public GameState
{

public:

	void CreateShape()override;
};

class BasicSphere:public GameState
{

public:

	void CreateShape()override;
};

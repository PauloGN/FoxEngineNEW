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

	struct Vertex
	{
		Vector3 position;
		Color color;
	};

	using Vertices = std::vector<Vertex>;
	Vertices mVertices;

	Camera mCamera;
	ConstantBuffer mConstantBuffer;
	MeshBuffer mMeshBuffer;
	VertexShader mVertexShader;
	PixelShader mPixelShader;
};

class ColorCube :public GameState 
{

public:

	void CreateShape()override;
};

class GrayCube :public GameState
{

public:
	void CreateShape()override;
};
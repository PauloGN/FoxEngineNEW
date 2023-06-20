#pragma once
#include "FoxEngine/Inc/FoxEngine.h"

using namespace FoxEngine;
using namespace FoxEngine::FoxMath;
using namespace FoxEngine::Graphics;

class GameState : public FoxEngine::AppState
{
public:
	
	struct DebugObjInfo
	{
		bool bShowObj = true;
		Color color = Colors::Aquamarine;
		Vector3 Position;
		Vector3 Position2;
		Vector3 Size;
		float scalarSize;
	};


	void Initialize()override;
	void Terminate()override;
	void Update(float deltaTime)override;
	void Render()override;
	void EngineCameraControll(float deltaTime);
	void DebugUI()override;
	void LoadDebug();

protected:

	virtual void CreateTexture() = 0;

	Camera mCamera;
	ConstantBuffer mConstantBuffer;
	MeshBuffer mMesheBuffer;
	VertexShader mVertexShader;
	PixelShader mPixelShader;

	// *** Week 08 ***
	Texture mDiffuseTexture;
	Sampler mSampler;

	Texture mDiffuseTexture2;
	Sampler mSampler2;

//=================================			DEBUG TOOLs
protected:
	/*
		Render (true or false)
		maipulate: color, position and size
	*/

	bool mDrawTextureObj = true;

	DebugObjInfo mSphere1;
	DebugObjInfo mSphere2;
	DebugObjInfo mAABB;
	Vector3 mPositionAABB;
	DebugObjInfo mFilledAABB;
	Vector3 mPositionFilledAABB;
	DebugObjInfo mGrid;

};

class BasicCubeTextureDebug :public GameState 
{

public:

	void Initialize()override;
	void CreateTexture()override;

};

class BasicSphereTextureDebug :public GameState
{

public:

	void Initialize() override;
	void CreateTexture()override;
};


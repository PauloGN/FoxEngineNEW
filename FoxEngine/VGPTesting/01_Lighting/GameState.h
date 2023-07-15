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
	void EngineFPS(float deltaTime);

protected:

	struct TransformData
	{
		Matrix4 world;
		Matrix4 wvp;
		Vector3 viewPosition;
		float padding;
	};


	TypedConstantBuffer<TransformData> mTransformBuffer;
	TypedConstantBuffer<DirectionalLight> mLightBuffer;
	TypedConstantBuffer<Material> mMaterialBuffer;

	DirectionalLight mDirectionalLight;
	Material mMaterial;

	MeshBuffer mSphereMeshBuffer;
	VertexShader mSphereVS;
	PixelShader mSheperePS;
	Sampler mSphereSampler;
	Texture mSphereTexture;

	//View port Camera/Main Camera
	Camera mCamera;

	//Holds-> ConstantBuffer, VertexShader, PixelShader, Sampler
	FoxEngine::Graphics::StandardEffect mStandardEffect;
	FoxEngine::Graphics::SimpleEffect mSimpleEffectSky;

	//Holds-> Transform, MeshBuffer, Texture
	RenderObject mRenderObject;//this is the obj
	RenderObject mRenderSky;//this is the obj

	float mTimePassed = 0.0f;
	float mFPS = 0.0f;
	int mFrameCount = 0.0f;
};
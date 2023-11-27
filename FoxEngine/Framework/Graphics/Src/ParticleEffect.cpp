#include "Precompiled.h"
#include "ParticleEffect.h"

#include "Camera.h"
#include "RenderObject.h"
#include "MeshBuffer.h"
#include "VertexTypes.h"


using namespace FoxEngine;
using namespace FoxEngine::Graphics;
using namespace FoxEngine::FoxMath;

void ParticleEffect::Initialize()
{
	std::filesystem::path filePath = L"../../Assets/Shaders/DoParticles.fx";
	mVertexShader.Initialize<VertexPC>(filePath);
	mPixelShader.Initialize(filePath);
	mTransformBuffer.Initialize();
	mBlendState.Initialize(BlendState::Mode::AlphaBlend);
}

void ParticleEffect::Terminate()
{
	mBlendState.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void ParticleEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mTransformBuffer.BindVS(0);
	mBlendState.Set();
}

void ParticleEffect::End()
{
	mBlendState.ClearState();
}

void ParticleEffect::Render(const RenderObject & renderObject)
{
	Matrix4 matWorld = renderObject.transform.GetMatrix4();
	Matrix4 matView = mCamera->GetViewMatrix();
	Matrix4 matProj = mCamera->GetProjectionMatrix();
	Matrix4 matFinal = matWorld * matView * matProj;
	
	TransformData data;
	data.wvp = Transpose(matFinal);
	mTransformBuffer.Update(data);

	renderObject.meshBuffer.Render();
}

void ParticleEffect::DebugUI()
{}

void ParticleEffect::SetCamera(const Camera & camera)
{
	mCamera = &camera;
}
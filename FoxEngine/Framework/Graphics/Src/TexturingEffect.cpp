#include "Precompiled.h"
#include "TexturingEffect.h"

#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

void FoxEngine::Graphics::TexturingEffect::Initialize()
{

	mVertexShader.Initialize<VertexPX>("../../Assets/Shaders/DoTexturing.fx");
	mPixelShader.Initialize("../../Assets/Shaders/DoTexturing.fx");

	mTransformBuffer.Initialize();

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mBlendState.Initialize(BlendState::Mode::Additive);
}

void FoxEngine::Graphics::TexturingEffect::Terminate()
{
	mBlendState.Terminate();
	mSampler.Terminate();

	mTransformBuffer.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void FoxEngine::Graphics::TexturingEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);

	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void FoxEngine::Graphics::TexturingEffect::End()
{}

void FoxEngine::Graphics::TexturingEffect::Render(const RenderObject & renderObject)
{
	ASSERT(mCamera != nullptr, "TexturingEffect -- no camera set!");

    const FoxMath::Matrix4& world = renderObject.transform.GetMatrix4();
    const FoxMath::Matrix4& view = mCamera->GetViewMatrix();
    const FoxMath::Matrix4& proj = mCamera->GetProjectionMatrix();

    mTransformBuffer.Update(FoxMath::Transpose(world * view * proj));

    if (renderObject.diffuseMapId)
    {
		mBlendState.Set();
    }

    auto tm = TextureManager::Get();
    tm->BindPS(renderObject.diffuseMapId, 0);

    renderObject.meshBuffer.Render();

	if (renderObject.diffuseMapId)
	{
		BlendState::ClearState;
	}
} 

void FoxEngine::Graphics::TexturingEffect::SetCamera(const Camera & camera)
{
	mCamera = &camera;
}

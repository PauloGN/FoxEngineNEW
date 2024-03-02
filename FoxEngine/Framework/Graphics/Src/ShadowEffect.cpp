#include "Precompiled.h"
#include "ShadowEffect.h"

#include "AnimationUtil.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace FoxEngine; 
using namespace FoxEngine::Graphics;

static constexpr size_t MaxBoneCount = 256;

void FoxEngine::Graphics::ShadowEffect::Initialize()
{
	mLightCamera.SetMode(Camera::ProjectionMode::Orthographic);

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Shadow.fx";
	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTransformBuffer.Initialize();
	mBoneTransform.Initialize(MaxBoneCount * sizeof(FoxMath::Matrix4));

	constexpr uint32_t depthMapResolution = 4096;
	mDepthMapRenderTarget.Initialize(depthMapResolution, depthMapResolution, Texture::Format::RGBA_U32);
}

void FoxEngine::Graphics::ShadowEffect::Terminate()
{
	mDepthMapRenderTarget.Terminate();
	mBoneTransform.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void FoxEngine::Graphics::ShadowEffect::Begin()
{
	UpdateLightCamera();

	mVertexShader.Bind();
	mPixelShader.Bind();
	mTransformBuffer.BindVS(0);
	mBoneTransform.BindVS(1);

	mDepthMapRenderTarget.BeginRender();
}

void FoxEngine::Graphics::ShadowEffect::End()
{
	mDepthMapRenderTarget.EndRender();
}

void FoxEngine::Graphics::ShadowEffect::Render(const RenderObject & renderObject)
{
	const auto& matWorld = renderObject.transform.GetMatrix4();
	const auto& matView = mLightCamera.GetViewMatrix();
	const auto& matProj = mLightCamera.GetProjectionMatrix();

	TransformData data;
	data.wvp = FoxMath::Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);

	if(renderObject.skeleton != nullptr)
	{
		AnimationUtil::BoneTransforms boneTransforms;
		AnimationUtil::ComputeBoneTransform(renderObject.modelId, boneTransforms, renderObject.animator);
		AnimationUtil::ApplyBoneOffsets(renderObject.modelId, boneTransforms);
		for(FoxMath::Matrix4& transform: boneTransforms)
		{
			transform = FoxMath::Transpose(transform);
		}

		boneTransforms.resize(MaxBoneCount);
		mBoneTransform.Update(boneTransforms.data());
	}

	renderObject.meshBuffer.Render();
}

void FoxEngine::Graphics::ShadowEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("ShadowEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("DepthMap");
		ImGui::Image(
			mDepthMapRenderTarget.GetRawData(),
			{144, 144},
			{0, 0},
			{1, 1},
			{ 1, 1, 1, 1},
			{1, 1, 1, 1});
		ImGui::DragFloat("Size##", &mSize, 1.0f, 1.0f, 1000.0f);
	}
}

void FoxEngine::Graphics::ShadowEffect::UpdateLightCamera()
{
	ASSERT(mDirectionalLight != nullptr, "Shadow: no light set!");

	const FoxMath::Vector3& direction = mDirectionalLight->direction;
	mLightCamera.SetDirection(direction);
	mLightCamera.SetPosition(mFocusPosition - (direction * 1000.0f));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(2000.0f);
	mLightCamera.SetSize(mSize, mSize);
}
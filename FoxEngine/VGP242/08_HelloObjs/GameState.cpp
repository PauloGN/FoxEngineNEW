#include "GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;
using namespace FoxEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 0.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mRenderTargetCamera.SetPosition({ 0.0f, 2.0f, -1.0f });
    mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
    mRenderTargetCamera.SetAspectRatio(1.0f);

    mConstantBuffer.Initialize(sizeof(Matrix4));

    //MeshPX cube = MeshBuilder::CreateSkybox(100.0f);
    //MeshPC rect = MeshBuilder::CreateRectPC(1.0f, 2.0f, 3.0f);
    //MeshPC plane = MeshBuilder::CreatePlanePC(3, 3, 1.0f);
    //MeshPC cylinder = MeshBuilder::CreateCylinderPC(8, 2);
    //MeshPX sphere = MeshBuilder::CreateSpherePX(30, 30, 1.0f);
    //mMeshBuffer.Initialize(sphere);

    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexturing.fx";
    mVertexShader.Initialize<VertexPX>(shaderFile);
    mPixelShader.Initialize(shaderFile);

    //mDiffuseTexture.Initialize(L"../../Assets/Textures/earth.jpg");
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    mWorldTrans = Matrix4::Identity;

    constexpr uint32_t size = 1080;
    mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U32);

    mSkySphere.Initialize();
    mPlanets.Initialize();
    mEarth2.Initialize();
    mEarth2.SetPosition({ 10.0f, 0.0f, 0.0f });
    mPlanets.SetPosition({ 0, 5.0f, 0.0f });
}
void GameState::Terminate()
{
    mEarth2.Terminate();
    mPlanets.Terminate();
    mSkySphere.Terminate();

    mRenderTarget.Terminate();
    mSampler.Terminate();
    //mDiffuseTexture.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    //mMeshBuffer.Terminate();
    mConstantBuffer.Terminate();
}

void GameState::Render()
{
    if (!mDrawWorld)
    {
        return;
    }
    mVertexShader.Bind();
    mPixelShader.Bind();
    //mDiffuseTexture.BindPS(0);
    mSampler.BindPS(0);

    mRenderTarget.BeginRender();
    mPlanets.Render(mRenderTargetCamera, mConstantBuffer, false);
    mRenderTarget.EndRender();
    RenderMesh(mCamera, true);
}
void GameState::RenderMesh(const Camera& camera, bool useTransform)
{
    mSkySphere.Render(camera, mConstantBuffer, useTransform);
    mPlanets.Render(camera, mConstantBuffer, useTransform);
    mEarth2.Render(camera, mConstantBuffer, useTransform);
    //mMeshBuffer.Render();
}

void GameState::Update(float deltaTime)
{
    auto input = InputSystem::Get();
    const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f) * deltaTime;
    const float turnSpeed = 0.1f * deltaTime;
    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed);
    }
    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
    }
}
void GameState::DebugUI()
{
    ImGui::Begin("Debug Draw", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Checkbox("Draw World", &mDrawWorld);
    ImGui::Checkbox("Draw Sphere 1", &mDrawSphere1);
    if (mDrawSphere1)
    {
        ImGui::ColorEdit4("Sphere 1 Color", &mSphere1Color.r);
        ImGui::DragFloat3("Sphere 1 Pos", &mSphere1Pos.x, 0.1f);
        SimpleDraw::AddSphere(30, 30, 1.0f, mSphere1Pos, mSphere1Color);
    }

    SimpleDraw::AddCircle(60, 10.0f, Colors::Aqua);
    ImGui::Checkbox("Draw Sphere 2", &mDrawSphere2);
    if (mDrawSphere2)
    {
        ImGui::ColorEdit4("Sphere 2 Color", &mSphere2Color.r);
        ImGui::DragFloat3("Sphere 2 Pos", &mSphere2Pos.x, 0.1f);
        SimpleDraw::AddSphere(60, 60, 1.0f, mSphere2Pos, mSphere2Color);
    }
    SimpleDraw::Render(mCamera);

    ImGui::Text("Render Target");
    ImGui::Image(
        mRenderTarget.GetRawData(),
        { 128, 128 },
        { 0, 0 },
        { 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 }
    );

    ImGui::End();
}
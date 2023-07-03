#include "GameState.h"
using namespace FoxEngine;
using namespace FoxEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
    mCamera.SetLookAt(Vector3::Zero);

    MeshPX sphere = MeshBuilder::CreateSpherePX(30, 20, 1);
    mRenderObject.meshBuffer.Initialize(sphere);
    mRenderObject.mDiffuseTexture.Initialize(L"../../Assets/Images/water/water_texture.jpg");
    mBasktBall.meshBuffer.Initialize(sphere);
    mBasktBall.mDiffuseTexture.Initialize(L"../../Assets/Images/misc/basketball.jpg");

	//Buffer and rendering
	mSimpleEffect.Initialize();
    mSimpleEffect.SetCamera(mCamera);

    //Render Target
    constexpr uint32_t size = 512;
    mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U32);
    mSimpleEffectRenderTarget.Initialize();
    mSimpleEffectRenderTarget.SetCamera(mRenderTargetCamera);
    mRenderTargetCamera.SetAspectRatio(1.0f);
    mRenderTargetCamera.SetLookAt(Vector3::Zero);
};
void GameState::DebugUI()
{
    //ImGui

    ImGui::Begin("Controls##", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    Vector3 pos = mRenderObject.transform.position;
    if(ImGui::DragFloat3("Sphere Position##", &pos.x, 0.1f, -5.0f, 5.f))
    {
        mRenderObject.transform.position = pos;
    }

    Quaternion rotation = mRenderObject.transform.rotation;
    if (ImGui::DragFloat4("Sphere Rotation##", &rotation.x, 0.1f))
    {
        mRenderObject.transform.rotation = rotation;
    }

    Vector3 scale = mRenderObject.transform.scale;
    if (ImGui::DragFloat3("Sphere Scale##", &scale.x, 0.1f))
    {
        mRenderObject.transform.scale = scale;
    }

    ImGui::Separator();
    ImGui::Text("RenderTarget");

    ImGui::Image(
    mRenderTarget.GetRawData(),//image data
        {128, 128},//size of the image
        {0, 0},
        {1,1},
        { 1,1,1,1 },
        {1,1,1,1}
    );

    ImGui::End();

    //Draw SimpleDraw stuffs
    SimpleDraw::AddGroundPlane(55, Colors::White);
    SimpleDraw::Render(mCamera);
};
void GameState::Render()
{
	mSimpleEffect.Begin();
		mSimpleEffect.Render(mRenderObject);
		mSimpleEffect.Render(mBasktBall);
	mSimpleEffect.End();

    //Render Target
    mRenderTarget.BeginRender();
	    mSimpleEffectRenderTarget.Begin();
        mSimpleEffectRenderTarget.Render(mRenderObject);
        mSimpleEffectRenderTarget.Render(mBasktBall);
	    mSimpleEffectRenderTarget.End();
    mRenderTarget.EndRender();
};
void GameState::Terminate()
{
    mSimpleEffectRenderTarget.Terminate();
    mRenderTarget.Terminate();
    mSimpleEffect.Terminate();
    mRenderObject.Terminate();
    mBasktBall.Terminate();
};
void GameState::Update(float deltaTime)
{
    EngineCameraController(deltaTime);
    mBasktBall.transform.position.x += 0.2 * deltaTime;

   const Vector3 lookAtBasket = {mBasktBall.transform.position.x,
        mBasktBall.transform.position.y,
        mBasktBall.transform.position.z - 3,
    };
    mRenderTargetCamera.SetPosition(lookAtBasket);

};

void GameState::EngineCameraController(float deltaTime)
{
    auto input = InputSystem::Get();
    const int moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10 : 1;
    const float d = 0.01f * moveSpeed;


    //Foward and Backward
    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(d);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-d);
    }
    //Right and Left
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(d);
    }
    else if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-d);
    }

    //Rotation
    if (input->IsMouseDown(MouseButton::RBUTTON))
    {

        const int turnSpeedMultiplyer = input->IsKeyDown(KeyCode::LSHIFT) ? 2 : 1;

        const float x = input->GetMouseMoveX() * deltaTime * turnSpeedMultiplyer;
        const float y = input->GetMouseMoveY() * deltaTime * turnSpeedMultiplyer;

        mCamera.Yaw(x);
        mCamera.Pitch(y);

    }

    //UP and Down

    if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-d);
    }
    else if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(d);
    }
}
#include "GameState.h"
using namespace FoxEngine;
using namespace FoxEngine::Input;

void GameState::Initialize()
{
    //Main Camera
    mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
    mCamera.SetLookAt(Vector3::Zero);

    //Create a shape
    MeshPX sphere = MeshBuilder::CreateSpherePX(30, 20, 1);

    //Initialize meshBuffer and give it a texture to draw
    mRenderObject.meshBuffer.Initialize(sphere);
    mRenderObject.diffuseMapId = TextureManager::Get()->LoadTexture(L"earth.jpg");

    mBasktBall.meshBuffer.Initialize(sphere);

    mBasktBall.diffuseMapId = TextureManager::Get()->LoadTexture(L"earth.jpg");

	//ConstantBuffer, rendering, Matrix4 calculus
	mSimpleEffect.Initialize();
    mSimpleEffect.SetCamera(mCamera);

    //Render Target
    constexpr uint32_t size = 128;
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
    //constant buffer and more for the main view
	mSimpleEffect.Begin();
		mSimpleEffect.Render(mRenderObject);
		mSimpleEffect.Render(mBasktBall);
	mSimpleEffect.End();

    //Render Target stuffs
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
    //FPS movements for the view port
    EngineCameraController(deltaTime);
    //Apply simple movement for the  basket ball mesh
    mBasktBall.transform.position.x += 0.2 * deltaTime;
    //render basket ball mesh in a new position every frame
    Vector3 lookAtBasket = {mBasktBall.transform.position.x,
        mBasktBall.transform.position.y,
        mBasktBall.transform.position.z - 3,
    };

    mRenderTargetCamera.SetPosition(lookAtBasket);

    auto input = InputSystem::Get();

    if(input->IsKeyPressed(KeyCode::UP))
    {
        MainApp().ChangeState("SecondGameState");
    }else if(input->IsKeyPressed(KeyCode::DOWN))
    {
        MainApp().ChangeState("GameState");
       
    }
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
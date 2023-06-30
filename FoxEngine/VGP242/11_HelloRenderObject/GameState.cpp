#include "GameState.h"
using namespace FoxEngine::Input;

void GameState::Initialize()
{
    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";

    //Create a Vertex shader
	mVertexShader.Initialize<VertexPC>(shaderFile);
    //Create a pixel shader
    mPixelShader.Initialize(shaderFile);

    mCamera.SetPosition({ 0.0f, 0.0f, -5.0f });
    mCamera.SetLookAt(Vector3::Zero);

    mConstantBuffer.Initialize(sizeof(Matrix4));

    MeshPC sphere = MeshBuilder::CreateSpherePC(30, 20, 1);
    mRenderObject.meshBuffer.Initialize(sphere);
};
void GameState::DebugUI()
{

};
void GameState::Render()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

	Matrix4 matworld = mRenderObject.transform.GetMatrix4();
    Matrix4 matView = mCamera.GetViewMatrix();
    Matrix4 matProj = mCamera.GetProjectionMatrix();
    Matrix4 matFinal = matworld * matView * matProj;
    Matrix4 wvp = Transpose(matFinal);

    mConstantBuffer.Update(&wvp);
    mConstantBuffer.BindVS(0);

    mRenderObject.meshBuffer.Render();

};
void GameState::Terminate()
{
    mRenderObject.Terminate();
    mConstantBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
};
void GameState::Update(float deltaTime)
{
    EngineCameraController(deltaTime);
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

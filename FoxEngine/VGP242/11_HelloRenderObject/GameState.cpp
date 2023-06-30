#include "GameState.h"
using namespace FoxEngine;
using namespace FoxEngine::Input;

void GameState::Initialize()
{
    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";

    //Create a Vertex shader
	mVertexShader.Initialize<VertexPC>(shaderFile);
    //Create a pixel shader
    mPixelShader.Initialize(shaderFile);

    mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
    mCamera.SetLookAt(Vector3::Zero);

    mConstantBuffer.Initialize(sizeof(Matrix4));

    MeshPC sphere = MeshBuilder::CreateSpherePC(30, 20, 1);
    mRenderObject.meshBuffer.Initialize(sphere);
    mRenderObject2.meshBuffer.Initialize(sphere);
    mRenderObject2.transform.position = {2.0f, 2.0f, 2.0f};

};
void GameState::DebugUI()
{
    //ImGui

    ImGui::Begin("Controls##", nullptr, ImGuiWindowFlags_DockNodeHost);

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

    ImGui::End();

    //Draw SimpleDraw stuffs
    SimpleDraw::AddSphere(30,20,1, Vector3::One, Colors::Red);
    SimpleDraw::AddGroundPlane(55, Colors::White);
    SimpleDraw::Render(mCamera);
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

	//SECOND OBJ
    matworld = mRenderObject2.transform.GetMatrix4();
    matView = mCamera.GetViewMatrix();
    matProj = mCamera.GetProjectionMatrix();
    matFinal = matworld * matView * matProj;
    wvp = Transpose(matFinal);

    mConstantBuffer.Update(&wvp);
    mConstantBuffer.BindVS(0);

    mRenderObject2.meshBuffer.Render();

};
void GameState::Terminate()
{
    mRenderObject.Terminate();
    mRenderObject2.Terminate();
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
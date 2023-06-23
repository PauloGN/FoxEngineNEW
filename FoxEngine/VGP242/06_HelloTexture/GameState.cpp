#include "GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;


namespace
{

	float gRotationY = 0.0f;
	float gRotationX = 0.0f;

}

void GameState::Initialize() 
{
	//NDC - Normalized Device Coordinate
	// create shapes

	CreateTexture();

	//Initialize the camera
	mCamera.SetPosition(FoxMath::Vector3(0.f, 1.f, -3.f));//Offset back in z
	mCamera.SetLookAt(FoxMath::Vector3(0.f, 0.f, 0.f));

	mConstantBuffer.Initialize(sizeof(Matrix4));

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexturing.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	//Texture
	//mDiffuseTexture.Initialize(L"../../Assets/Textures/earth.jpg");
	mDiffuseTexture.Initialize(L"../../Assets/Images/samples/happy.jpg");


	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

}
void GameState::Terminate() 
{
	mSampler.Terminate();
	mDiffuseTexture.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMesheBuffer.Terminate();
	mConstantBuffer.Terminate();
}
void GameState::Update(float deltaTime) 
{
	auto input = InputSystem::Get();
	App& myApp = MainApp();


	//Change State

	if (input->IsKeyPressed(KeyCode::HOME))
	{
		GraphicsSystem::Get()->SetClearColor(Colors::Black);
		myApp.ChangeState("BasicCubeTexture");

	}
	else if (input->IsKeyPressed(KeyCode::END))
	{
		GraphicsSystem::Get()->SetClearColor(Colors::Blue);
		myApp.ChangeState("SkyBox");
	}


	//Rotation control
	if (input->IsKeyDown(KeyCode::UP)) {

		gRotationX += Constants::HalfPi * deltaTime * 0.5f;

	}
	else if (input->IsKeyDown(KeyCode::DOWN)) {

		gRotationX += -Constants::HalfPi * deltaTime * 0.5f;

	}

	if (input->IsKeyDown(KeyCode::RIGHT)) {

		gRotationY += -Constants::HalfPi * deltaTime * 0.5f;

	}
	else if (input->IsKeyDown(KeyCode::LEFT)) {

		gRotationY += Constants::HalfPi * deltaTime * 0.5f;

	}


	EngineCameraController(deltaTime);

}
void GameState::Render() 
{

	mVertexShader.Bind();
	mPixelShader.Bind();
	//Texture

	//mSampler.BindVS(0);
	//mDiffuseTexture.BindVS(0);
	mDiffuseTexture.BindPS(0);
	mSampler.BindPS(0);


	//Use the Camera

	Matrix4 matWorld = Matrix4::RotationY(gRotationY) * Matrix4::RotationX(gRotationX);
	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();
	Matrix4 matFinal = matWorld * matView * matProj;

	Matrix4 wvp = Transpose(matFinal);

	mConstantBuffer.Update(&wvp);
	mConstantBuffer.BindVS(0);

	mMesheBuffer.Render();
}

void GameState::EngineCameraController(float deltaTime)
{
	auto input = Input::InputSystem::Get();
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

//=======================================		Week 07 		=================================\\

void BasicCubeTextureDebug::CreateTexture()
{
	MeshPX cube = MeshBuilder::CreateCubePX(1.0);
	mMesheBuffer.Initialize(cube);

	//MeshPX Sphere = MeshBuilder::CreateSpherePX(30, 30, .8f);
	//mMesheBuffer.Initialize(Sphere);

	//MeshPX plane = MeshBuilder::CreatePlanePX(4, 4, 0.1f);
	//mMesheBuffer.Initialize(plane);

}

/// <summary>
/// Sky box
/// </summary>

void BasicSphereTextureDebug::CreateTexture()
{
	MeshPX skybox = MeshBuilder::CreateSkyboxPX(100.0);
	mMesheBuffer.Initialize(skybox);
}

void BasicSphereTextureDebug::Initialize()
{

	CreateTexture();

	//Initialize the camera
	mCamera.SetPosition(FoxMath::Vector3(0.f, 1.f, -3.f));//Offset back in z
	mCamera.SetLookAt(FoxMath::Vector3(0.f, 0.f, 0.f));

	mConstantBuffer.Initialize(sizeof(Matrix4));

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexturing.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	//Texture
	mDiffuseTexture.Initialize(L"../../Assets/Images/skybox/skybox1.png");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

}

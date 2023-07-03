#include "GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;


namespace
{
	float gRotationY = 0.0f;
	float gRotationX = 0.0f;
	App& myApp = MainApp();
}

void GameState::Initialize() 
{
	//NDC - Normalized Device Coordinate
	// create shapes

	CreateShape();

	//Initialize the camera
	mCamera.SetPosition(FoxMath::Vector3(0.f, 1.f, -3.f));//Offset back in z
	mCamera.SetLookAt(FoxMath::Vector3(0.f, 0.f, 0.f));

	mConstantBuffer.Initialize(sizeof(Matrix4));

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";
	mVertexShader.Initialize(shaderFile, VE_Position | VE_Color);
	mPixelShader.Initialize(shaderFile);
}
void GameState::Terminate() 
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMesheBuffer.Terminate();
	mConstantBuffer.Terminate();
}
void GameState::Update(float deltaTime) 
{
	auto input = InputSystem::Get();

	//update rotation
	gRotationY += Constants::HalfPi * deltaTime * 0.5f;
	gRotationX += Constants::HalfPi * deltaTime * 0.2f;


	//A state with a new shape
	if (input->IsKeyPressed(KeyCode::UP)) {

		GraphicsSystem::Get()->SetClearColor(Colors::Gray);
		myApp.ChangeState("BasicCube");

	}
	else if (input->IsKeyPressed(KeyCode::DOWN)) {

		GraphicsSystem::Get()->SetClearColor(Colors::Blue);
		myApp.ChangeState("BasicRec");

	}
	else if (input->IsKeyPressed(KeyCode::RIGHT)) {

		GraphicsSystem::Get()->SetClearColor(Colors::Red);
		myApp.ChangeState("Week4Example");
	}
	else if (input->IsKeyPressed(KeyCode::LEFT)) {

		GraphicsSystem::Get()->SetClearColor(Colors::DarkGreen);
		myApp.ChangeState("BasicCil");
	}
	else if (input->IsKeyPressed(KeyCode::LSHIFT))
	{
		GraphicsSystem::Get()->SetClearColor(Colors::DarkOrange);
		myApp.ChangeState("BasicSphere");
	}

}
void GameState::Render() 
{

	mVertexShader.Bind();
	mPixelShader.Bind();

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

//Week 06

void BasicCubeTextureDebug::CreateShape()
{
	MeshPC cube = MeshBuilder::CreateCubePC(1.0, Colors::Aqua);
	mMesheBuffer.Initialize(cube);
}

void BasicSphereTextureDebug::CreateShape()
{
	MeshPC rec = MeshBuilder::CreateRectPC(1.0f, 0.5f, 0.2f, Colors::AliceBlue);
	mMesheBuffer.Initialize(rec);
}

void Week4Example::CreateShape()
{
	MeshPC plane = MeshBuilder::CreatePlanePC(3,3,1.0f);
	mMesheBuffer.Initialize(plane);
}

void BasicCil::CreateShape()
{	
	MeshPC cil = MeshBuilder::CreateCylinderPC(8, 2);
	mMesheBuffer.Initialize(cil);
}

void BasicSphere::CreateShape()
{
	MeshPC sphere = MeshBuilder::CreateSpherePC(12, 8, 1.0f);
	mMesheBuffer.Initialize(sphere);
}

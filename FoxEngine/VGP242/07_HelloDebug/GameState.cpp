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
{}
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
		myApp.ChangeState("BasicCubeTextureDebug");

	}
	else if (input->IsKeyPressed(KeyCode::END))
	{
		GraphicsSystem::Get()->SetClearColor(Colors::Blue);
		myApp.ChangeState("BasicSphereTextureDebug");
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

	if (!mDrawTextureObj)
	{
		return;
	}


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

//========================================    DEBUG UI ========================================\\

void GameState::DebugUI()
{
	//Debug section week 08
	//mBlendState.Initialize(BlendState::Mode::AlphaBlend); //into simpledraw initialize // into run app.cpp

	ImGui::Begin("Debug Draw", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Checkbox("Draw Texture", &mDrawTextureObj);

	ImGui::Checkbox("Draw Sphere1", &mSphere1.bShowObj);
	if (mSphere1.bShowObj)
	{
		ImGui::ColorEdit4("Sphere1 Color", &mSphere1.color.r);
		ImGui::DragFloat3("Sphere1 pos:", &mSphere1.Position.x, .02f);
		ImGui::DragFloat("Sphere1 Size:", &mSphere1.scalarSize, .02f);
		SimpleDraw::AddSphere(30, 30, mSphere1.scalarSize, mSphere1.Position, mSphere1.color);
	}

	ImGui::Checkbox("Draw Sphere2", &mSphere2.bShowObj);
	if (mSphere2.bShowObj)
	{
		ImGui::ColorEdit4("Sphere2 Color", &mSphere2.color.r);
		ImGui::DragFloat3("Sphere2 pos:", &mSphere2.Position.x, .02f);
		ImGui::DragFloat("Sphere2 Size:", &mSphere2.scalarSize, .02f);
		SimpleDraw::AddSphere(30, 30, mSphere2.scalarSize, mSphere2.Position, mSphere2.color);
	}

	ImGui::Checkbox("Draw AABB", &mAABB.bShowObj);
	if (mAABB.bShowObj)
	{
		ImGui::DragFloat3("AABB Pos: ", &mPositionAABB.x, .02f);
		ImGui::DragFloat3("AABB min Pos: ", &mAABB.Position.x, .02f);
		ImGui::DragFloat3("AABB max Pos: ", &mAABB.Position2.x, .02f);
		ImGui::ColorEdit4("AABB Color", &mAABB.color.r);
		SimpleDraw::AddAABB(mAABB.Position + mPositionAABB, mAABB.Position2 + mPositionAABB, mAABB.color);
	}

	ImGui::Checkbox("Draw FilledAABB", &mFilledAABB.bShowObj);
	if (mFilledAABB.bShowObj)
	{
		
		ImGui::DragFloat3("FilledAABB Pos: ", &mPositionFilledAABB.x, .02f);

		ImGui::ColorEdit4("FilledAABB Color", &mFilledAABB.color.r);
		SimpleDraw::AddFilledAABB(mFilledAABB.Position + mPositionFilledAABB, mFilledAABB.Position2 + mPositionFilledAABB, mFilledAABB.color);

	}

	ImGui::Checkbox("Draw Grid", &mGrid.bShowObj);
	if (mGrid.bShowObj)
	{

		int size = static_cast<int>(mGrid.scalarSize);

		ImGui::ColorEdit4("Grid Color", &mGrid.color.r);
		ImGui::DragInt("Grid Size", &size);
		SimpleDraw::AddGroundPlane(size, mGrid.color);
		mGrid.scalarSize = size;
	}

		SimpleDraw::Render(mCamera);

		ImGui::End();

}

void GameState::LoadDebug()
{
	mSphere1.Position = { -3.0f, 0.0f, 0.0f };
	mSphere1.scalarSize = 1.0f;
	mSphere1.color = Colors::OrangeRed;


	mSphere2.Position = { 3.0f, 0.0f, 0.0f };
	mSphere2.scalarSize = 1.0f;
	mSphere2.color = Colors::Green;

	mFilledAABB.color = Colors::Red;
	mFilledAABB.Position = { -2.0f, 1.0f,0.0f };
	mFilledAABB.Position2 = { 2.0f, 2.0f, 2.0f };
	mPositionFilledAABB = Vector3::Zero;
	
	mAABB.color = Colors::BurlyWood;
	mAABB.Position = { -2,2,0 };
	mAABB.Position2 = { 2,4,2 };
	mPositionAABB = Vector3::Zero;

	mGrid.color = Colors::Aqua;
	mGrid.scalarSize = 100;

}

//=======================================		Week 08 Hello Debug		=================================\\

void BasicCubeTextureDebug::Initialize()
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
	mDiffuseTexture.Initialize(L"../../Assets/Images/samples/happy.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	LoadDebug();

}

void BasicCubeTextureDebug::CreateTexture()
{

	MeshPX cube = MeshBuilder::CreateCubePX(1.0);
	mMesheBuffer.Initialize(cube);


	//MeshPX plane = MeshBuilder::CreatePlanePX(4, 4, 0.1f);
	//mMesheBuffer.Initialize(plane);

}

//=================================================  SPHERE DEBUG

void BasicSphereTextureDebug::CreateTexture()
{
	MeshPX Sphere = MeshBuilder::CreateSpherePX(30, 30, 1.0f);
	mMesheBuffer.Initialize(Sphere);
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
	mDiffuseTexture.Initialize(L"../../Assets/Textures/earth.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	LoadDebug();
}

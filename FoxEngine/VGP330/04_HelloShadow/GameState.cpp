#include "GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

namespace
{
	bool bRotateChar = false;
}

void GameState::Initialize()
{
	//Initialize camera
	mCamera.SetPosition(FoxMath::Vector3(0.f, 3.f, -3.f));//Offset back in z
	mCamera.SetLookAt(FoxMath::Vector3(0.f));

	//Lights default value
	mDirectionalLight.direction = FoxMath::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mDirectionalLight.diffuse = { 0.4f, 0.4f, 0.4f, 1.0f };
	mDirectionalLight.specular = {0.9f, 0.9f, 0.9f, 1.0f};

	//Standard - Camera - light
	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	//Shadow initialization - Order does not matter here
	mShadowEffect.Initialize();
	mShadowEffect.SetdirectionalLight(mDirectionalLight);

	//Find char model and load
	ModelId modelId = ModelManager::Get()->LoadModel("../../Assets/Models/Character/AlienMan.model");
	mAlien = CreateRenderGroup(modelId);

	Mesh groundMesh = MeshBuilder::CreateGroundPlane(20,20,1.0f);
	mGround.meshBuffer.Initialize(groundMesh);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"misc/Ground.jpg");
	mGround.normalMapId = TextureManager::Get()->LoadTexture(L"misc/NormalMap.jpg");
	mGround.bumpMapId = TextureManager::Get()->LoadTexture(L"misc/DisplacementMap.jpg");
	mGround.specMapId = TextureManager::Get()->LoadTexture(L"misc/SpecularMap.jpg");

	mGround.material.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mGround.material.diffuse = { 0.3f, 0.3f, 0.3f, 1.0f };
	mGround.material.specular = { 0.3f, 0.3f, 0.3f, 1.0f };
	mGround.material.power = 20.0f;
}

void GameState::Terminate()
{
	CleanupRenderGroup(mAlien);
	mGround.Terminate();
	mShadowEffect.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Render()
{
	mShadowEffect.Begin();
		DrawrenderGroup(mShadowEffect, mAlien);
	mShadowEffect.End();

	//Render object
	mStandardEffect.Begin();
		DrawrenderGroup(mStandardEffect, mAlien);
		mStandardEffect.Render(mGround);
	mStandardEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Draw", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::PushID("FPS");
		if (ImGui::CollapsingHeader("FPS"))
		{
			ImGui::Text("fps: %.2f", mFPS);
		}
		ImGui::PopID();
		//Light
		ImGui::PushID("Light");
		if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::DragFloat3("Directional Light##", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f))
			{
				mDirectionalLight.direction = FoxMath::Normalize(mDirectionalLight.direction);
			}

			ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
			ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
			ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
		}
		ImGui::PopID();

		mStandardEffect.DebugUI();
		mShadowEffect.DebugUI();
	ImGui::End();
}

void GameState::Update(float deltaTime)
{
	//Controller
	EngineCameraController(deltaTime);
	//FPS
	EngineFPS(deltaTime);

	//model move
	if (Input::InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
	{
		bRotateChar = !bRotateChar;
	}
	if (bRotateChar)
	{
		for (auto& a : mAlien)
		{
			a.transform.vrotation.y += 1 * deltaTime;
		}
	}

	//Shadow Update
	mShadowEffect.SetFocus({mCamera.GetPosition().x, 0.0f, mCamera.GetPosition().z});
}

void GameState::EngineCameraController(float deltaTime)
{
	const InputSystem* input = Input::InputSystem::Get();
	const int moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10 : 1;
	const float displacement = 0.01f * moveSpeed;

	//Foward and Backward
	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(displacement);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-displacement);
	}
	//Right and Left
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(displacement);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-displacement);
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
		mCamera.Rise(-displacement);
	}
	else if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(displacement);
	}
}

void GameState::EngineFPS(float deltaTime)
{
	//FPS
	//mFPS = 1.0f / deltaTime;
	mFrameCount++;
	mTimePassed += deltaTime;
	if (mTimePassed >= 1.0f)
	{
		mFPS = mFrameCount;
		mTimePassed -= 1.0f;
		mFrameCount = 0.0f;
	}
}

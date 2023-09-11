#include "GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

namespace
{
	bool bRotateChar = false;
	Vector3 spaceshipRotation;
	Material mMaterial;

	float totalTime = 0.0f;
	Vector3 spaceShipPos(0.0f);
	bool bToggleTranslationOnAndOff = false;
}

void GameState::Initialize()
{
	//Initialize camera
	mCamera.SetPosition(FoxMath::Vector3(0.f, 3.f, -7.f));//Offset back in z
	mCamera.SetLookAt(FoxMath::Vector3(0.f));

	//Lights default value
	mDirectionalLight.direction = FoxMath::Normalize({ .5f, -1.f, .1f });
	mDirectionalLight.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mDirectionalLight.diffuse = { 0.4f, 0.4f, 0.4f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

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

	mSimpleEffect.Initialize();
	mSimpleEffect.SetCamera(mCamera);

	//POST PRO
	mPostProcessingEffect.Initialize(L"../../Assets/Shaders/PostProcessing.fx");
	mPostProcessingEffect.SetTexture(&mRenderTarget);
	mPostProcessingEffect.SetTexture(&mCombineTexture, 1);

	mCombineTexture.Initialize(L"../../Assets/Images/samples/happy.jpg");

	auto gs = GraphicsSystem::Get();
	const auto screenWidth = gs->GetBackBufferWidth();
	const auto screenHeiht = gs->GetBackBufferHeight();

	mRenderTarget.Initialize(screenWidth, screenHeiht, RenderTarget::Format::RGBA_U8);

	//Find char model and load
	ModelId modelId = ModelManager::Get()->LoadModel("../../Assets/Models/Character/source.model");
	mSpaceShip = CreateRenderGroup(modelId);

	for (auto& a : mSpaceShip)
	{
		a.transform.scale = Vector3(1);
	}

	for (auto& a : mSpaceShip)
	{
		a.transform.vrotation.x = 1.7f;
	}
	//Spaceship pos
	for (auto& a : mSpaceShip)
	{
		a.transform.position = Vector3(90.0f, 14.0f, 70.0f);
	}
	spaceShipPos.x = 90.0f;
	spaceShipPos.z = 70.0f;

	mCamera.SetPosition(FoxMath::Vector3(90.0f, 24.0f, 55.0f));

	mTerrain.Initialize("../../Assets/Textures/terrain/heightmap_512x512.raw", 15.0f);
	mAlienGround.meshBuffer.Initialize(mTerrain.mesh);
	mAlienGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"terrain/AlienTerrain/Alien_Rock_01_basecolor.png");
	mAlienGround.specMapId = TextureManager::Get()->LoadTexture(L"terrain/AlienTerrain/Alien_Rock_01_height.png");
	mAlienGround.normalMapId = TextureManager::Get()->LoadTexture(L"terrain/AlienTerrain/Alien_Rock_01_normal.png");
	mAlienGround.material.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mAlienGround.material.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mAlienGround.material.specular = { 0.8f, 0.8f, 0.8f, 1.0f };
	mAlienGround.material.power = 20.0f;


	mTerrainEffect.Initialize();
	mTerrainEffect.SetCamera(mCamera);
	mTerrainEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mTerrainEffect.SetDirectionalLight(mDirectionalLight);
	mTerrainEffect.SetShadowMap(mShadowEffect.GetDepthMap());

	MeshPX screenMesh = MeshBuilder::CreateScreenQuad();
	mScreenQuad.meshBuffer.Initialize(screenMesh);

	MeshPX sphere = MeshBuilder::CreateSkySpherePX(128, 128, 450.0f);
	mSkySphere.meshBuffer.Initialize(sphere);
	mSkySphere.diffuseMapId = TextureManager::Get()->LoadTexture(L"Space02.jpg");
}

void GameState::Terminate()
{
	mSkySphere.Terminate();
	mTerrainEffect.Terminate();
	mAlienGround.Terminate();
	mCombineTexture.Terminate();
	CleanupRenderGroup(mSpaceShip);
	mRenderTarget.Terminate();
	mPostProcessingEffect.Terminate();
	mShadowEffect.Terminate();
	mSimpleEffect.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Render()
{
	mShadowEffect.Begin();
		DrawrenderGroup(mShadowEffect, mSpaceShip);
	mShadowEffect.End();

	mTerrainEffect.Begin();
		mTerrainEffect.Render(mAlienGround);
	mTerrainEffect.End();

	//Render object
	mRenderTarget.BeginRender();
		mStandardEffect.Begin();
			DrawrenderGroup(mStandardEffect, mSpaceShip);
			mStandardEffect.Render(mAlienGround);
		mStandardEffect.End();
		mSimpleEffect.Begin();
			mSimpleEffect.Render(mSkySphere);
		mSimpleEffect.End();
	mRenderTarget.EndRender();

	mPostProcessingEffect.Beging();
	mPostProcessingEffect.Render(mScreenQuad);
	mPostProcessingEffect.End();

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
	//Model
	ImGui::PushID("Model");
	if (ImGui::CollapsingHeader("Model"))
	{
		if (ImGui::DragFloat3("Model Rotation##", &spaceshipRotation.x, 0.1f))
		{
			//Rotarion
			for (auto& s : mSpaceShip)
			{
				s.transform.vrotation = spaceshipRotation;
			}
		}

		ImGui::Checkbox("Translate", &bToggleTranslationOnAndOff);

		if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.r);
			ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.r);
			ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.r);
			ImGui::ColorEdit4("Emissive##Material", &mMaterial.emissive.r);
			ImGui::DragFloat("Power##Material", &mMaterial.power);

			for (auto& s : mSpaceShip)
			{
				s.material = mMaterial;
			}
		}
	}
	ImGui::PopID();
	//Ground mat
	ImGui::PushID("Ground");
	if (ImGui::CollapsingHeader("Ground"))
	{
		ImGui::ColorEdit4("Ambient##Material", &mAlienGround.material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mAlienGround.material.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mAlienGround.material.specular.r);
		ImGui::ColorEdit4("Emissive##Material", &mAlienGround.material.emissive.r);
		ImGui::DragFloat("Power##Material", &mAlienGround.material.power);
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
	mPostProcessingEffect.DebugUI();
	mShadowEffect.DebugUI();
	mTerrainEffect.DebugUI();
	ImGui::End();
}

void GameState::Update(float deltaTime)
{
	//Controller
	EngineCameraController(deltaTime);
	//FPS
	EngineFPS(deltaTime);

	if (Input::InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
	{
		bRotateChar = !bRotateChar;
	}

	ModelTransform(deltaTime);
	//Shadow Update
	mShadowEffect.SetFocus({ mCamera.GetPosition().x, 0.0f, mCamera.GetPosition().z });

	mSkySphere.transform.vrotation.y += Constants::HalfPi * deltaTime * .002f;
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

void GameState::ModelTransform(float deltaTime)
{
	if (bRotateChar)
	{
		for (auto& a : mSpaceShip)
		{
			a.transform.vrotation.y += 1 * deltaTime;
		}
	}

	if (bToggleTranslationOnAndOff)
	{
		totalTime += deltaTime;
		spaceShipPos.y = std::sin(totalTime) + 14.0f;

		for (auto& a : mSpaceShip)
		{
			a.transform.position.y = spaceShipPos.y;
		}
	}
}
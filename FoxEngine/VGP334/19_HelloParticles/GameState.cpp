#include "GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

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

	Mesh groundMesh = MeshBuilder::CreateGroundPlane(20,20,1.0f);
	mGround.meshBuffer.Initialize(groundMesh);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"misc/concrete.jpg");
	mGround.material.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mGround.material.diffuse = { 0.3f, 0.3f, 0.3f, 1.0f };
	mGround.material.specular = { 0.3f, 0.3f, 0.3f, 1.0f };
	mGround.material.power = 20.0f;

	mGroundShape.InitializeHull({10.0f, .5f, 10.0f}, { 0.0f, -.5f, 0.0f });
	mGroundRigidBody.Initialize(mGround.transform, mGroundShape, 0.0f);

	//Particle Stuff

	mParticleEffect.Initialize();
	mParticleEffect.SetCamera(mCamera);

	//initialize the particle system

	ParticleSystemInfo info;
	info.lifeTime = FLT_MAX;
	info.maxParticles = 100;
	info.spawnPosition = { 0.0f, 2.0f, 0.0f };
	info.spawnDirection = { 0.0f, 1.0f, 0.0f };
	info.spawnDelay = 0.0f;
	info.minParticlesPerEmit = 1.0f;
	info.maxParticlesPerEmit = 3.0f;
	info.minTimeBetweenParticles = 0.1f;
	info.maxTimeBetweenParticles = 0.3f;
	info.minSpawnAngle = -FoxMath::Constants::HalfPi * 0.25f;
	info.maxSpawnAngle = FoxMath::Constants::HalfPi * 0.25f;
	info.minSpeed = 2.0f;
	info.maxSpeed = 7.0f;

	//info for the particles

	info.particleInfo.lifetime = 2.0f;
	info.particleInfo.colorStart = Colors::LightBlue;
	info.particleInfo.colorEnd = Colors::Green;
	info.particleInfo.colorEnd.a = 0.1f;
	info.particleInfo.scaleStart = Vector3::One;
	info.particleInfo.scaleEnd = {0.1f, 0.1f, 0.1f};

	mParticleSystem.Initialize(info);
}

void GameState::Terminate()
{
	mParticleSystem.Terminate();
	mParticleEffect.Terminate();
	mGroundRigidBody.Terminate();
	mGroundShape.Terminate();
	mGround.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Render()
{
	//Render object
	mStandardEffect.Begin();
		mStandardEffect.Render(mGround);
	mStandardEffect.End();

	mParticleEffect.Begin();
		mParticleSystem.Render(mParticleEffect);
	mParticleEffect.End();

	//SimpleDraw::AddGroundPlane(20.0f, Colors::LightBlue);
	SimpleDraw::Render(mCamera);
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
		/*ImGui::PushID("Light");
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
		ImGui::PopID();*/

		mParticleSystem.DebugUI();
		mStandardEffect.DebugUI();
		Physics::PhysicsWorld::Get()->DebugUI();

	ImGui::End();
}

void GameState::Update(float deltaTime)
{
	//FPS
	EngineFPS(deltaTime);

	//Controller
	EngineCameraController(deltaTime);

	mParticleSystem.Update(deltaTime);
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
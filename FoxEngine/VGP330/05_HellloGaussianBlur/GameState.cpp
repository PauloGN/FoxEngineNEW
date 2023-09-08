#include "GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

namespace
{
	bool bRotateEarth = false;
}

void GameState::Initialize()
{
	//Initialize camera
	mCamera.SetPosition(FoxMath::Vector3(0.f, 1.f, -25.f));//Offset back in z
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

	//POST PRO
	mPostProcessingEffect.Initialize(L"../../Assets/Shaders/PostProcessing.fx");
	mPostProcessingEffect.SetTexture(&mBaseRenderTarget);
	mPostProcessingEffect.SetTexture(&mGaussianBlurEffect.GetResultTexture(), 1);

	//Gaussian Blur
	mGaussianBlurEffect.Initialize();
	mGaussianBlurEffect.SetBlurIterations(10);
	mGaussianBlurEffect.SetBlurSaturation(1.0f);
	mGaussianBlurEffect.SetSourceTexture(mBloomRenderTarget);

	auto gs = GraphicsSystem::Get();
	const auto screenWidth = gs->GetBackBufferWidth();
	const auto screenHeiht = gs->GetBackBufferHeight();

	mBaseRenderTarget.Initialize(screenWidth, screenHeiht, RenderTarget::Format::RGBA_U8);
	mBloomRenderTarget.Initialize(screenWidth, screenHeiht, RenderTarget::Format::RGBA_U8);

	MeshPX screenMesh = MeshBuilder::CreateScreenQuad();
	mScreenQuad.meshBuffer.Initialize(screenMesh);

	auto tm = TextureManager::Get();
	mSunRenderObj.diffuseMapId = tm->LoadTexture("sun.jpg");
	mSunRenderObj.meshBuffer.Initialize(MeshBuilder::CreateSphere(1024, 512, 5.0f));
	mSunRenderObj.material.emissive = Colors::Yellow;
	mSunRenderObj.material.power = 1.0f;

	mEarthRenderObj.diffuseMapId = tm->LoadTexture("earth.jpg");
	mEarthRenderObj.normalMapId = tm->LoadTexture("earth_normal.jpg");
	mEarthRenderObj.bumpMapId = tm->LoadTexture("earth_bump.jpg");
	mEarthRenderObj.specMapId = tm->LoadTexture("earth_spec.jpg");
	mEarthRenderObj.meshBuffer.Initialize(MeshBuilder::CreateSphere(1024, 512, 1.0f));
	mEarthRenderObj.material.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mEarthRenderObj.material.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mEarthRenderObj.material.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mEarthRenderObj.material.power = 10.0f;

	mEarthRenderObj.transform.position = {0.0f, 0.0f, -10.0f};
}

void GameState::Terminate()
{
	mEarthRenderObj.Terminate();
	mSunRenderObj.Terminate();
	mScreenQuad.Terminate();
	mBloomRenderTarget.Terminate();
	mBaseRenderTarget.Terminate();

	mPostProcessingEffect.Terminate();
	mGaussianBlurEffect.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Render()
{
	//Render object
	mBaseRenderTarget.BeginRender();
		mStandardEffect.Begin();
			mStandardEffect.Render(mSunRenderObj);
			mStandardEffect.Render(mEarthRenderObj);
		mStandardEffect.End();
		SimpleDraw::AddGroundPlane(30, Colors::Purple);
		SimpleDraw::Render(mCamera);
	mBaseRenderTarget.EndRender();

	mBloomRenderTarget.BeginRender();
		// swap the material on the earth
		Material dummyMaterial;
		dummyMaterial.power = 1.0f;
		std::swap(mEarthRenderObj.material, dummyMaterial);
			mStandardEffect.Begin();
				mStandardEffect.Render(mSunRenderObj);
				mStandardEffect.Render(mEarthRenderObj);
			mStandardEffect.End();
		// restore to the original
		std::swap(mEarthRenderObj.material, dummyMaterial);
	mBloomRenderTarget.EndRender();

	mGaussianBlurEffect.Begin();
		mGaussianBlurEffect.Render(mScreenQuad);
	mGaussianBlurEffect.End();

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

			ImGui::Spacing();
			ImGui::Text("Earth Orbit");
				ImGui::DragFloat("Offset", &mEarthOffset, 0.01f);
				ImGui::DragFloat("Rotation", &mEarthRotation, 0.01f);
				ImGui::DragFloat("Revolution", &mEarthRevolution, 0.01f);
			ImGui::Spacing();

		mStandardEffect.DebugUI();
		mPostProcessingEffect.DebugUI();
		mGaussianBlurEffect.DebugUi();
	ImGui::End();
}

void GameState::Update(float deltaTime)
{
	//Controller
	EngineCameraController(deltaTime);
	//FPS
	EngineFPS(deltaTime);
	//Earth controller
	if (Input::InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
	{
		bRotateEarth = !bRotateEarth;
	}
	if (bRotateEarth)
	{
		mEarthRenderObj.transform.vrotation.y += 1 * deltaTime;
	}
	//Earth orbit
	Vector3 earthPosition = {0.0f, 0.0f, -mEarthOffset};
	mEarthRenderObj.transform.position = TransformCoord(earthPosition, Matrix4::RotationY(mEarthRevolution));
	mEarthRenderObj.transform.rotation.y = mEarthRotation;
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
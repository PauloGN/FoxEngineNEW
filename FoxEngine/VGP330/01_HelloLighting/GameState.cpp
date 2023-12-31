#include "GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

void GameState::Initialize()
{
	//Initialize camera
	mCamera.SetPosition(FoxMath::Vector3(0.f, 1.f, -3.f));//Offset back in z
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

	//Initialize render object
	auto tm = TextureManager::Get();
	Mesh earth = MeshBuilder::CreateSphere(180, 180, 1.0f);
	mRenderObject.meshBuffer.Initialize(earth);
	mRenderObject.diffuseMapId = tm->LoadTexture("earth.jpg");
	mRenderObject.normalMapId = tm->LoadTexture("earth_normal.jpg");
	mRenderObject.bumpMapId = tm->LoadTexture("earth_bump.jpg");
	mRenderObject.specMapId = tm->LoadTexture("earth_spec.jpg");
	//mRenderObject.material.ambient = FoxEngine::Colors::Red;
	//mRenderObject.material.diffuse = FoxEngine::Colors::Red;
	//mRenderObject.material.specular = FoxEngine::Colors::Red;
	//mRenderObject.material.emissive = FoxEngine::Colors::Blue;

	const uint32_t size = 512;
	mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U8);

	//Standard cloud - Camera - light
	std::filesystem::path shaderFileCl = L"../../Assets/Shaders/DoTexturing.fx";
	mCloudEffect.Initialize(shaderFileCl);
	mCloudEffect.SetCamera(mCamera);
	mCloudEffect.SetDirectionalLight(DirectionalLight());
	//Clouding 
	//Initialize Cloud
	Mesh clouds2 = MeshBuilder::CreateSphere(190, 190, 1.04f);
	mCloudObject2.meshBuffer.Initialize(clouds2);
	mCloudObject2.diffuseMapId = tm->LoadTexture("earth_clouds.jpg");

	mBlendState.Initialize(FoxEngine::Graphics::BlendState::Mode::Additive);
}

void GameState::Terminate()
{
	mBlendState.Terminate();
	mRenderTarget.Terminate();
	mRenderObject.Terminate();
	mCloudObject2.Terminate();
	mCloudEffect.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Render()
{
	mBlendState.Set();

	//Render target
	mCamera.SetAspectRatio(1.0f);//Ignore screen and take a squared apect ratio
	mRenderTarget.BeginRender();
		mStandardEffect.Begin();
			mStandardEffect.Render(mRenderObject);
			mCloudEffect.Render(mCloudObject2);
		mStandardEffect.End();
	mRenderTarget.EndRender();
	mCamera.SetAspectRatio(0.0f);

	////Render object
	mStandardEffect.Begin();
		mStandardEffect.Render(mRenderObject);
		mCloudEffect.Render(mCloudObject2);
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
		ImGui::PushID("Material");
		if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::ColorEdit4("Ambient##Material", &mRenderObject.material.ambient.r);
			ImGui::ColorEdit4("Diffuse##Material", &mRenderObject.material.diffuse.r);
			ImGui::ColorEdit4("Specular##Material", &mRenderObject.material.specular.r);
			ImGui::ColorEdit4("Emissive##Material", &mRenderObject.material.emissive.r);
			ImGui::DragFloat("Power##Material", &mRenderObject.material.power);
		}
		ImGui::PopID();

		ImGui::PushID("Transform");
		if (ImGui::CollapsingHeader("Earth Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat3("Position", &mRenderObject.transform.position.x, 0.02f, -10.0f, 10.0f);
			ImGui::DragFloat3("Rotation", &mRenderObject.transform.vrotation.x, 0.02f, -10.0f, 10.0f);
		}
		ImGui::PopID();

		ImGui::Text("Render Target");
		ImGui::Image(
			mRenderTarget.GetRawData(),
			{ 128, 128 },
			{ 0, 0 },
			{ 1, 1 },
			{ 1, 1, 1, 1 },
			{ 1, 1, 1, 1 }
		);

		mStandardEffect.DebugUI();
	ImGui::End();
}

void GameState::Update(float deltaTime)
{
	//Controller
	EngineCameraController(deltaTime);
	//FPS
	EngineFPS(deltaTime);

	//Earth rotation
	mRenderObject.transform.vrotation.y += deltaTime * 0.012;
	mCloudObject2.transform.position = mRenderObject.transform.position;
	mCloudObject2.transform.vrotation.y += deltaTime * 0.025;
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

	mFPS = 1.0f / deltaTime;
	const float currentTime = FoxEngine::Core::TimeUtil::GetTime();

}

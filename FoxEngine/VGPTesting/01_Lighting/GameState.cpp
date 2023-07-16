#include "GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

void GameState::Initialize()
{

	//Initialize the camera
	mCamera.SetPosition(FoxMath::Vector3(0.f, 1.f, -3.f));//Offset back in z
	mCamera.SetLookAt(FoxMath::Vector3(0.f));

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);

	//Sky
	mSimpleEffectSky.Initialize();
	mSimpleEffectSky.SetCamera(mCamera);

	//Initialize render object
	Mesh earth = MeshBuilder::CreateSphere(30, 30, 1.0f);
	MeshPX sky = MeshBuilder::CreateSkySpherePX(100, 100, 350);
	mRenderObject.meshBuffer.Initialize(earth);
	mRenderObject.diffuseMapId = TextureManager::Get()->LoadTexture("earth.jpg");
	mRenderObject.transform.position = Vector3(-2.0f, 2.0f, 3.0f);
	mRenderSky.meshBuffer.Initialize(sky);
	mRenderSky.diffuseMapId = TextureManager::Get()->LoadTexture("space.jpg");

	//Light
	mSphereMeshBuffer.Initialize(earth);
	mSheperePS.Initialize(L"../../Assets/Shaders/Standard.fx");
	mSphereVS.Initialize<Mesh::VertexType>(L"../../Assets/Shaders/Standard.fx");

	mSphereSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mSphereTexture.Initialize(L"../../Assets/Textures/earth.jpg");

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

}
void GameState::Terminate()
{
	//Light
	mSphereSampler.Terminate();
	mSphereTexture.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();

	mRenderSky.Terminate();
	mRenderObject.Terminate();
	mSimpleEffectSky.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Render()
{
	//Light
	mTransformBuffer.BindVS(0);
	mSphereVS.Bind();
	mSheperePS.Bind();

	Matrix4 matWorld = Matrix4::Translation(Vector3(0, 0, 1));
	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();

	TransformData transformData;
	transformData.wvp = Transpose(matWorld* matView*matProj);
	transformData.world = matWorld;
	transformData.viewPosition = mCamera.GetPosition();

	mSphereSampler.BindPS(0);
	mSphereTexture.BindPS(0);

	mLightBuffer.BindPS(1);
	mLightBuffer.BindVS(1);
	mMaterialBuffer.BindPS(2);
	mMaterialBuffer.BindVS(2);

	mLightBuffer.Update(mDirectionalLight);
	mMaterialBuffer.Update(mMaterial);
	mTransformBuffer.Update(transformData);

	mSphereMeshBuffer.Render();

	mStandardEffect.Begin();
	mStandardEffect.Render(mRenderObject);
	mStandardEffect.End();

	mSimpleEffectSky.Begin();
	mSimpleEffectSky.Render(mRenderSky);
	mSimpleEffectSky.End();
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

		ImGui::PushID("Light");
		if (ImGui::CollapsingHeader("Light"))
		{
			ImGui::DragFloat("DirectionX", &mDirectionalLight.direction.x);
			ImGui::DragFloat("DirectionY", &mDirectionalLight.direction.y);
			ImGui::DragFloat("DirectionZ", &mDirectionalLight.direction.z);
			ImGui::Spacing();
			ImGui::ColorEdit4("Ambient", &mDirectionalLight.ambient.r);
			ImGui::ColorEdit4("Diffuse", &mDirectionalLight.diffuse.r);
			ImGui::ColorEdit4("Specular", &mDirectionalLight.specular.r);
		}
		ImGui::PopID();

		ImGui::PushID("Material");
		if (ImGui::CollapsingHeader("Material"))
		{
			ImGui::ColorEdit4("Ambient", &mMaterial.ambient.r);
			ImGui::ColorEdit4("Diffuse", &mMaterial.diffuse.r);
			ImGui::ColorEdit4("Specular", &mMaterial.specular.r);
			ImGui::DragFloat("Power", &mMaterial.power);
		}
		ImGui::PopID();

		mStandardEffect.DebugUI();

	ImGui::End();
}

Vector3 Line(5, 5, 0);

void GameState::Update(float deltaTime)
{
	//Light

	EngineCameraController(deltaTime);
	//FPS
	EngineFPS(deltaTime);
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

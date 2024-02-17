#include "GameState.h"
#include "Character.h"

using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

namespace
{
	bool bPlayAnimTrigger = true;
	bool bCamZomm = false;
	std::unique_ptr<ThirdPersonCamera> thCam;
}

void GameState::Initialize()
{
	//Initialize camera
	mCamera.SetPosition(FoxMath::Vector3(0.f, 5.f, -10.f));//Offset back in z
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

	//Find char model and load
	mCharacterId = ModelManager::Get()->LoadModel("../../Assets/Models/Character/AlienMan.model");
	ModelManager::Get()->AddAnimation(mCharacterId, "../../Assets/Models/Character/Idle.animset");
	ModelManager::Get()->AddAnimation(mCharacterId, "../../Assets/Models/Character/Walking.animset");
	mCharacterAnimator.Initialize(mCharacterId);
	mAlien = CreateRenderGroup(mCharacterId, &mCharacterAnimator);

	thCam = std::make_unique<ThirdPersonCamera>(mCamera, mAlien);

	Mesh groundMesh = MeshBuilder::CreateGroundPlane(20,20,1.0f);
	mGround.meshBuffer.Initialize(groundMesh);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"misc/concrete.jpg");
	mGround.material.ambient = { 0.6f, 0.6f, 0.6f, 1.0f };
	mGround.material.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mGround.material.specular = { 0.6f, 0.6f, 0.6f, 1.0f };
	mGround.material.power = 20.0f;
}

void GameState::Terminate()
{
	CleanupRenderGroup(mAlien);
	mGround.Terminate();
	thCam.reset();
	mStandardEffect.Terminate();
}

void GameState::Render()
{
	//Render object
	mStandardEffect.Begin();
		if (mDrawSkeleton)
		{
			AnimationUtil::BoneTransforms boneTransforms;
			AnimationUtil::ComputeBoneTransform(mCharacterId, boneTransforms, &mCharacterAnimator);
			AnimationUtil::DrawSkeleton(mCharacterId, boneTransforms);
		}
		else
		{
			DrawrenderGroup(mStandardEffect, mAlien);
		}
		mStandardEffect.Render(mGround);
	mStandardEffect.End();

	//SimpleDraw::AddGroundPlane(20.0f, Colors::LightBlue);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Draw", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		//FPS
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
		//Quaternion
#pragma region Quaternion controller
		ImGui::PushID("Rotation");
		ImGui::Spacing();
		ImGui::Text("Quaternion Rotation");
		if (ImGui::CollapsingHeader("Euler Angles", ImGuiTreeNodeFlags_CollapsingHeader))
		{
		bool updateCharacter = false;
			if (ImGui::DragFloat("Yaw", &mYaw, 0.01f))
			{
				updateCharacter = true;
			}
		
			if (ImGui::DragFloat("Pitch", &mPitch, 0.01f))
			{
				updateCharacter = true;
			}

			if (ImGui::DragFloat("Roll", &mRoll, 0.01f))
			{
				updateCharacter = true;
			}

			if (updateCharacter)
			{
				for (auto& a : mAlien)
				{
					a.transform.rotation = Quaternion::CreateFromYawPitchRoll(mYaw, mPitch, mRoll);
				}
			}
		}
		ImGui::PopID();
#pragma endregion
		//Skeleton controller
		ImGui::PushID("Skeleton");
		ImGui::Checkbox("DrawSkeleton", &mDrawSkeleton);
			ImGui::DragFloat("Animation Scale", &mAnimationScale, 0.1f, 0.0f, 1.0f);
			if (ImGui::DragInt("Play Animation", &mAnimationIndex, 1, -1, mCharacterAnimator.GetAnimationsCount() - 1))
			{
				mCharacterAnimator.PlayAnimation(mAnimationIndex, true);
			}
		ImGui::PopID();
		//Other Debugs
		mStandardEffect.DebugUI();
	ImGui::End();
}

void GameState::Update(float deltaTime)
{
	//Input system
	auto input = Input::InputSystem::Get();

	//FPS
	EngineFPS(deltaTime);

#pragma region Character TPC
	//Character controller
	mCharacterAnimator.Update(deltaTime * mAnimationScale);
	thCam->Update(input->GetMouseMoveX(), -input->GetMouseMoveY(), deltaTime);
	thCam->Rotate(-mCamera.GetDirection());
	//thCam->Move(Vector3(10.0f, 0.0f, 10.0f), deltaTime, .2f);
	thCam->LateUpdate(bCamZomm, 0.02f);
	CharacterAnimationControler(deltaTime);
#pragma endregion

#pragma region Testing Functions
	//FoxMath::Vector3 cameraDir = mCamera.GetDirection();
	//bool bRotate = mCamera.GetDirection().x != 0 || mCamera.GetDirection().y != 0 || mCamera.GetDirection().z != 0;
	//RotateAlongCamera(mAlien, bRotate, cameraDir);

	//FoxMath::Vector3 cameraPos = mCamera.GetPosition();
	//bool bMove = mCamera.GetPosition().x != 0 || mCamera.GetPosition().y != 0 || mCamera.GetPosition().z != 0;
	//MoveAlongCamera(mAlien, bMove, cameraPos, deltaTime, 50);
#pragma endregion

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
void GameState::CharacterAnimationControler(float deltaTime)
{
	const InputSystem* input = Input::InputSystem::Get();
	//Foward and Backward
	if (input->IsKeyDown(KeyCode::W))
	{
		if (bPlayAnimTrigger)
		{
			mAnimationIndex = 2;
			bPlayAnimTrigger = false;
			mCharacterAnimator.PlayAnimation(mAnimationIndex, true);
		}

		// Get the forward vector from the camera's orientation
		FoxMath::Vector3 cameraForward = mCamera.GetDirection();

		// Calculate the new target position based on camera forward vector
		FoxMath::Vector3 newTarget = mAlien[0].transform.position + cameraForward * moveSpeed * deltaTime;
		newTarget.y = 0;
		
		// Move the character forward
		thCam->Move(newTarget, deltaTime, moveSpeed);

	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Zoom(10);
	}
	//Right and Left
	if (input->IsKeyDown(KeyCode::D))
	{

	}
	else if (input->IsKeyDown(KeyCode::A))
	{

	}
	else if (false)
	{
		mAnimationIndex = 0;
		bPlayAnimTrigger = true;
		mCharacterAnimator.PlayAnimation(mAnimationIndex, true);
	}

	//Zoom
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		bCamZomm = true;
	}
	else if (!input->IsMouseDown(MouseButton::RBUTTON))
	{
		bCamZomm = false;
	}

}
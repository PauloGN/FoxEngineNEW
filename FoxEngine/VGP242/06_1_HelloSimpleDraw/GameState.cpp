#include "GameState.h"


void GameState::Initialize()
{
	mCamera.SetPosition({0.0f, 0.0f, -5.0f});
	mCamera.SetLookAt(Vector3::Zero);
}

void GameState::Terminate()
{
	
}

void GameState::Update(float deltaTime)
{
	EngineCameraController(deltaTime);


	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::SPACE))
	{
		eShape = (Shape)((int)eShape + 1 % (int)Shape::S_MAX);
	}
}

void GameState::Render()
{
	switch (eShape)
	{
	case Shape::S_AABB:
		{
			SimpleDraw::AddAABB(1, 1, 0, 2,2,2, Colors::Green );
		}
		break;
	case Shape::S_Sphere:
		{
			SimpleDraw::AddSphere(30, 30, 2, Vector3::Zero, Colors::Aqua);
		}
		break;
	case Shape::S_Transform:
		{
			SimpleDraw::AddTransform(Matrix4::Identity);
		}
		break;
	case Shape::S_AABBFilled:
		{
			SimpleDraw::AddFilledAABB(2, 2, 0, 4, 4, 4, Colors::BlanchedAlmond);
		}
		break;
	case Shape::S_Custom:
		{
			SimpleDraw::AddTransform(Matrix4::Identity);
			SimpleDraw::AddSphere(30, 30, .5f, Vector3::Zero, Colors::Red);
		}
		break;
	case Shape::S_MAX:
			eShape = Shape::S_Transform;
		break;
	default:
		{
			eShape = Shape::S_Transform;
		}
	}
	
	SimpleDraw::Render(mCamera);
}

void GameState::EngineCameraController(float deltaTime)
{
	const auto input = Input::InputSystem::Get();
	const int moveSpeed = input->IsKeyDown(Input::KeyCode::LSHIFT) ? 10 : 1;
	const float camSpeed = deltaTime * moveSpeed;


	//Foward and Backward
	if (input->IsKeyDown(Input::KeyCode::W))
	{
		mCamera.Walk(camSpeed);
	}
	else if (input->IsKeyDown(Input::KeyCode::S))
	{
		mCamera.Walk(-camSpeed);
	}
	//Right and Left
	if (input->IsKeyDown(Input::KeyCode::D))
	{
		mCamera.Strafe(camSpeed);
	}
	else if (input->IsKeyDown(Input::KeyCode::A))
	{
		mCamera.Strafe(-camSpeed);
	}

	//Rotation
	if (input->IsMouseDown(Input::MouseButton::RBUTTON))
	{

		const int turnSpeedMultiplyer = input->IsKeyDown(Input::KeyCode::LSHIFT) ? 2 : 1;

		const float x = input->GetMouseMoveX() * deltaTime * turnSpeedMultiplyer;
		const float y = input->GetMouseMoveY() * deltaTime * turnSpeedMultiplyer;

		mCamera.Yaw(x);
		mCamera.Pitch(y);

	}

	//UP and Down

	if (input->IsKeyDown(Input::KeyCode::Q))
	{
		mCamera.Rise(-camSpeed);
	}
	else if (input->IsKeyDown(Input::KeyCode::E))
	{
		mCamera.Rise(camSpeed);
	}
}
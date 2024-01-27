#include "GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

namespace
{
	Vector3 lookAt;

}

void GameState::Initialize()
{
	mCamera.SetPosition({0.0f, 1.0f,0.0f});
	mCamera.SetLookAt( {0.f, 0.0f, 0.0f} );
}

void GameState::Terminate()
{
	
}

void GameState::Render()
{
	SimpleDraw::Render(mCamera);
	
}

void GameState::DebugUI()
{
	SimpleDraw::AddGroundPlane(20, Colors::Aqua);
}

void GameState::Update(float deltaTime)
{

	auto i = Input::InputSystem::Get();

	//X
	if(i->IsKeyDown(Input::KeyCode::Q))
	{
		lookAt.x += 0.1f;
	}
	if (i->IsKeyDown(Input::KeyCode::W))
	{
		lookAt.x -= 0.1f;
	}
	//Y
	if (i->IsKeyDown(Input::KeyCode::A))
	{
		lookAt.y += 0.1f;
	}
	if (i->IsKeyDown(Input::KeyCode::S))
	{
		lookAt.y -= 0.1f;
	}	if (i->IsKeyDown(Input::KeyCode::Z))
	//Z
	{
		lookAt.z += 0.1f;
	}
	if (i->IsKeyDown(Input::KeyCode::X))
	{
		lookAt.z -= 0.1f;
	}

	mCamera.SetLookAt(lookAt);
}
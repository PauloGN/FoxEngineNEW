#pragma once
#include "FoxEngine/Inc/FoxEngine.h"


using namespace FoxEngine;
using namespace FoxEngine::FoxMath;

class GameState : public AppState
{
public:
	virtual ~GameState() = default;

	void Initialize() override;
	void Terminate() override;
	void Render() override;
	void DebugUI() override{}
	void Update(const float deltaTime) override
	{
		if(GetAsyncKeyState(VK_UP))
		{
			MainApp().ChangeState("Square");
			Graphics::GraphicsSystem::Get()->SetClearColor(Colors::DarkRed);
		}

		if (GetAsyncKeyState(VK_DOWN))
		{
			MainApp().ChangeState("Triangle");
			Graphics::GraphicsSystem::Get()->SetClearColor(Colors::Black);
		}

		if (GetAsyncKeyState(VK_RIGHT))
		{
			MainApp().ChangeState("GameState");
			Graphics::GraphicsSystem::Get()->SetClearColor(Colors::BlanchedAlmond);
		}
	}

protected:

	virtual void CreateShape();

	struct Vertex
	{
		Vector3 position;
		Color color;
	};

	using Vertices = std::vector<Vertex>;
	Vertices mVertices;//It is needed a minimum of a 3 vertices to get a triangle

	//This is the data buffer passed to the GPU
	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11VertexShader* mVertexShader = nullptr;
	ID3D11InputLayout* mInputLayout = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;

};


class Triangle:public GameState
{
	void Update(const float deltaTime)override;
protected:
	void CreateShape() override;
};

class Square :public GameState
{
	void Update(const float deltaTime)override;
protected:
	void CreateShape() override;
};


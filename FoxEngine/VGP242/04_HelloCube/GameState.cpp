#include "GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;


namespace
{
	float gRotationY = 0.0f;
	float gRotationX = 0.0f;
	float totalTime = 0.0f;
	Vector3 position(0.0f);
	static bool bToggleTranslationOnAndOff = true;
}

void GameState::Initialize() 
{
	//NDC - Normalized Device Coordinate
	// create shapes
	CreateShape();

	//Initialize the camera
	mCamera.SetPosition(FoxMath::Vector3(0.f, 1.f, -3.f));//Offset back in z
	mCamera.SetLookAt(FoxMath::Vector3(0.f, 0.f, 0.f));

	mConstantBuffer.Initialize(sizeof(Matrix4));

	//*******
	mMeshBuffer.Initialize(mVertices);
	//*******

	//Create a vertex shader
	const std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";
	mVertexShader.Initialize<VertexPC>(shaderFile);

	//Create a pixel shader
	mPixelShader.Initialize(shaderFile);
}
void GameState::Terminate() 
{
	mVertices.clear();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
}
void GameState::Update(float deltaTime) 
{
	const auto input = InputSystem::Get();
	App& myApp = MainApp();

	//A state with a new shape
	if (input->IsKeyPressed(KeyCode::UP)) {

		GraphicsSystem::Get()->SetClearColor(Colors::Black);
		myApp.ChangeState("ColorCube");
	}
	else if (input->IsKeyPressed(KeyCode::DOWN)) {

		GraphicsSystem::Get()->SetClearColor(Colors::Blue);
		myApp.ChangeState("GrayCube");
	} else if(input->IsKeyPressed(KeyCode::LEFT))
	{
		GraphicsSystem::Get()->SetClearColor(Colors::Black);
	}

	//Rotation
	gRotationY += Constants::HalfPi * deltaTime * 0.5f;
	gRotationX += Constants::HalfPi * deltaTime * 0.2f;

	//Translation
	if(input->IsKeyPressed(KeyCode::SPACE))
	{
		bToggleTranslationOnAndOff = !bToggleTranslationOnAndOff;
	}

	if(bToggleTranslationOnAndOff)
	{
		totalTime += deltaTime;
		position.y = std::sin(totalTime);
	}
}
void GameState::Render() 
{

	mVertexShader.Bind();
	mPixelShader.Bind();
	
	//Use the Camera
	//Matrix4 matWorld = Matrix4::Identity; Normal
	Matrix4 matWorld = Matrix4::Translation(position) * Matrix4::RotationY(gRotationY);
	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();
	Matrix4 matFinal = matWorld * matView * matProj;

	//convert from left hand to right hand coordinated system
	Matrix4 wvp = Transpose(matFinal);

	mConstantBuffer.Update(&wvp);
	mConstantBuffer.BindVS(0);

	mMeshBuffer.Render();
}

//====================================	Assignment 04	==================================\\

/*
*Vertices[]
*indices[] indexes vertices

a vertex is position color and UV

float3, float4, float2 = 9 floats 4 bytes each, witch means 36 bytes per vertex 
by using this technique we can save memory

*/

void ColorCube::CreateShape()
{

	//Black screen

	//Front face
	mVertices.push_back({ Vector3(0.0f,0.0f,0.0f),Colors::Red});   //0
	mVertices.push_back({ Vector3(0.0f,0.5f,0.0f),Colors::Blue});  //1
	mVertices.push_back({ Vector3(0.5f,0.0f,0.0f),Colors::Blue});

	mVertices.push_back({ Vector3(0.0f,0.5f,0.0f),Colors::Blue}); //1
	mVertices.push_back({ Vector3(0.5f,0.5f,0.0f),Colors::Red});
	mVertices.push_back({ Vector3(0.5f,0.0f,0.0f),Colors::Blue});

	//Back face

	mVertices.push_back({ Vector3(0.0f,0.0f,0.5f),Colors::Blue});
	mVertices.push_back({ Vector3(0.5f,0.0f,0.5f),Colors::Green});
	mVertices.push_back({ Vector3(0.0f,0.5f,0.5f),Colors::Red});

	mVertices.push_back({ Vector3(0.0f,0.5f,0.5f),Colors::Red});
	mVertices.push_back({ Vector3(0.5f,0.0f,0.5f),Colors::Green});
	mVertices.push_back({ Vector3(0.5f,0.5f,0.5f),Colors::Blue});


	//top face

	mVertices.push_back({ Vector3(0.0f,0.5f,0.0f),Colors::Blue }); //1
	mVertices.push_back({ Vector3(0.0f,0.5f,0.5f),Colors::Red });
	mVertices.push_back({ Vector3(0.5f,0.5f,0.0f),Colors::Red });

	mVertices.push_back({ Vector3(0.0f,0.5f,0.5f),Colors::Red });
	mVertices.push_back({ Vector3(0.5f,0.5f,0.5f),Colors::Blue });
	mVertices.push_back({ Vector3(0.5f,0.5f,0.0f),Colors::Red });


	//Bottom face

	mVertices.push_back({ Vector3(0.0f,0.0f,0.0f),Colors::Blue }); // 0
	mVertices.push_back({ Vector3(0.5f,0.0f,0.0f),Colors::Red });
	mVertices.push_back({ Vector3(0.0f,0.0f,0.5f),Colors::Red });
										   
	mVertices.push_back({ Vector3(0.0f,0.0f,0.5f),Colors::Red });
	mVertices.push_back({ Vector3(0.5f,0.0f,0.0f),Colors::Red });
	mVertices.push_back({ Vector3(0.5f,0.0f,0.5f),Colors::Blue });


	//Right face

	mVertices.push_back({ Vector3(0.0f,0.0f,0.5f),Colors::Blue });
	mVertices.push_back({ Vector3(0.0f,0.5f,0.0f),Colors::Blue }); // 1
	mVertices.push_back({ Vector3(0.0f,0.0f,0.0f),Colors::Red });  // 0

	mVertices.push_back({ Vector3(0.0f,0.0f,0.5f),Colors::Blue });
	mVertices.push_back({ Vector3(0.0f,0.5f,0.5f),Colors::Red });
	mVertices.push_back({ Vector3(0.0f,0.5f,0.0f),Colors::Blue }); // 1


	//Left face

	mVertices.push_back({ Vector3(0.5f,0.0f,0.5f),Colors::Green });
	mVertices.push_back({ Vector3(0.5f,0.0f,0.0f),Colors::Blue });
	mVertices.push_back({ Vector3(0.5f,0.5f,0.0f),Colors::Red });

	mVertices.push_back({ Vector3(0.5f,0.0f,0.5f),Colors::Green });
	mVertices.push_back({ Vector3(0.5f,0.5f,0.0f),Colors::Red });
	mVertices.push_back({ Vector3(0.5f,0.5f,0.5f),Colors::Blue });

}

void GrayCube::CreateShape()
{

	//Blue Front

	mVertices.push_back({ Vector3(-0.5f,-0.5f,-0.5f),Colors::Gray });		// FBL
	mVertices.push_back({ Vector3(-0.5f,0.5f,-0.5f),Colors::Black });		// FTL
	mVertices.push_back({ Vector3(0.5f,-0.5f,-0.5f),Colors::Black });		// FBR

	mVertices.push_back({ Vector3(0.5f,-0.5f,-0.5f),Colors::Black });		// FBR
	mVertices.push_back({ Vector3(-0.5f, 0.5f,-0.5f),Colors::Black });		// FTL
	mVertices.push_back({ Vector3(0.5f,0.5f,-0.5f),Colors::Gray});			// FTR

	////Blue Back

	mVertices.push_back({ Vector3(-0.5f,-0.5f,0.5f),Colors::Gray });		//BBL
	mVertices.push_back({ Vector3(0.5f,-0.5f,0.5f),Colors::Black });		//BBR
	mVertices.push_back({ Vector3(-0.5f,0.5f,0.5f),Colors::Black });		//BTL

	mVertices.push_back({ Vector3(0.5f,-0.5f,0.5f),Colors::Black });		//BBR
	mVertices.push_back({ Vector3(0.5f,0.5f,0.5f),Colors::Gray });			//BTR
	mVertices.push_back({ Vector3(-0.5f,0.5f,0.5f),Colors::Black });		//BTL


	//Blue Right

	mVertices.push_back({ Vector3(0.5f,-0.5f,-0.5f),Colors::Gray });		// FBR
	mVertices.push_back({ Vector3(0.5f,0.5f,-0.5f),Colors::Black });		// FTR
	mVertices.push_back({ Vector3(0.5f,-0.5f,0.5f),Colors::Black });		// BBR
	
	mVertices.push_back({ Vector3(0.5f,-0.5f,0.5f),Colors::Black });		//BBR
	mVertices.push_back({ Vector3(0.5f,0.5f,-0.5f),Colors::Gray });			//FTR
	mVertices.push_back({ Vector3(0.5f,0.5f,0.5f),Colors::Black });			//BTR

	//Blue Left

	mVertices.push_back({ Vector3(-0.5f,-0.5f,-0.5f),Colors::Gray });		//FBL
	mVertices.push_back({ Vector3(-0.5f,-0.5f,0.5f),Colors::Black });		//BBL
	mVertices.push_back({ Vector3(-0.5f,0.5f,-0.5f),Colors::Black });		//FTL
									  
	mVertices.push_back({ Vector3(-0.5f,-0.5f,0.5f),Colors::Black });		//BBL
	mVertices.push_back({ Vector3(-0.5f,0.5f,0.5f),Colors::Black });		//BTL
	mVertices.push_back({ Vector3(-0.5f,0.5f,-0.5f),Colors::Gray });		//FTL


	//Blue top

	mVertices.push_back({ Vector3(-0.5f,0.5f,-0.5f),Colors::Gray });		//FTL
	mVertices.push_back({ Vector3(-0.5f,0.5f,0.5f),Colors::Black });		//BTL
	mVertices.push_back({ Vector3(0.5f,0.5f,-0.5f),Colors::Black });		//FTR

	mVertices.push_back({ Vector3(0.5f,0.5f,-0.5f),Colors::Black });		//FTR
	mVertices.push_back({ Vector3(-0.5f,0.5f,0.5f),Colors::Black });		//BTL
	mVertices.push_back({ Vector3(0.5f,0.5f,0.5f),Colors::Gray });			//BTR

	//Blue Bottom

	mVertices.push_back({ Vector3(-0.5f,-0.5f,-0.5f),Colors::Gray });		//FBL
	mVertices.push_back({ Vector3(0.5f,-0.5f,-0.5f),Colors::Black });		//FBR
	mVertices.push_back({ Vector3(-0.5f,-0.5f,0.5f),Colors::Black });		//BBL
																				
	mVertices.push_back({ Vector3(0.5f,-0.5f,-0.5f),Colors::Black });		//FBR
	mVertices.push_back({ Vector3(0.5f,-0.5f,0.5f),Colors::Gray });			//BBR
	mVertices.push_back({ Vector3(-0.5f,-0.5f,0.5f),Colors::Black });		//BBL
									  
}
#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace FoxEngine;
using namespace FoxEngine::Core;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;


void FoxEngine::App::ChangeState(const std::string& stateName)
{
	auto iter = mAppStates.find(stateName);

	if (iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}
}

void App::Run(const FoxEngine::AppConfig& config)
{

	//create window
	
	Window myWindow;
	myWindow.Initialize(
		GetModuleHandle(nullptr), 
		config.appName,
		config.winWidth,
		config.winHeight
	);


	ASSERT(myWindow.IsActive(), "Window creation has failed!");

	//initialize static classes
	auto handle = myWindow.GetWindowHandle();
	InputSystem::StaticInitialize(handle);
	GraphicsSystem::StaticInitialize(handle, false);
	DebugUI::StaticInitialize(handle, false, true);
	SimpleDraw::StaticInitialize(config.debugDrawLimit);


	//States ***** Week 3
	ASSERT(mCurrentState != nullptr, "App need an app state");
	mCurrentState->Initialize();

	mRunning = true;
	while (mRunning)
	{
		//Process all the inputs from the window
		myWindow.ProcessMessage();

		auto inputSystem = InputSystem::Get();

		inputSystem->Update();
		if (!myWindow.IsActive() || inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			break;
		}

		//if there is any new pending state change it and set it as a current state
		if (mNextState != nullptr)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}

		//run the game
		auto deltaTime = TimeUtil::GetDeltaTime();

		if (deltaTime < .5f)
		{
			mCurrentState->Update(deltaTime);
		}

		auto graphicsSystem = GraphicsSystem::Get();
		graphicsSystem->BeginRender();
			mCurrentState->Render();
			DebugUI::BeginRender();
			mCurrentState->DebugUI();
			DebugUI::EndRender();
		graphicsSystem->EndRender();
		
		//Building shapes
	}

	// terminate static classes
	mCurrentState->Terminate();

	SimpleDraw::StaticTerminate();
	DebugUI::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	InputSystem::StaticTerminate();
	myWindow.Terminate();
}

void FoxEngine::App::Quit()
{

	mRunning = false;

}

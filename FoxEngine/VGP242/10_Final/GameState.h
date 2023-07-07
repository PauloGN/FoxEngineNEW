#pragma once

#include <FoxEngine/Inc/FoxEngine.h>
#include "RenderObject.h"

using namespace FoxEngine;
using namespace FoxEngine::FoxMath;
using namespace FoxEngine::Graphics;

class GameState : public AppState
{
public:
    void Initialize();
    void Terminate();
    void Render();
    void Update(float deltaTime);
    void DebugUI();

    //Controllers
    void EngineCameraController(float deltaTime);

protected:

    void RenderMesh(const Camera& camera, bool useTransform);

    Camera mCamera;
    Camera mRenderTargetCamera;

    //Became simple Effect
	ConstantBuffer mConstantBuffer;
    VertexShader mVertexShader;
    PixelShader mPixelShader;
    Sampler mSampler;

	RenderTarget mRenderTarget;

    SkySphere mSkySphere;
    Planet mPlanets[9];
    Star mSun;

    bool mDrawCircleOrbits = true;

};

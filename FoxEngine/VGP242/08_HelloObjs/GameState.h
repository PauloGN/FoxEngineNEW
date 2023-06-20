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

protected:

    void RenderMesh(const Camera& camera, bool useTransform);

    Camera mCamera;
    Camera mRenderTargetCamera;
    ConstantBuffer mConstantBuffer;
    //MeshBuffer mMeshBuffer;
    VertexShader mVertexShader;
    PixelShader mPixelShader;
    RenderTarget mRenderTarget;

    SkySphere mSkySphere;
    Planet mPlanets;
    Planet mEarth2;

    //Texture mDiffuseTexture;
    Sampler mSampler;
    Matrix4 mWorldTrans;

    bool mDrawWorld = true;
    bool mDrawSphere1 = true;
    bool mDrawSphere2 = true;

    Color mSphere1Color = Colors::Pink;
    Color mSphere2Color = Colors::Aqua;

    Vector3 mSphere1Pos = { 0.0f, 0.0f, 0.0f };
    Vector3 mSphere2Pos = { 0.0f, 0.0f, 0.0f };
};

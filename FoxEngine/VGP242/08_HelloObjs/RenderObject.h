#pragma once
#include <FoxEngine/Inc/FoxEngine.h>

using namespace FoxEngine;
using namespace FoxEngine::FoxMath;
using namespace FoxEngine::Graphics;

class RenderObjectx
{
public:
    virtual void Initialize() {}
    virtual void Terminate() {}
    virtual void Render(const Camera& camera, ConstantBuffer& constantBuffer, bool useTransform) {}
    virtual void Update(float deltaTime) {}
    void SetPosition(const Vector3& pos) { mTransform._41 = pos.x; mTransform._42 = pos.y; mTransform._43 = pos.z; }

    virtual void DebugUI()
    {
        SimpleDraw::AddCircle(60, 10.0, Colors::AliceBlue);
    }

protected:
    MeshBuffer mMeshBuffer;
    Texture mDiffuseTexture;
    Matrix4 mTransform;
};

class Planet : public RenderObjectx
{
public:
    void Initialize() override;
    void Terminate()  override;
    void Render(const Camera& camera, ConstantBuffer& constantBuffer, bool useTransform) override;
    void Update(float deltaTime) override;
};

class SkySphere : public RenderObjectx
{
public:
    void Initialize() override;
    void Terminate()  override;
    void Render(const Camera& camera, ConstantBuffer& constantBuffer, bool useTransform) override;
    void Update(float deltaTime) override;
};
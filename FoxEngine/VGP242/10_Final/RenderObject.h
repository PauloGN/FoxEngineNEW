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
    const Vector3 GetPosition() const { return { mTransform._41, mTransform._42,  mTransform._43}; }

    virtual void DebugUI() {};

    Vector3 mPosition;

protected:

    MeshBuffer mMeshBuffer;
    Texture mDiffuseTexture;
    Matrix4 mTransform;
};

/// <summary>                      PLANET                                   </summary>

class Planet : public RenderObjectx
{
public:

    void SetPlanet(const std::filesystem::path& fileName, const float radius);

    void Initialize() override;
    void Terminate()  override;
    void Render(const Camera& camera, ConstantBuffer& constantBuffer, bool useTransform) override;
    void Update(float deltaTime) override;

    //Get and Set
    void SetRotationRate(const float rate) { mRotationRate = rate; }
    void SetTranslationRate(const float rate) { mTranslationRate = rate; }        
    const float GetRotationRate() const { return mRotationRate;}
    const float GetTranslationRate() const { return mTranslationRate; }

    void AddCircleOrbt(const bool addCircle);

private:

    float distanceFromTheSun = 0.0f;
    std::filesystem::path mFileName;
    float mRadius;
    float mRotationY = 0.0f;
    float mRotationRate = 0.5f;
    float mTranslationRate = 25.f;

};

/// <summary>                      STAR                                 </summary>

class Star : public RenderObjectx
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

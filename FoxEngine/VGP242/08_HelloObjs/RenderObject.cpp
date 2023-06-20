#include "RenderObject.h"

void Planet::Initialize()
{
    MeshPX sphere = MeshBuilder::CreateSpherePX(30, 30, 1.0f);
    mMeshBuffer.Initialize(sphere);

    mDiffuseTexture.Initialize(L"../../Assets/Textures/earth.jpg");

    mTransform = Matrix4::Identity;
}
void Planet::Terminate()
{
    mDiffuseTexture.Terminate();
    mMeshBuffer.Terminate();
}
void Planet::Render(const Camera& camera, ConstantBuffer& constantBuffer, bool useTransform)
{
    mDiffuseTexture.BindPS(0);

    Matrix4 matWorld = (useTransform) ? mTransform : Matrix4::Identity;
    Matrix4 matView = camera.GetViewMatrix();
    Matrix4 matProj = camera.GetProjectionMatrix();
    Matrix4 matFinal = matWorld * matView * matProj;
    Matrix4 wvp = Transpose(matFinal);

    constantBuffer.Update(&wvp);
    constantBuffer.BindVS(0);

    mMeshBuffer.Render();

    Texture::UnbindPS(0);
}
void Planet::Update(float deltaTime)
{

}

void SkySphere::Initialize()
{
    MeshPX skySphere = MeshBuilder::CreateSkySpherePX(100, 100, 100.0f);
    mMeshBuffer.Initialize(skySphere);

    mDiffuseTexture.Initialize(L"../../Assets/Images/skysphere/sunrise.jpg");

    mTransform = Matrix4::Identity;
}
void SkySphere::Terminate()
{
    mDiffuseTexture.Terminate();
    mMeshBuffer.Terminate();
}
void SkySphere::Render(const Camera& camera, ConstantBuffer& constantBuffer, bool useTransform)
{
    mDiffuseTexture.BindPS(0);

    Matrix4 matWorld = (useTransform) ? mTransform : Matrix4::Identity;
    Matrix4 matView = camera.GetViewMatrix();
    Matrix4 matProj = camera.GetProjectionMatrix();
    Matrix4 matFinal = matWorld * matView * matProj;
    Matrix4 wvp = Transpose(matFinal);

    constantBuffer.Update(&wvp);
    constantBuffer.BindVS(0);

    mMeshBuffer.Render();

    Texture::UnbindPS(0);
}
void SkySphere::Update(float deltaTime)
{

}
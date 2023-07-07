#include "RenderObject.h"

//////   Planet    \\\\\\

void Planet::SetPlanet(const std::filesystem::path& fileName, const float radius)
{
    mFileName = fileName;
    mRadius = radius;
    distanceFromTheSun = mPosition.x;
}

void Planet::Initialize()
{
    MeshPX sphere = MeshBuilder::CreateSpherePX(30, 30, mRadius);
    mMeshBuffer.Initialize(sphere);

    mDiffuseTexture.Initialize(mFileName);

    mTransform = Matrix4::Identity;
    distanceFromTheSun = FoxMath::Magnitude(mPosition);
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
    matWorld = Matrix4::RotationY(mRotationY) * mTransform;
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
    mRotationY += Constants::HalfPi * deltaTime * mRotationRate;
   //mTransform = Matrix4::RotationY(mRotationY) * mTransform;

    
    if (mPosition.x >= 0)
    {
        if (mPosition.z >= 0)
        {
           //angulo do planeta com relação ao sol
           float angle = atan(mPosition.z/mPosition.x);
           mPosition.x -= sin(angle) * deltaTime * mTranslationRate;
           mPosition.z += cos(angle) * deltaTime * mTranslationRate;
        }
        else
        {
            float angle = atan(-mPosition.z / mPosition.x);
            mPosition.x += sin(angle) * deltaTime * mTranslationRate;
            mPosition.z += cos(angle) * deltaTime * mTranslationRate;
        }
    }
    else//x < 0
    {
        if (mPosition.z >= 0)
        {
            //angulo do planeta com relação ao sol
            float angle = atan(-mPosition.z / mPosition.x);
            mPosition.x -= sin(angle) * deltaTime * mTranslationRate;
            mPosition.z -= cos(angle) * deltaTime * mTranslationRate;
        }
        else// z < 0
        {
            float angle = atan(mPosition.z / mPosition.x);
            mPosition.x += sin(angle) * deltaTime * mTranslationRate;
            mPosition.z -= cos(angle) * deltaTime * mTranslationRate;
        }

    }

    mPosition.x = Clamp(mPosition.x, -distanceFromTheSun, distanceFromTheSun);
    mPosition.z = Clamp(mPosition.z, -distanceFromTheSun,  distanceFromTheSun);

   SetPosition(mPosition);
  
}

void Planet::AddCircleOrbt(const bool addCircle)
{
   SimpleDraw::AddCircle(80, distanceFromTheSun, Colors::Aqua);
}


//////   SKY SPHERE  \\\\\\


void SkySphere::Initialize()
{
    MeshPX skySphere = MeshBuilder::CreateSkySpherePX(100, 100, 550.0f);
    mMeshBuffer.Initialize(skySphere);

    mDiffuseTexture.Initialize(L"../../Assets/Images/skysphere/milky_way.jpg");

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
    matWorld = Matrix4::RotationY(mRotationY) * mTransform;
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
    mRotationY += Constants::HalfPi * deltaTime * mRotationRate;
}

//////   Star    \\\\\\

void Star::Initialize()
{
    MeshPX sphere = MeshBuilder::CreateSpherePX(30, 30, 8.0f);
    mMeshBuffer.Initialize(sphere);

    mDiffuseTexture.Initialize(L"../../Assets/Images/planets/Sun.jpg");

    mTransform = Matrix4::Identity;
}

void Star::Terminate()
{
    mDiffuseTexture.Terminate();
    mMeshBuffer.Terminate();
}

void Star::Render(const Camera& camera, ConstantBuffer& constantBuffer, bool useTransform)
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

void Star::Update(float deltaTime)
{
}

#include "GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;
using namespace FoxEngine::Input;

namespace
{
    //Activate and deactivate render target
    bool gbSun = true;
    bool gbMercury;
    bool gbVenus;
    bool gbEarth;
    bool gbMars;
    bool gbJupiter;
    bool gbSaturn;
    bool gbUranus;
    bool gbNeptune;
    bool gbPluto;

    //Set index and offset
    int gPlanetIndex = 0;
    float gPlanetOffset = -1;
    std::string planetName = "";

    //Set Rotation and translation speed

    float gRotationSpeed = .5f;
    float gTranslationSpeed = 25.f;

    void RenderAndUpdateSelected(Planet* planets)
    {

        if (ImGui::Checkbox("Sun", &gbSun))
        {
          gbMercury = false;
          gbVenus = false;
          gbEarth = false;
          gbMars = false;
          gbJupiter = false;
          gbSaturn = false;
          gbUranus = false;
          gbNeptune = false;
          gbPluto = false;

        }

        if (!gbSun)
        {

            constexpr float drag = .5f;

            ImGui::Text(planetName.c_str());
            ImGui::DragFloat("Rotation rate", &gRotationSpeed, drag, 0.0f, 8.0f);
            ImGui::DragFloat("Translation rate", &gTranslationSpeed, drag, 0.0f, 200);
            planets[gPlanetIndex].SetRotationRate(gRotationSpeed);
            planets[gPlanetIndex].SetTranslationRate(gTranslationSpeed);
        }

        if (ImGui::Checkbox("Mercury", &gbMercury))
        {
            gbSun = false;
            gbVenus = false;
            gbEarth = false;
            gbMars = false;
            gbJupiter = false;
            gbSaturn = false;
            gbUranus = false;
            gbNeptune = false;
            gbPluto = false;

            if (gbMercury)
            {
                gPlanetIndex = 0;
                gPlanetOffset = -0.7;
                planetName = "Mercury";

                gRotationSpeed =    planets[gPlanetIndex].GetRotationRate();
                gTranslationSpeed = planets[gPlanetIndex].GetTranslationRate();
            }

        }

        if (ImGui::Checkbox("Venus", &gbVenus))
        {
            gbSun = false;
            gbMercury = false;
            gbEarth = false;
            gbMars = false;
            gbJupiter = false;
            gbSaturn = false;
            gbUranus = false;
            gbNeptune = false;
            gbPluto = false;

            if (gbVenus)
            {
                gPlanetIndex = 1;
                gPlanetOffset = -2;
                planetName = "Venus";


                gRotationSpeed = planets[gPlanetIndex].GetRotationRate();
                gTranslationSpeed = planets[gPlanetIndex].GetTranslationRate();
            }
        }

        if (ImGui::Checkbox("Earth", &gbEarth))
        {
            gbSun = false;
            gbMercury = false;
            gbVenus = false;
            gbMars = false;
            gbJupiter = false;
            gbSaturn = false;
            gbUranus = false;
            gbNeptune = false;
            gbPluto = false;

            if (gbEarth)
            {
                planetName = "Earth";
                gPlanetIndex = 2;
                gPlanetOffset = -2;

                gRotationSpeed = planets[gPlanetIndex].GetRotationRate();
                gTranslationSpeed = planets[gPlanetIndex].GetTranslationRate();
            }

        }

        if (ImGui::Checkbox("Mars", &gbMars))
        {
            gbSun = false;
            gbMercury = false;
            gbVenus = false;
            gbEarth = false;
            gbJupiter = false;
            gbSaturn = false;
            gbUranus = false;
            gbNeptune = false;
            gbPluto = false;

            if (gbMars)
            {
                planetName = "Mars";
                gPlanetIndex = 3;
                gPlanetOffset = -1.2f;

                gRotationSpeed = planets[gPlanetIndex].GetRotationRate();
                gTranslationSpeed = planets[gPlanetIndex].GetTranslationRate();
            }

        }

        if (ImGui::Checkbox("Jupiter", &gbJupiter))
        {
            gbSun = false;
            gbMercury = false;
            gbVenus = false;
            gbEarth = false;
            gbMars = false;
            gbSaturn = false;
            gbUranus = false;
            gbNeptune = false;
            gbPluto = false;

            if (gbJupiter)
            {
                planetName = "Jupiter";
                gPlanetIndex = 4;
                gPlanetOffset = -10.0f;

                gRotationSpeed = planets[gPlanetIndex].GetRotationRate();
                gTranslationSpeed = planets[gPlanetIndex].GetTranslationRate();
            }

        }

        if (ImGui::Checkbox("Saturn", &gbSaturn))
        {
            gbSun = false;
            gbMercury = false;
            gbVenus = false;
            gbEarth = false;
            gbMars = false;
            gbJupiter = false;
            gbUranus = false;
            gbNeptune = false;
            gbPluto = false;

            if (gbSaturn)
            {
                planetName = "Saturn";
                gPlanetIndex = 5;
                gPlanetOffset = -9.7f;

                gRotationSpeed = planets[gPlanetIndex].GetRotationRate();
                gTranslationSpeed = planets[gPlanetIndex].GetTranslationRate();
            }
        }


        if (ImGui::Checkbox("Uranus", &gbUranus))
        {
            gbSun = false;
            gbMercury = false;
            gbVenus = false;
            gbEarth = false;
            gbMars = false;
            gbJupiter = false;
            gbSaturn = false;
            gbNeptune = false;
            gbPluto = false;

            if (gbUranus)
            {
                planetName = "Uranus";
                gPlanetIndex = 6;
                gPlanetOffset = -4.2f;

                gRotationSpeed = planets[gPlanetIndex].GetRotationRate();
                gTranslationSpeed = planets[gPlanetIndex].GetTranslationRate();
            }

        }

        if (ImGui::Checkbox("Neptune", &gbNeptune))
        {
            gbSun = false;
            gbMercury = false;
            gbVenus = false;
            gbEarth = false;
            gbMars = false;
            gbJupiter = false;
            gbSaturn = false;
            gbUranus = false;
            gbPluto = false;

            if (gbNeptune)
            {
                planetName = "Neptune";
                gPlanetIndex = 7;
                gPlanetOffset = -1.7f;

                gRotationSpeed = planets[gPlanetIndex].GetRotationRate();
                gTranslationSpeed = planets[gPlanetIndex].GetTranslationRate();
            }
        }

        if (ImGui::Checkbox("Pluto", &gbPluto))
        {
            gbSun = false;
            gbMercury = false;
            gbVenus = false;
            gbEarth = false;
            gbMars = false;
            gbJupiter = false;
            gbSaturn = false;
            gbUranus = false;
            gbNeptune = false;

            if (gbPluto)
            {
                planetName = "Pluto";
                gPlanetIndex = 8;
                gPlanetOffset = -1.0f;

                gRotationSpeed = planets[gPlanetIndex].GetRotationRate();
                gTranslationSpeed = planets[gPlanetIndex].GetTranslationRate();
            }
        }

    }

}

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 5.0f, -45.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    //RENDER TARGET
    mRenderTargetCamera.SetPosition ({ 0.0f, 0.0f , -19 });
    mRenderTargetCamera.SetLookAt ({ 0.0f, 0.0f, 0.0f });
    mRenderTargetCamera.SetAspectRatio (1.0f);

    mConstantBuffer.Initialize(sizeof(Matrix4));

    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexturing.fx";
    mVertexShader.Initialize<VertexPX>(shaderFile);
    mPixelShader.Initialize(shaderFile);

    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    //RENDER TARGET
    constexpr uint32_t size = 512;
    mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U32);

    //Sky sphere
    mSkySphere.Initialize();

    ////  Planets   \\\\
    //Mercury
    mPlanets[0].mPosition = { 20, 0.0f, 0.0f };
    mPlanets[0].SetPlanet(L"../../Assets/Images/planets/mercury.jpg", 0.3f);
    mPlanets[0].Initialize();
    mPlanets[0].SetPosition(mPlanets[0].mPosition);
    mPlanets[0].SetRotationRate(.3);
    mPlanets[0].SetTranslationRate(20.0f);
  
    //Venus
    mPlanets[1].mPosition = { 33, 0.0f, 0.0f };
    mPlanets[1].SetPlanet(L"../../Assets/Images/planets/venus.jpg", 0.8f);
    mPlanets[1].Initialize();
    mPlanets[1].SetPosition({ 33, 0.0f, 0.0f });
    mPlanets[1].SetRotationRate(.2);
    mPlanets[1].SetTranslationRate(28.0f);

    //Earth
    mPlanets[2].mPosition = { 50, 0.0f, 0.0f };
    mPlanets[2].SetPlanet(L"../../Assets/Images/planets/earth.jpg", 0.8f);
    mPlanets[2].Initialize();
    mPlanets[2].SetPosition({ 50, 0.0f, 0.0f });
    mPlanets[0].SetRotationRate(.2f);
    mPlanets[0].SetTranslationRate(38.0f);

    //Mars
    mPlanets[3].mPosition = { 65, 0.0f, 0.0f };
    mPlanets[3].SetPlanet(L"../../Assets/Images/planets/mars.jpg", 0.5f);
    mPlanets[3].Initialize();
    mPlanets[3].SetPosition({ 65, 0.0f, 0.0f });
    //Jupiter
    mPlanets[4].mPosition = { 78, 0.0f, 0.0f };
    mPlanets[4].SetPlanet(L"../../Assets/Images/planets/jupiter.jpg", 4.0f);
    mPlanets[4].Initialize();
    mPlanets[4].SetPosition({ 78, 0.0f, 0.0f });
    mPlanets[4].SetRotationRate(.45);
    //Saturn
    mPlanets[5].mPosition = { 98, 0.0f, 0.0f };
    mPlanets[5].SetPlanet(L"../../Assets/Images/planets/saturn.jpg", 4.0f);
    mPlanets[5].Initialize();
    mPlanets[5].SetPosition({ 98, 0.0f, 0.0f });
    mPlanets[5].SetRotationRate(.7f);
    //Uranus
    mPlanets[6].mPosition = { 110, 0.0f, 0.0f };
    mPlanets[6].SetPlanet(L"../../Assets/Images/planets/uranus.jpg", 1.8f);
    mPlanets[6].Initialize();
    mPlanets[6].SetPosition({ 110, 0.0f, 0.0f });
    //Neptune
    mPlanets[7].mPosition = { 125, 0.0f, 0.0f };
    mPlanets[7].SetPlanet(L"../../Assets/Images/planets/neptune.jpg", 0.7f);
    mPlanets[7].Initialize();
    mPlanets[7].SetPosition({ 125, 0.0f, 0.0f });
    //Pluto 
    mPlanets[8].mPosition = { 135, 0.0f, 0.0f };
    mPlanets[8].SetPlanet(L"../../Assets/Images/planets/pluto.jpg", 0.4f);
    mPlanets[8].Initialize();
    mPlanets[8].SetPosition({ 135, 0.0f, 0.0f });

    //Sun
    mSun.Initialize();
    mSun.SetPosition(FoxMath::Vector3::Zero);
}

void GameState::Terminate()
{
    //Planets
    for (size_t i = 0; i < std::size(mPlanets); i++)
    {
        mPlanets[i].Terminate();
    }

    //Sky Sphere
    mSkySphere.Terminate();
    
    // Sun
    mSun.Terminate();


    mRenderTarget.Terminate();
    mSampler.Terminate();

    mPixelShader.Terminate();
    mVertexShader.Terminate();

    mConstantBuffer.Terminate();
}

void GameState::Render()
{

    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);

    //RENDER TARGET
    Vector3 pos;

    mRenderTarget.BeginRender();
    if (gbSun)
    {
        pos = Vector3(0.0f, 0.0f, -20.0f);
        mRenderTargetCamera.SetPosition(pos);
        mSun.Render(mRenderTargetCamera, mConstantBuffer, true);
        mSkySphere.Render(mRenderTargetCamera, mConstantBuffer, true);

    }
    else
    {
        pos = Vector3(mPlanets[gPlanetIndex].GetPosition().x, mPlanets[gPlanetIndex].GetPosition().y, mPlanets[gPlanetIndex].GetPosition().z + gPlanetOffset);
        mRenderTargetCamera.SetPosition(pos);
        mPlanets[gPlanetIndex].Render(mRenderTargetCamera, mConstantBuffer, true);
        mSkySphere.Render(mRenderTargetCamera, mConstantBuffer, true);

    }
    mRenderTarget.EndRender();

    //Render universe
    RenderMesh(mCamera, true);
}
void GameState::RenderMesh(const Camera& camera, bool useTransform)
{
    mSkySphere.Render(camera, mConstantBuffer, useTransform);
    mSun.Render(camera, mConstantBuffer, true);
    //Planets
    for (size_t i = 0; i < std::size(mPlanets); i++)
    {
        mPlanets[i].Render(camera, mConstantBuffer, useTransform);//RENDER TARGET
    }
}

void GameState::Update(float deltaTime)
{
    EngineCameraController(deltaTime);

    //Planets
    for (size_t i = 0; i < std::size(mPlanets); i++)
    {
        mPlanets[i].Update(deltaTime);
    }

    mSkySphere.Update(deltaTime);
}
void GameState::DebugUI()
{
  
    ImGui::Begin("Debug Draw", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Checkbox("Draw World", &mDrawCircleOrbits);
    if (mDrawCircleOrbits)
    {
        for (Planet& planet : mPlanets)
        {
            planet.AddCircleOrbt(mDrawCircleOrbits);
        }
    }

    //RENDER TARGET

    RenderAndUpdateSelected(mPlanets);


    ImGui::Text("Render Target");
    ImGui::Image(
        mRenderTarget.GetRawData(),
        { 128, 128 },
        { 0, 0 },
        { 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 }
    );

    SimpleDraw::Render(mCamera);
    ImGui::End();
}

void GameState::EngineCameraController(float deltaTime)
{
    auto input = Input::InputSystem::Get();
    const int moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10 : 1;
    const float d = 0.01f * moveSpeed;


    //Foward and Backward
    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(d);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-d);
    }
    //Right and Left
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(d);
    }
    else if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-d);
    }

    //Rotation
    if (input->IsMouseDown(MouseButton::RBUTTON))
    {

        const int turnSpeedMultiplyer = input->IsKeyDown(KeyCode::LSHIFT) ? 2 : 1;

        const float x = input->GetMouseMoveX() * deltaTime * turnSpeedMultiplyer;
        const float y = input->GetMouseMoveY() * deltaTime * turnSpeedMultiplyer;

        mCamera.Yaw(x);
        mCamera.Pitch(y);

    }

    //UP and Down

    if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-d);
    }
    else if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(d);
    }
}

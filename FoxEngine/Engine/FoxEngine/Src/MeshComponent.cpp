#include "Precompiled.h"
#include "MeshComponent.h"

#include "GameWorld.h"
#include "RenderService.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

void MeshComponent::Initialize()
{
	RenderService* rs = GetOwner().GetWorld().GetService<RenderService>();
	rs->Register(this);
}

void MeshComponent::Terminate()
{
	RenderService* rs = GetOwner().GetWorld().GetService<RenderService>();
	rs->Unregister(this);
}

void MeshComponent::Deserialize(const rapidjson::Value& value)
{
	Model::MeshData& meshData = mModel.meshData.emplace_back();
	Model::MaterialData& material = mModel.materialData.emplace_back();

	if (value.HasMember("Shape"))
	{
		const auto& shapeData = value["Shape"].GetObj();
		if(shapeData.HasMember("Type"))
		{
			const std::string shapeType = shapeData["Type"].GetString();
			if(shapeType == "Cube")
			{
				const float size = shapeData["Size"].GetFloat();
				meshData.mesh = MeshBuilder::CreateCube(size);
			}
			else if(shapeType == "Sphere")
			{
				const int slices = shapeData["Slices"].GetInt();
				const int rings = shapeData["Rings"].GetInt();
				const float radius = shapeData["Radius"].GetFloat();

				meshData.mesh = MeshBuilder::CreateSphere(slices, rings, radius);
			}
			else if (shapeType == "Plane")
			{
				const int rows = shapeData["Rows"].GetInt();
				const int columns = shapeData["Columns"].GetInt();
				const float spacing = shapeData["Spacing"].GetFloat();

				meshData.mesh = MeshBuilder::CreateGroundPlane(rows, columns, spacing);

			}else
			{
				ASSERT(false, "ShapeType %s is Invalid", shapeType.c_str());
			}
		}
	}//end shape

	if(value.HasMember("Material"))
	{
		const auto& materialData = value["Material"].GetObj();

		if(materialData.HasMember("ColorAmbient"))
		{
			const auto& color = materialData["ColorAmbient"].GetArray();
			material.material.ambient.r = color[0].GetFloat();
			material.material.ambient.g = color[1].GetFloat();
			material.material.ambient.b = color[2].GetFloat();
			material.material.ambient.a = color[3].GetFloat();
		}

		if (materialData.HasMember("ColorDiffuse"))
		{
			const auto& color = materialData["ColorDiffuse"].GetArray();
			material.material.diffuse.r = color[0].GetFloat();
			material.material.diffuse.g = color[1].GetFloat();
			material.material.diffuse.b = color[2].GetFloat();
			material.material.diffuse.a = color[3].GetFloat();
		}

		if (materialData.HasMember("ColorSpecular"))
		{
			const auto& color = materialData["ColorSpecular"].GetArray();
			material.material.specular.r = color[0].GetFloat();
			material.material.specular.g = color[1].GetFloat();
			material.material.specular.b = color[2].GetFloat();
			material.material.specular.a = color[3].GetFloat();
		}

		if (materialData.HasMember("ColorEmissive"))
		{
			const auto& color = materialData["ColorEmissive"].GetArray();
			material.material.emissive.r = color[0].GetFloat();
			material.material.emissive.g = color[1].GetFloat();
			material.material.emissive.b = color[2].GetFloat();
			material.material.emissive.a = color[3].GetFloat();
		}

		if (materialData.HasMember("SpecularPower"))
		{
			material.material.power = materialData["SpecularPower"].GetFloat();
		}

		if (value.HasMember("Textures"))
		{
			const auto& texturesData = value["Textures"].GetObj();

			if (texturesData.HasMember("DiffuseMap"))
			{
				material.diffuseMapName = texturesData["DiffuseMap"].GetString();
			}

			if (texturesData.HasMember("NormalMap"))
			{
				material.normalMapName = texturesData["NormalMap"].GetString();
			}

			if (texturesData.HasMember("BumpMap"))
			{
				material.bumpMapName = texturesData["BumpMap"].GetString();
			}

			if (texturesData.HasMember("SpecularMap"))
			{
				material.specularMapName = texturesData["SpecularMap"].GetString();
			}
		}
	}

	if (value.HasMember("CastShadow"))
	{
		mCastShadow = value["CastShadow"].GetBool();
	}
}

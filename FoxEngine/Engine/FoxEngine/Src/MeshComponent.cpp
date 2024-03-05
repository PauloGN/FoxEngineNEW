#include "Precompiled.h"
#include "MeshComponent.h"

#include "GameWorld.h"
#include "RenderService.h"
#include "SaveUtil.h"

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

void MeshComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	rapidjson::Value ShapeValue(rapidjson::kObjectType);
	rapidjson::Value materialValue(rapidjson::kObjectType);
	rapidjson::Value texturesValue(rapidjson::kObjectType);

	SaveUtil::SaveString("Type", mLoadingData.shapeType.c_str(), doc, ShapeValue);

	if (mLoadingData.shapeType == "Cube")
	{
		SaveUtil::SaveFloat("Size", mLoadingData.fParam, doc, ShapeValue);
	}
	else if (mLoadingData.shapeType == "Sphere")
	{
		SaveUtil::SaveInt("Slices", mLoadingData.iParam0, doc, ShapeValue);
		SaveUtil::SaveInt("Rings", mLoadingData.iParam1, doc, ShapeValue);
		SaveUtil::SaveFloat("Radius", mLoadingData.fParam, doc, ShapeValue);
	}
	else if (mLoadingData.shapeType == "Plane")
	{
		SaveUtil::SaveInt("Rows", mLoadingData.iParam0, doc, ShapeValue);
		SaveUtil::SaveInt("Columns", mLoadingData.iParam1, doc, ShapeValue);
		SaveUtil::SaveFloat("Spacing", mLoadingData.fParam, doc, ShapeValue);
	}
	else
	{
		ASSERT(false, "ShapeType %s is Invalid", mLoadingData.shapeType.c_str());
	}
	//Material
	Model::MaterialData& material = mModel.materialData[0];
	SaveUtil::SaveColor("ColorAmbient", material.material.ambient, doc, materialValue);
	SaveUtil::SaveColor("ColorDiffuse", material.material.diffuse, doc, materialValue);
	SaveUtil::SaveColor("ColorSpecular", material.material.specular, doc, materialValue);
	SaveUtil::SaveColor("ColorEmissive", material.material.emissive, doc, materialValue);
	SaveUtil::SaveFloat("SpecularPower",material.material.power, doc, materialValue);
		//Textures
	if (!material.diffuseMapName.empty())
	{
		SaveUtil::SaveString("DiffuseMap", material.diffuseMapName.c_str(), doc, texturesValue);
	}
	if (!material.normalMapName.empty())
	{
		SaveUtil::SaveString("NormalMap", material.normalMapName.c_str(), doc, texturesValue);
	}
	if (!material.bumpMapName.empty())
	{
		SaveUtil::SaveString("BumpMap", material.bumpMapName.c_str(), doc, texturesValue);
	}
	if (!material.specularMapName.empty())
	{
		SaveUtil::SaveString("SpecularMap", material.specularMapName.c_str(), doc, texturesValue);
	}
	//Save component name/data
	componentValue.AddMember("Shape", ShapeValue, doc.GetAllocator());
	componentValue.AddMember("Material", materialValue, doc.GetAllocator());
	componentValue.AddMember("Textures", texturesValue, doc.GetAllocator());
	SaveUtil::SaveBool("CastShadow", mCastShadow, doc, componentValue);

	value.AddMember("MeshComponent", componentValue, doc.GetAllocator());
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
			mLoadingData.shapeType = shapeData["Type"].GetString();
			if(mLoadingData.shapeType == "Cube")
			{
				mLoadingData.fParam = shapeData["Size"].GetFloat();
				meshData.mesh = MeshBuilder::CreateCube(mLoadingData.fParam);
			}
			else if(mLoadingData.shapeType == "Sphere")
			{
				mLoadingData.iParam0 = shapeData["Slices"].GetInt();
				mLoadingData.iParam1 = shapeData["Rings"].GetInt();
				mLoadingData.fParam = shapeData["Radius"].GetFloat();

				meshData.mesh = MeshBuilder::CreateSphere(mLoadingData.iParam0, mLoadingData.iParam1, mLoadingData.fParam);
			}
			else if (mLoadingData.shapeType == "Plane")
			{
				mLoadingData.iParam0 = shapeData["Rows"].GetInt();
				mLoadingData.iParam1 = shapeData["Columns"].GetInt();
				mLoadingData.fParam = shapeData["Spacing"].GetFloat();

				meshData.mesh = MeshBuilder::CreateGroundPlane(mLoadingData.iParam0, mLoadingData.iParam1, mLoadingData.fParam);

			}else
			{
				ASSERT(false, "ShapeType %s is Invalid", mLoadingData.shapeType.c_str());
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

void MeshComponent::EditorUI()
{
	std::string headerTag = "Mesh Component: " + GetOwner().GetName();
	if(ImGui::CollapsingHeader(headerTag.c_str()))
	{
		bool bUpdate = false;

		Material& material = mModel.materialData[0].material;

		if(ImGui::ColorEdit4("Ambient: ", &material.ambient.r))
		{
			bUpdate = true;
		}
		if(ImGui::ColorEdit4("Diffuse: ", &material.diffuse.r))
		{
			bUpdate = true;
		}
		if(ImGui::ColorEdit4("Specular: ", &material.specular.r))
		{
			bUpdate = true;
		}
		if(ImGui::ColorEdit4("Emissive: ", &material.emissive.r))
		{
			bUpdate = true;
		}
		if(ImGui::DragFloat("Power: ", &material.power))
		{
			bUpdate = true;
		}

		if(ImGui::Checkbox("Cast Shadow: ", &mCastShadow))
		{
			bUpdate = true;
		}

		if(bUpdate)
		{
			RenderService* rs = GetOwner().GetWorld().GetService<RenderService>();
			rs->Unregister(this);
			rs->Register(this);
		}
	}
}

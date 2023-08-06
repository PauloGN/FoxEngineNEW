#include <FoxEngine/Inc/FoxEngine.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cstdio>

using namespace FoxEngine;
using namespace FoxEngine::Graphics;
using namespace FoxEngine::FoxMath;

struct Arguments
{
	std::filesystem::path inputFileName;
	std::filesystem::path outputFileName;
	float scale = 1.0f;
};

std::optional<Arguments> ParseArgs(int argc, char* argv[])
{
	if (argc < 3)
	{
		return std::nullopt;
	}

	Arguments arguments;
	arguments.inputFileName = argv[argc - 2];
	arguments.outputFileName = argv[argc - 1];

	for (int i = 1; i + 2 < argc ; ++i)
	{
		if (strcmp( argv[i], "-scale") == 0)
		{
			arguments.scale = atof(argv[i + 1]);
			++i;
		}
	}

	return arguments;
}

Vector3 ToVector3(const aiVector3D& v)
{
	return{
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z)
	};
}

Quaternion ToQuaternion(const aiQuaternion& q)
{
	return{
		static_cast<float>(q.x),
		static_cast<float>(q.y),
		static_cast<float>(q.z),
		static_cast<float>(q.w)
	};
}

Color ToColor(const aiColor3D& c)
{
	return{
		static_cast<float>(c.r),
		static_cast<float>(c.g),
		static_cast<float>(c.b),
		1.0f
	};
}

Matrix4 ToMatrix4(const aiMatrix4x4& m)
{
	return{
		static_cast<float>(m.a1), static_cast<float>(m.b1), static_cast<float>(m.c1), static_cast<float>(m.d1),
		static_cast<float>(m.a2), static_cast<float>(m.b2), static_cast<float>(m.c2), static_cast<float>(m.d2),
		static_cast<float>(m.a3), static_cast<float>(m.b3), static_cast<float>(m.c3), static_cast<float>(m.d3),
		static_cast<float>(m.a4), static_cast<float>(m.b4), static_cast<float>(m.c4), static_cast<float>(m.d4)
	};
}

void ExporteEmbeddedTexture(const aiTexture* texture, const Arguments& args, const std::filesystem::path& fileName)
{
	printf("Extracting embedded texture %s\n", fileName.u8string().c_str());

	std::string fullFileName = args.outputFileName.u8string();
	fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
	fullFileName += fileName.filename().u8string();

	FILE* file = nullptr;
	auto err = fopen_s(&file, fullFileName.c_str(), "wb");
	if (err != 0 || file ==nullptr)
	{
		printf("Error: failed to open file %s for saving\n", fullFileName.c_str());
		return;
	}

	size_t written = fwrite(texture->pcData, 1, texture->mWidth, file);
	ASSERT(written == texture->mWidth, "Error: failed to extract embedded texture!");
	fclose(file);
}


std::string FindTexture(const aiScene* scene, const aiMaterial* aiMaterial, aiTextureType textureType, const Arguments& args, const std::string& suffix, uint32_t materialIndex)
{
	const uint32_t textureCount = aiMaterial->GetTextureCount(textureType);
	if (textureCount == 0)
	{
		return "";
	}

	std::filesystem::path textureName;
	aiString texturePath;
	if (aiMaterial->GetTexture(textureType, 0, &texturePath) == aiReturn_SUCCESS)
	{
		if (texturePath.C_Str()[0] == '*')
		{
			std::string fileName = args.inputFileName.u8string();
			fileName.erase(fileName.length() - 4);
			fileName += suffix;
			fileName += texturePath.C_Str()[1];

			ASSERT(scene->HasTextures(), "Error: no embedded texture found!\n");

			int textureIndex = atoi(texturePath.C_Str() + 1);
			ASSERT(textureIndex < scene->mNumTextures, "Error: Invalid texture index!\n");

			const aiTexture* embeddedTexture = scene->mTextures[textureIndex];
			ASSERT(embeddedTexture->mHeight == 0, "Error: Uncompressed texture found!\n");

			if (embeddedTexture->CheckFormat("jpg"))
			{
				fileName += "jpg";
			}
			else if (embeddedTexture->CheckFormat("png"))
			{
				fileName += "png";
			}
			else
			{
				ASSERT(false, "Error: Unrecognized format!\n");
			}

			//ExportEmbeddedTexture...
			ExporteEmbeddedTexture(embeddedTexture, args, fileName);

			printf("Adding texture %s\n", fileName.c_str());
			textureName = fileName;
		}
		else if(auto embeddedTexture = scene->GetEmbeddedTexture(texturePath.C_Str()); embeddedTexture)
		{
			std::filesystem::path embeddedFilePath = texturePath.C_Str();
			std::string fileName = args.inputFileName.u8string();
			fileName.erase(fileName.length() - 4);
			fileName += suffix;
			fileName += "_" + std::to_string(materialIndex);
			fileName += embeddedFilePath.extension().u8string();

			//ExportEmbeddedTexture
			ExporteEmbeddedTexture(embeddedTexture, args, fileName);

			printf("Adding texture %s\n", fileName.c_str());
			textureName = fileName;
		}
		else
		{
			std::filesystem::path filePath = texturePath.C_Str();
			std::string fileName = filePath.filename().u8string();

			printf("Adding texture %s\n", fileName.c_str());
			textureName = fileName;
		}
	}

	return textureName.filename().u8string();
}


int main(int argc, char* argv[])
{
	const auto argOpt = ParseArgs(argc, argv);

	if (argOpt.has_value() == false)
	{
		printf("No Arguments present to import fbx model\n");
		return -1;
	}

	const auto& arguments = argOpt.value();
	Assimp::Importer importer;
	const uint32_t flags = aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded;

	const aiScene* scene = importer.ReadFile(arguments.inputFileName.u8string().c_str(), flags);
	if (scene == nullptr)
	{
		printf("Error: %s\n", importer.GetErrorString());
		return -1;
	}

	printf("Importing %s...\n", arguments.inputFileName.u8string().c_str());

	Model model;
	if (scene->HasMeshes())
	{
		printf("Reading mesh data...\n");
		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			const auto& aiMesh = scene->mMeshes[meshIndex];
			if (aiMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
			{
				continue;
			}

			const uint32_t numVertices = aiMesh->mNumVertices;
			const uint32_t numFaces = aiMesh->mNumFaces;
			const uint32_t numIndices = numFaces * 3;

			auto& meshData = model.meshData.emplace_back();

			printf("Reading Material Index...\n");
			meshData.materialIndex = aiMesh->mMaterialIndex;

			printf("Reading Vertices...\n");

			auto& mesh = meshData.mesh;
			mesh.vertices.reserve(numVertices);

			const aiVector3D* positions = aiMesh->mVertices;
			const aiVector3D* normals = aiMesh->mNormals;
			const aiVector3D* tangents = aiMesh->HasTangentsAndBitangents() ? aiMesh->mTangents : nullptr;
			const aiVector3D* texCoords = aiMesh->HasTextureCoords(0) ? aiMesh->mTextureCoords[0] : nullptr;

			for (uint32_t v = 0; v < numVertices; ++v)
			{
				auto& vertex = mesh.vertices.emplace_back();
				vertex.position = ToVector3(positions[v]) * arguments.scale;
				vertex.normal = ToVector3(normals[v]);
				vertex.tangent = tangents ? ToVector3(tangents[v]) : Vector3::Zero;
				if (texCoords)
				{
					vertex.uvCoord = { texCoords[v].x, texCoords[v].y };
				}
			}

			printf("Reading indices...\n");
			mesh.indices.reserve(numIndices);
			const auto& aiFaces = aiMesh->mFaces;
			for (uint32_t i = 0; i < numFaces; ++i)
			{
				const auto& aiFace = aiFaces[i];
				mesh.indices.push_back(aiFace.mIndices[0]);
				mesh.indices.push_back(aiFace.mIndices[1]);
				mesh.indices.push_back(aiFace.mIndices[2]);
			}
		}
	}

	if (scene->HasMaterials())
	{
		printf("Reading material data...\n");

		const uint32_t numMaterial = scene->mNumMaterials;
		model.materialData.reserve(numMaterial);

		for (uint32_t materialIndex = 0; materialIndex < numMaterial; ++materialIndex)
		{
			const auto aiMaterial = scene->mMaterials[materialIndex];
			aiColor3D ambient, diffuse, emissive, specular;
			ai_real specularPower = 1.0f;

			aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
			aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
			aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
			aiMaterial->Get(AI_MATKEY_SHININESS, specularPower);

			auto& materialData = model.materialData.emplace_back();
			materialData.material.ambient = ToColor(ambient);
			materialData.material.diffuse = ToColor(diffuse);
			materialData.material.emissive = ToColor(emissive);
			materialData.material.specular = ToColor(specular);
			materialData.material.power = static_cast<float>(specularPower);

			//Get texture
			materialData.diffuseMapName = FindTexture(scene, aiMaterial, aiTextureType_DIFFUSE, arguments, "_diffuse", materialIndex);
			materialData.normalMapName = FindTexture(scene, aiMaterial, aiTextureType_NORMALS, arguments, "_normal", materialIndex);
			materialData.bumpMapName = FindTexture(scene, aiMaterial, aiTextureType_DISPLACEMENT, arguments, "_bump", materialIndex);
			materialData.specularMapName = FindTexture(scene, aiMaterial, aiTextureType_SPECULAR, arguments, "_specular", materialIndex);
		}
	}

	printf("Saving model...\n");
	ModelIO::SaveModel(arguments.outputFileName, model);

	printf("Saving material...\n");
	ModelIO::SaveMaterial(arguments.outputFileName, model);

	return 0;
}
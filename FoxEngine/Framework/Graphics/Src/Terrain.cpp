#include "Precompiled.h"
#include "Terrain.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

void Terrain::Initialize(const std::filesystem::path& fileName, float heightScale)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "rb");

	ASSERT(file != nullptr, "Terrain: file was not found %s", fileName.c_str());

	fseek(file, 0L, SEEK_END);
	const uint32_t fileSize = ftell(file);
	const uint32_t dimensions = static_cast<uint32_t>(sqrt(static_cast<float>(fileSize)));
	fseek(file, 0L, SEEK_SET);

	mRows = dimensions;
	mColumns = dimensions;

	const float tileCount = 35.0f;

	mesh.vertices.resize(mRows * mColumns);
	for (uint32_t z = 0; z < mRows; ++z)
	{
		for (uint32_t x = 0; x < mColumns; ++x)
		{
			const int c = fgetc(file);
			const float height = (c / 255.f) * heightScale;
			const uint32_t index = x + (z * mColumns);

			FoxEngine::Graphics::Vertex& vertex = mesh.vertices[index];
			const float posX = static_cast<float>(x);
			const float posZ = static_cast<float>(z);
			vertex.position = { posX, height, posZ };
			vertex.normal = FoxMath::Vector3::YAxis;
			vertex.uvCoord.x = (static_cast<float>(x) / mColumns) * tileCount;
			vertex.uvCoord.y = (static_cast<float>(z) / mRows) * tileCount;
		}
	}
	fclose(file);

	const uint32_t cells = (mRows - 1) * (mColumns - 1);
	mesh.indices.reserve(cells * 6);
	for (uint32_t z = 0; z < mRows - 1; ++z)
	{
		for (uint32_t x = 0; x < mColumns - 1; ++x)
		{
			const uint32_t bl = x + (z * mColumns);
			const uint32_t tl = x + ((z + 1) * mColumns);
			const uint32_t br = (x + 1) + (z * mColumns);
			const uint32_t tr = (x + 1) + ((z + 1) * mColumns);

			mesh.indices.push_back(bl);
			mesh.indices.push_back(tl);
			mesh.indices.push_back(tr);

			mesh.indices.push_back(bl);
			mesh.indices.push_back(tr);
			mesh.indices.push_back(br);
		}
	}
}

float Terrain::GetHeight(const FoxMath::Vector3 & position) const
{
	const int x = static_cast<int>(position.x);
	const int z = static_cast<int>(position.z);

	if (x < 0 || z < 0 || x + 1 >= mColumns || z + 1 >= mRows)
	{
		return 0.0f;
	}

	const uint32_t bl = x + (z * mColumns);
	const uint32_t tl = x + ((z + 1) * mColumns);
	const uint32_t br = (x + 1) + (z * mColumns);
	const uint32_t tr = (x + 1) + ((z + 1) * mColumns);

	const float u = position.x - x;
	const float v = position.z - z;

	float height;
	if (u > v) // bottom right triangle
	{
		const auto& a = mesh.vertices[br];
		const auto& b = mesh.vertices[tr];
		const auto& c = mesh.vertices[bl];
		const auto deltaAB = b.position.y - a.position.y;
		const auto deltaAC = c.position.y - a.position.y;
		height = a.position.y + (deltaAB * v) + (deltaAC * (1 - u));
	}
	else // top left
	{
		const auto& a = mesh.vertices[tl];
		const auto& b = mesh.vertices[tr];
		const auto& c = mesh.vertices[bl];
		const float deltaAB = b.position.y - a.position.y;
		const float deltaAC = c.position.y - a.position.y;
		height = a.position.y + (deltaAB * u) + (deltaAC * (1 - v));
	}

	return height;
}
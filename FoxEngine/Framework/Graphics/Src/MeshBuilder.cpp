#include "Precompiled.h"
#include "MeshBuffer.h"
#include "MeshBuilder.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

namespace
{

	Color GetNextColor(int& index)
	{
		constexpr Color colorTable[] = {
			Colors::DarkMagenta,
			Colors::Aqua,
			Colors::Azure,
			Colors::AliceBlue,
			Colors::DarkOliveGreen,
			Colors::Bisque,
			Colors::DarkGray,
			Colors::Orange,
			Colors::Red,
			Colors::Green,
			Colors::Blue,
			Colors::Yellow
		};

		index = (index + 1) % std::size(colorTable);

		return colorTable[index];
	}

}

void CreateCubeIdices(std::vector<uint32_t>& indices)
{
	indices = {
		//front
		0,1,2,
		0,2,3,
		//back
		7, 6, 5,
		7, 5, 4,
		//right
		3, 2, 6,
		3, 6, 7,
		//Left
		0, 5, 1,
		0, 4, 5,
		//Top
		1, 5, 6,
		1, 6, 2,
		//Bottom
		0, 3 , 7,
		0, 7, 4
	};
}

void CreateSkyBoxIdices(std::vector<uint32_t>& indices)
{
	indices = {

		//Front
		0 , 2, 1,
		1, 2, 3,
		//Back
		5, 4 , 7,
		4, 6, 7,
		//Right
		9, 8, 11,
		8, 10, 11,
		//Left
		13, 12, 15,
		12, 14, 15,
		//Top
		18, 19 , 16,
		19, 17, 16,
		//Bottom
		21, 20, 23,
		20, 22, 23
	};

}

void CreatePlaneIndices(std::vector<uint32_t>& indices, int numRows, int numColums)
{

	for (int r = 0; r <= numRows; ++r)
	{
		for (int c = 0; c < numColums; ++c)
		{
			int i = r * (numColums)+c;

			//triangle 1
			indices.push_back(i);
			indices.push_back(i + numColums + 1);
			indices.push_back(i + 1);

			//triangle 2
			indices.push_back(i);
			indices.push_back(i + numColums);
			indices.push_back(i + numColums + 1);
		}
	}
}

void CreateCapIndices(std::vector<uint32_t>& indices, int slices, int topIndex, int bottomIndex)
{
	for (int s = 0; s < slices; s++)
	{
		//Bottom Triangles
		indices.push_back(s);
		indices.push_back(s + 1);
		indices.push_back(bottomIndex);

		//Top Triangles

		int topRowSIndex = topIndex - slices - 1 + s;

		indices.push_back(topIndex);
		indices.push_back(topRowSIndex + 1);
		indices.push_back(topRowSIndex);
	}
}



MeshPC FoxEngine::Graphics::MeshBuilder::CreateCubePC(float size, const Color& color)
{
	MeshPC mesh;

	int index = rand() % 100;

	const float halfSize = size * 0.5f;
	//front
	mesh.vertices.push_back({{-halfSize,-halfSize, -halfSize}, GetNextColor(index) });	//0 FBL
	mesh.vertices.push_back({{-halfSize, halfSize, -halfSize}, GetNextColor(index) });	//1 FTL
	mesh.vertices.push_back({{halfSize, halfSize, -halfSize}, GetNextColor(index) });		//2 FTR
	mesh.vertices.push_back({{halfSize,-halfSize, -halfSize}, GetNextColor(index) });		//3 FBR
	//Back
	mesh.vertices.push_back({ {-halfSize,-halfSize, halfSize}, GetNextColor(index) });	//4 BBL
	mesh.vertices.push_back({ {-halfSize, halfSize, halfSize}, GetNextColor(index) });	//5 BTL 
	mesh.vertices.push_back({ {halfSize, halfSize, halfSize}, GetNextColor(index) });		//6 BTR
	mesh.vertices.push_back({ {halfSize,-halfSize, halfSize}, GetNextColor(index) });		//7 BBR

	CreateCubeIdices(mesh.indices);

	return mesh;
}

MeshPX FoxEngine::Graphics::MeshBuilder::CreateCubePX(float size)
{
	MeshPX mesh; //Position and UV coord


	const float halfSize = size * 0.5f;
	////front
	//mesh.vertices.push_back({ {-halfSize,-halfSize, -halfSize}, { 0.25f, 0.66f } });	//0 FBL
	//mesh.vertices.push_back({ {-halfSize, halfSize, -halfSize}, { 0.25f, 0.33f } });	//1 FTL
	//mesh.vertices.push_back({ {halfSize, halfSize, -halfSize}, { 0.5f, 0.33f } });		//2 FTR
	//mesh.vertices.push_back({ {halfSize,-halfSize, -halfSize}, { 0.5f, 0.66f } });		//3 FBR
	////Back
	//mesh.vertices.push_back({ {-halfSize,-halfSize, halfSize}, { 0.25f, 1.0f } });		//4 BBL
	//mesh.vertices.push_back({ {-halfSize, halfSize, halfSize}, { 0.0f, 0.33f } });		//5 BTL 
	//mesh.vertices.push_back({ {halfSize, halfSize, halfSize}, { 0.5f, 0.0f } });		//6 BTR
	//mesh.vertices.push_back({ {halfSize,-halfSize, halfSize}, { 0.5f, 1.0f } });		//7 BBR

	//front
	mesh.vertices.push_back({ {-halfSize,-halfSize, -halfSize}, { 0.0f, 1.0f } });		//0 FBL
	mesh.vertices.push_back({ {-halfSize, halfSize, -halfSize}, { 0.0f, 0.0f } });		//1 FTL
	mesh.vertices.push_back({ {halfSize, halfSize, -halfSize}, { 1.0f, 0.0f } });		//2 FTR
	mesh.vertices.push_back({ {halfSize,-halfSize, -halfSize}, { 1.0f, 1.0f } });		//3 FBR
	//Back
	mesh.vertices.push_back({ {-halfSize,-halfSize, halfSize}, { 1.0f, 1.0f } });		//4 BBL
	mesh.vertices.push_back({ {-halfSize, halfSize, halfSize}, { 1.0f, 0.0f } });		//5 BTL 
	mesh.vertices.push_back({ {halfSize, halfSize, halfSize}, { 0.0f, 0.0f } });		//6 BTR
	mesh.vertices.push_back({ {halfSize,-halfSize, halfSize}, { 0.0f, 1.0f } });		//7 BBR


	CreateCubeIdices(mesh.indices);

	return mesh;
}

Mesh FoxEngine::Graphics::MeshBuilder::CreateCube(float size)
{
	Mesh mesh;
	return mesh;
}

MeshPC FoxEngine::Graphics::MeshBuilder::CreateRectPC(float width, float height, float depth, const Color& color)
{
	MeshPC mesh;

	int index = rand() % 100;

	const float hw = width * 0.5f;	//x
	const float hh = height * 0.5f;	//y
	const float hd = depth * 0.5f;	//z

	//front
	mesh.vertices.push_back({ {-hw,-hh, -hd}, GetNextColor(index) });	//0 FBL
	mesh.vertices.push_back({ {-hw, hh, -hd}, GetNextColor(index) });	//1 FTL
	mesh.vertices.push_back({ {hw, hh, -hd}, GetNextColor(index) });		//2 FTR
	mesh.vertices.push_back({ {hw,-hh, -hd}, GetNextColor(index) });		//3 FBR
	//Back
	mesh.vertices.push_back({ {-hw,-hh, hd}, GetNextColor(index) });		//4 BBL
	mesh.vertices.push_back({ {-hw, hh, hd}, GetNextColor(index) });		//5 BTL 
	mesh.vertices.push_back({ {hw, hh, hd}, GetNextColor(index) });		//6 BTR
	mesh.vertices.push_back({ {hw,-hh, hd}, GetNextColor(index) });		//7 BBR

	CreateCubeIdices(mesh.indices);

	return mesh;
}

MeshPX FoxEngine::Graphics::MeshBuilder::CreateRectPX(float width, float height, float depth)
{
	MeshPX mesh;

	const float hw = width * 0.5f;	//x
	const float hh = height * 0.5f;	//y
	const float hd = depth * 0.5f;	//z

	//front
	mesh.vertices.push_back({ {-hw,-hh, -hd}, { 0.25f, 0.66f } });		//0 FBL
	mesh.vertices.push_back({ {-hw, hh, -hd}, { 0.25f, 0.33f } });		//1 FTL
	mesh.vertices.push_back({ {hw, hh, -hd}, { 0.5f, 0.33f } });		//2 FTR
	mesh.vertices.push_back({ {hw,-hh, -hd}, { 0.5f, 0.66f } });		//3 FBR
	//Back
	mesh.vertices.push_back({ {-hw,-hh, hd}, { 0.25f, 1.0f } });		//4 BBL
	mesh.vertices.push_back({ {-hw, hh, hd}, { 0.0f, 0.33f } });		//5 BTL 
	mesh.vertices.push_back({ {hw, hh, hd}, { 0.5f, 0.0f } });			//6 BTR
	mesh.vertices.push_back({ {hw,-hh, hd}, { 0.5f, 1.0f } });			//7 BBR
	

	CreateCubeIdices(mesh.indices);

	return mesh;
}

MeshPC FoxEngine::Graphics::MeshBuilder::CreatePlanePC(float numRows, int numColums, float spacing)
{
	MeshPC mesh;

	int index = rand() % 100;

	const float hpw = static_cast<float>(numColums) * spacing * 0.5f;
	const float hph = static_cast<float>(numRows) * spacing * 0.5f;

	float x = -hpw;
	float y = -hph;

	for (int r = 0; r <= numRows; r++)
	{
		for (int c = 0; c <= numColums; c++)
		{
			mesh.vertices.push_back({ {x, y, 0.0f}, GetNextColor(index)});
			x += spacing;
		}
		x = -hpw;
		y += spacing;
	}

	CreatePlaneIndices(mesh.indices, numRows, numColums);

	return mesh;
}

MeshPX FoxEngine::Graphics::MeshBuilder::CreatePlanePX(uint32_t numRows, uint32_t numColums, float spacing)
{

	ASSERT(numColums >= 2 && numRows >= 2, "Invalid Arguments for plane px");
	ASSERT(spacing > 0.0f, "Invalid Arguments for plane px");

	MeshPX mesh;//size/capacity = 0

	const uint32_t vertexCount = numColums * numRows;
	const uint32_t indexCount = (numColums - 1) * (numRows - 1) * 6;
	const float width = (numColums - 1) * spacing;
	const float length = (numRows - 1) * spacing;
	const float offsetX = width * -0.5f;
	const float offsetZ = length * -0.5f;
	const float uStep = 1.0f / (numColums - 1);
	const float vStep = 1.0f / (numRows - 1);

	//Reserve memory
	mesh.vertices.reserve(vertexCount);//pro code
	mesh.indices.reserve(indexCount);//pro code

	//Add Vertices
	for (uint32_t r = 0; r < numRows; ++r)
	{
		for (uint32_t c = 0; c < numColums; ++c)
		{
			const float x = (c * spacing) + offsetX;
			const float y = 0.0f;
			const float z = (r * spacing) + offsetZ;
			const float u = c * uStep;
			const float v = 1.0f - (r * vStep);
			mesh.vertices.push_back({ {x, y ,z}, {u, v} });//allocate
		}
	}

	//Add indices
	for (uint32_t r = 0; r < numRows - 1; ++r)
	{
		for (uint32_t c = 0; c < numColums - 1; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * numColums));
			mesh.indices.push_back((c + 0) + ((r + 1) * numColums));
			mesh.indices.push_back((c + 1) + ((r + 1) * numColums));

			mesh.indices.push_back((c + 0) + ((r + 0) * numColums));
			mesh.indices.push_back((c + 1) + ((r + 1) * numColums));
			mesh.indices.push_back((c + 1) + ((r + 0) * numColums));
		}
	}

	return mesh;
}

MeshPC FoxEngine::Graphics::MeshBuilder::CreateCylinderPC(int slices, int rings)
{
	MeshPC mesh;

	int index = rand() % 100;

	const float hh = static_cast<float>(rings) * 0.5f;

	for (int r = 0; r <= rings; ++r)
	{
		float ring = static_cast<float>(r);
		for (int s = 0; s <= slices; ++s)
		{
			float slice = static_cast<float>(s);
			float rotation = (slice / static_cast<float>(slices)) * FoxMath::Constants::TwoPi;

			mesh.vertices.push_back({
				{sin(rotation),	ring - hh,-cos(rotation)},
				GetNextColor(index)
				});
		}
	}

	mesh.vertices.push_back({ {0.0f, hh , 0.0f}, GetNextColor(index) });//Top Index
	mesh.vertices.push_back({ {0.0f, -hh , 0.0f}, GetNextColor(index) });//Bottom Index


	CreatePlaneIndices(mesh.indices, rings, slices);
	CreateCapIndices(mesh.indices, slices, mesh.vertices.size() -2, mesh.vertices.size() -1);

	return mesh;
}

MeshPC FoxEngine::Graphics::MeshBuilder::CreateSpherePC(int slices, int rings, float radius)
{
	MeshPC mesh;
	int index = rand() % 100;

	float verticalRotation = (FoxMath::Constants::Pi / static_cast<float> (rings - 1));
	float horizontalRotation = (FoxMath::Constants::TwoPi / static_cast<float> (slices));

	for (int r = 0; r <= rings; ++r)
	{
		float ring = static_cast<float>(r);
		float phi = ring * verticalRotation;

		for (int s = 0; s <= slices; ++s)
		{
			float slice = static_cast<float>(s);
			float rotation = slice * horizontalRotation;

			mesh.vertices.push_back({ {
				radius *sin(rotation) * sin(phi),
				radius * cos(phi),
				radius * cos(rotation) * sin(phi)
				}, GetNextColor(index)
				});
		}
	}

	CreatePlaneIndices(mesh.indices, rings, slices);

	return mesh;
}

MeshPX FoxEngine::Graphics::MeshBuilder::CreateSpherePX(int slices, int rings, float radius)
{
	MeshPX mesh;

	float verticalRotation = (FoxMath::Constants::Pi / static_cast<float> (rings - 1));
	float horizontalRotation = (FoxMath::Constants::TwoPi / static_cast<float> (slices));
	float uStep = 1.0f / static_cast<float>(slices);
	float vStep = 1.0f / static_cast<float>(rings);

	for (int r = 0; r <= rings; ++r)
	{
		float ring = static_cast<float>(r);
		float phi = ring * verticalRotation;

		for (int s = 0; s <= slices; ++s)
		{
			float slice = static_cast<float>(s);
			float rotation = slice * horizontalRotation;

			float u = 1.0f - (uStep * slice);
			float v = vStep * ring;

			mesh.vertices.push_back({ {
				radius * sin(rotation) * sin(phi),
				radius * cos(phi),
				radius * cos(rotation) * sin(phi)
				}, {u , v} });
		}
	}

	CreatePlaneIndices(mesh.indices, rings, slices);

	return mesh;
}

Mesh FoxEngine::Graphics::MeshBuilder::CreateSphere(int slices, int rings, float radius)
{
	Mesh mesh;
	return mesh;
}

MeshPX FoxEngine::Graphics::MeshBuilder::CreateSkyboxPX(float size)
{
	MeshPX skyMesh; //Position and UV coord

	const float halfSize = size * 0.5f;

	//constantes
	const float onef = 1.0f;
	
	//thirds
	const float oneThird = onef / 3.0f;
	const float twoThird = (onef + onef) / 3.0f;

	//Front
	skyMesh.vertices.push_back({ {-halfSize, halfSize, -halfSize},  { onef, oneThird } });		//0 FTL
	skyMesh.vertices.push_back({ {halfSize, halfSize, -halfSize},  { 0.75f, oneThird } });		//1 FTR
	skyMesh.vertices.push_back({ {-halfSize, -halfSize, -halfSize}, { onef, twoThird } });		//2 FBL
	skyMesh.vertices.push_back({ {halfSize,-halfSize, -halfSize},   { 0.75f, twoThird } });		//3 FBR

	//Back
	skyMesh.vertices.push_back({ { halfSize, halfSize, halfSize}, { 0.5f, oneThird } });		//4 BTR
	skyMesh.vertices.push_back({ {-halfSize, halfSize, halfSize}, { 0.25f, oneThird } });		//5 BTL 
	skyMesh.vertices.push_back({ {halfSize, -halfSize, halfSize}, { 0.5f, twoThird } });		//6 BBR
	skyMesh.vertices.push_back({ {-halfSize,-halfSize, halfSize}, { 0.25f, twoThird } });		//7 BBL

	//Right
	skyMesh.vertices.push_back({ { halfSize, halfSize,  -halfSize}, { 0.75f, oneThird } });		//8 FTR
	skyMesh.vertices.push_back({ { halfSize, halfSize,   halfSize}, { 0.5f, oneThird } });		//9 BTR
	skyMesh.vertices.push_back({ { halfSize, -halfSize, -halfSize}, { 0.75f, twoThird } });		//10 FBR
	skyMesh.vertices.push_back({ {halfSize, -halfSize,   halfSize}, { 0.5f, twoThird } });		//11 BBR
	
	//Left
	skyMesh.vertices.push_back({ {-halfSize,  halfSize,  halfSize}, { 0.25f, oneThird } });		//12 BTL
	skyMesh.vertices.push_back({ {-halfSize,  halfSize, -halfSize}, { 0.0f, oneThird } });		//13 FTL 
	skyMesh.vertices.push_back({ {-halfSize, -halfSize,  halfSize}, { 0.25f, twoThird } });		//14 BBL
	skyMesh.vertices.push_back({ {-halfSize, -halfSize, -halfSize}, { 0.0f, twoThird } });		//15 FBL

	//Top
	skyMesh.vertices.push_back({ {-halfSize, halfSize,  halfSize}, { 0.25f, oneThird } });		//16 BTL
	skyMesh.vertices.push_back({ { halfSize, halfSize,  halfSize}, { 0.5f, oneThird } });		//17 BTR
	skyMesh.vertices.push_back({ {-halfSize, halfSize, -halfSize}, { 0.25f, 0.0f } });			//18 FTL
	skyMesh.vertices.push_back({ { halfSize, halfSize, -halfSize}, { 0.5f, 0.0f } });			//19 FTR

	//Bottom																						  
	skyMesh.vertices.push_back({ { halfSize, -halfSize,  halfSize}, { 0.5f, twoThird } });		//20 BBR
	skyMesh.vertices.push_back({ {-halfSize, -halfSize,  halfSize}, { 0.25f, twoThird } });		//21 BBL 
	skyMesh.vertices.push_back({ { halfSize, -halfSize, -halfSize}, { 0.5f, onef } });			//22 FBR
	skyMesh.vertices.push_back({ {-halfSize, -halfSize, -halfSize}, { 0.25f, onef } });			//23 FBL


	CreateSkyBoxIdices(skyMesh.indices);

	return skyMesh;
}

MeshPX FoxEngine::Graphics::MeshBuilder::CreateSkySpherePX(int slices, int rings, float radius)
{
	MeshPX mesh;

	float vertRotation = (FoxMath::Constants::Pi / static_cast<float>(rings - 1));
	float horzRotation = (FoxMath::Constants::TwoPi / static_cast<float>(slices));
	float uStep = 1.0f / static_cast<float>(slices);
	float vStep = 1.0f / static_cast<float>(rings);
	for (int r = 0; r <= rings; ++r)
	{
		float ring = static_cast<float>(r);
		float phi = ring * vertRotation;
		for (int s = 0; s <= slices; ++s)
		{
			float slice = static_cast<float>(s);
			float rotation = slice * horzRotation;

			float u = 1.0f - (uStep * slice);
			float v = vStep * ring;
			mesh.vertices.push_back({ {
					radius * cos(rotation) * sin(phi),
					radius * cos(phi),
					radius * sin(rotation) * sin(phi)
					},
				{u, v} });
		}
	}

	CreatePlaneIndices(mesh.indices, rings, slices);
	return mesh;
}

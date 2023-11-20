#pragma once
#include "Precompiled.h"
#include "CollisionShape.h"

using namespace FoxEngine;
using namespace FoxEngine::Physics;

CollisionShape::~CollisionShape()
{
	ASSERT(mCollisionShape == nullptr, "Collision: must call terminate first");
}

void CollisionShape::InitializeSphere(float radius)
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: is already initialized");
	mCollisionShape = new btSphereShape(radius);
}

void CollisionShape::InitializeBox(const FoxEngine::FoxMath::Vector3& halthExtents)
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: is already initialized");
	mCollisionShape = new btBoxShape(halthExtents);
}

void CollisionShape::InitializeHull(const FoxEngine::FoxMath::Vector3& halthExtents, const FoxEngine::FoxMath::Vector3& origin)
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: is already initialized");
	btConvexHullShape* hullShape = new btConvexHullShape();

	std::vector<FoxEngine::FoxMath::Vector3> points =
	{
		{-halthExtents.x, -halthExtents.y, -halthExtents.z},
		{-halthExtents.x,  halthExtents.y, -halthExtents.z},
		{ halthExtents.x,  halthExtents.y, -halthExtents.z},
		{ halthExtents.x, -halthExtents.y, -halthExtents.z},
		{-halthExtents.x, -halthExtents.y,  halthExtents.z},
		{-halthExtents.x,  halthExtents.y,  halthExtents.z},
		{ halthExtents.x,  halthExtents.y,  halthExtents.z},
		{ halthExtents.x, -halthExtents.y,  halthExtents.z}
	};

	for (auto& point : points)
	{
		hullShape->addPoint(point + origin, false);
	}
	hullShape->recalcLocalAabb();
	mCollisionShape = hullShape;
}

void FoxEngine::Physics::CollisionShape::InitializeEmpty()
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: is already initialized");
	mCollisionShape = new btEmptyShape();
}

void CollisionShape::Terminate()
{
	SafeDelete(mCollisionShape);
}
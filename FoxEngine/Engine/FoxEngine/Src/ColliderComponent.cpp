#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameWorld.h"
//#include "TransformComponent.h"

using namespace FoxEngine;

void FoxEngine::ColliderComponent::Initialize()
{
	//
	//mAABB = AABB({0, 0, 0}, {1, 1, 1});
	//mTransformPtr = GetOwner().GetComponent<TransformComponent>();
}

void FoxEngine::ColliderComponent::Terminate()
{
	mCollisionShape.Terminate();
}

void FoxEngine::ColliderComponent::Deserialize(const rapidjson::Value& value)
{
	if(value.HasMember("Shape"))
	{
		const auto& shapeData = value["Shape"].GetObj();
		if(shapeData.HasMember("Type"))
		{
			const std::string shapeType = shapeData["Type"].GetString();
			if(shapeType == "Empty")
			{
				mCollisionShape.InitializeEmpty();
			}
			else if	(shapeType == "Box")
			{
				const auto& he = shapeData["HalfExtents"].GetArray();
				const float heX = he[0].GetFloat();
				const float heY = he[1].GetFloat();
				const float heZ = he[2].GetFloat();
				mCollisionShape.InitializeBox({heX, heY, heZ});

			}
			else if (shapeType == "Sphere")
			{
				const float radius = shapeData["Radius"].GetFloat();
				mCollisionShape.InitializeSphere(radius);
			}
			else if (shapeType == "Hull")
			{
				const auto& he = shapeData["HalfExtents"].GetArray();
				const float heX = he[0].GetFloat();
				const float heY = he[1].GetFloat();
				const float heZ = he[2].GetFloat();

				const auto& o = shapeData["Origin"].GetArray();
				const float oX = o[0].GetFloat();
				const float oY = o[1].GetFloat();
				const float oZ = o[2].GetFloat();

				mCollisionShape.InitializeHull({ heX, heY, heZ }, {oX, oY, oZ} );
			}
			else
			{
				ASSERT(false, "ColliderComponet: Shape Type %s Unsupported", shapeType);
			}
		}
		else
		{
			ASSERT(false, "ColliderComponet: must have type data");
		}
	}
	else
	{
		ASSERT(false, "ColliderComponet: must have shape data");
	}
}

//void ColliderComponent::DebugUI()
//{
//	Component::DebugUI();
//
//	const FoxMath::AABB aabb = GetAABB();
//	Graphics::SimpleDraw::AddAABB(aabb.center, aabb.extend, FoxEngine::Colors::Blue);
//}
//
//FoxMath::AABB ColliderComponent::GetAABB() const
//{
//	FoxMath::AABB aabb;
//	aabb.center = mTransformPtr->position + mAABB.center;
//	aabb.extend = mAABB.extend;
//
//	return aabb;
//}

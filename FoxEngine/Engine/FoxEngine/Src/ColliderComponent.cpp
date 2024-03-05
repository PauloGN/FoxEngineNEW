#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameWorld.h"
#include "SaveUtil.h"
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

void ColliderComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	rapidjson::Value shapeValue(rapidjson::kObjectType);
	SaveUtil::SaveString("Type", mLoadinData.shapeType.c_str(), doc, shapeValue);

	if (mLoadinData.shapeType == "Empty")
	{
		//nothing
	}
	else if (mLoadinData.shapeType == "Box")
	{
		SaveUtil::SaveVector3("HalfExtents", mLoadinData.param0, doc, shapeValue);
	}
	else if (mLoadinData.shapeType == "Sphere")
	{
		SaveUtil::SaveFloat("Radius", mLoadinData.param0.x, doc, shapeValue);
	}
	else if (mLoadinData.shapeType == "Hull")
	{
		SaveUtil::SaveVector3("HalfExtents", mLoadinData.param0, doc, shapeValue);
		SaveUtil::SaveVector3("Origin", mLoadinData.param1, doc, shapeValue);
	}
	else
	{
		ASSERT(false, "ColliderComponet: Shape Type %s Unsupported", mLoadinData.shapeType);
	}

	//Save component name/data
	componentValue.AddMember("Shape", shapeValue, doc.GetAllocator());
	value.AddMember("ColliderComponent", componentValue, doc.GetAllocator());
}

void FoxEngine::ColliderComponent::Deserialize(const rapidjson::Value& value)
{
	if(value.HasMember("Shape"))
	{
		const auto& shapeData = value["Shape"].GetObj();
		if(shapeData.HasMember("Type"))
		{
			mLoadinData.shapeType = shapeData["Type"].GetString();
			if(mLoadinData.shapeType == "Empty")
			{
				mCollisionShape.InitializeEmpty();
			}
			else if	(mLoadinData.shapeType == "Box")
			{
				const auto& he = shapeData["HalfExtents"].GetArray();
				const float heX = he[0].GetFloat();
				const float heY = he[1].GetFloat();
				const float heZ = he[2].GetFloat();

				mLoadinData.param0 = { heX, heY, heZ };
				mCollisionShape.InitializeBox(mLoadinData.param0);

			}
			else if (mLoadinData.shapeType == "Sphere")
			{
				const float radius = shapeData["Radius"].GetFloat();
				mLoadinData.param0.x = radius;
				mCollisionShape.InitializeSphere(mLoadinData.param0.x);
			}
			else if (mLoadinData.shapeType == "Hull")
			{
				const auto& he = shapeData["HalfExtents"].GetArray();
				const float heX = he[0].GetFloat();
				const float heY = he[1].GetFloat();
				const float heZ = he[2].GetFloat();

				const auto& o = shapeData["Origin"].GetArray();
				const float oX = o[0].GetFloat();
				const float oY = o[1].GetFloat();
				const float oZ = o[2].GetFloat();

				mLoadinData.param0 = { heX, heY, heZ };
				mLoadinData.param1 = { oX, oY, oZ };

				mCollisionShape.InitializeHull(mLoadinData.param0, mLoadinData.param1);
			}
			else
			{
				ASSERT(false, "ColliderComponet: Shape Type %s Unsupported", mLoadinData.shapeType);
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

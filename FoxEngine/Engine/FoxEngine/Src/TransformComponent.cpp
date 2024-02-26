#include "Precompiled.h"
#include "TransformComponent.h"
#include "SaveUtil.h"

using namespace FoxEngine;
using namespace FoxEngine::SaveUtil;

void TransformComponent::DebugUI()
{
	Graphics::SimpleDraw::AddTransform(GetMatrix4());
}

void TransformComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	//for anything that is not just one value
	rapidjson::Value componentValue(rapidjson::kObjectType);

	//Position
	SaveVector3("Position", position, doc, componentValue);
	//Rotation
	rapidjson::Value rotArray(rapidjson::kArrayType);
	Vector3 Euler = FoxMath::Vector3::QuaternionToEuler(rotation);
	SaveVector3("Rotation", Euler, doc, componentValue);
	//Scale
	SaveVector3("Scale", scale, doc, componentValue);
	//Save component name/data 
	value.AddMember("TransformComponent", componentValue, doc.GetAllocator());
}

void TransformComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("Position"))
	{
		const auto& pos = value["Position"].GetArray();
		position.x = pos[0].GetFloat();
		position.y = pos[1].GetFloat();
		position.z = pos[2].GetFloat();
	}

	if (value.HasMember("Rotation"))
	{
		const auto& rot = value["Rotation"].GetArray();
		const float x = rot[0].GetFloat() * FoxMath::Constants::DegToRad;
		const float y = rot[1].GetFloat() * FoxMath::Constants::DegToRad;
		const float z = rot[2].GetFloat() * FoxMath::Constants::DegToRad;
		//const float w = rot[3].GetFloat() * FoxMath::Constants::DegToRad;

		rotation = FoxMath::Quaternion::EulerToQuaternion({x, y, z});
	}

	if (value.HasMember("Scale"))
	{
		const auto& s = value["Scale"].GetArray();
		scale.x = s[0].GetFloat();
		scale.y = s[1].GetFloat();
		scale.z = s[2].GetFloat();
	}
}

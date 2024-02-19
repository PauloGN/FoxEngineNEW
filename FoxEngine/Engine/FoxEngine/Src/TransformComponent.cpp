#include "Precompiled.h"
#include "TransformComponent.h"

using namespace FoxEngine;

void TransformComponent::DebugUI()
{
	Graphics::SimpleDraw::AddTransform(GetMatrix4());
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
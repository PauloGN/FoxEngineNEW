#pragma once

namespace FoxEngine
{
	namespace SaveUtil
	{
		using rjDocument = rapidjson::Document;
		using rjValue = rapidjson::Value;
		using strArrayData = const std::vector<std::string>;

		void SaveVector3(const char* key, const FoxMath::Vector3& v, rjDocument& doc, rjValue& member);
		void SaveColor(const char* key, const Color& c, rjDocument& doc, rjValue& member);
		void SaveQuaternion(const char* key, const FoxMath::Quaternion& q, rjDocument& doc, rjValue& member);
		void SaveFloat(const char* key, float& value, rjDocument& doc, rjValue& member);
		void SaveString(const char* key, const char* value, rjDocument& doc, rjValue& member);
		void SaveStringArray(const char* key, strArrayData& values, rjDocument& doc, rjValue& member);
		void SaveInt(const char* key, int& value, rjDocument& doc, rjValue& member);
		void SaveBool(const char* key, bool& value, rjDocument& doc, rjValue& member);
	}
}

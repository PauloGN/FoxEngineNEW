#include "Precompiled.h"
#include "SaveUtil.h"

using namespace FoxEngine;
using namespace FoxEngine::SaveUtil;

void SaveUtil::SaveVector3(const char* key, const FoxMath::Vector3& v, rjDocument& doc, rjValue& member)
{
	rapidjson::Value vecArray(rapidjson::kArrayType);
	vecArray.PushBack(v.x, doc.GetAllocator());
	vecArray.PushBack(v.y, doc.GetAllocator());
	vecArray.PushBack(v.z, doc.GetAllocator());
	member.AddMember(rapidjson::StringRef(key), vecArray, doc.GetAllocator());
}
void SaveUtil::SaveQuaternion(const char* key, const FoxMath::Quaternion& q, rjDocument& doc, rjValue& member)
{
	rapidjson::Value quatArray(rapidjson::kArrayType);
	quatArray.PushBack(q.x, doc.GetAllocator());
	quatArray.PushBack(q.y, doc.GetAllocator());
	quatArray.PushBack(q.z, doc.GetAllocator());
	quatArray.PushBack(q.w, doc.GetAllocator());
	member.AddMember(rapidjson::StringRef(key), quatArray, doc.GetAllocator());
}

void SaveUtil::SaveFloat(const char* key, float& value, rjDocument& doc, rjValue& member)
{
	member.AddMember(rapidjson::StringRef(key), value, doc.GetAllocator());
}

void SaveUtil::SaveString(const char* key, const char* value, rjDocument& doc, rjValue& member)
{
	member.AddMember(rapidjson::StringRef(key), rapidjson::StringRef(value), doc.GetAllocator());
}

void SaveUtil::SaveStringArray(const char* key, strArrayData& values, rjDocument& doc, rjValue& member)
{
	rapidjson::Value strArray(rapidjson::kArrayType);

	for (const std::string& v : values)
	{
		strArray.PushBack(rapidjson::StringRef(v.c_str()), doc.GetAllocator());
	}
	member.AddMember(rapidjson::StringRef(key), strArray, doc.GetAllocator());
}

void SaveUtil::SaveInt(const char* key, int& value, rjDocument& doc, rjValue& member)
{
	member.AddMember(rapidjson::StringRef(key), value, doc.GetAllocator());
}

void SaveUtil::SaveBool(const char* key, bool& value, rjDocument& doc, rjValue& member)
{
	member.AddMember(rapidjson::StringRef(key), value, doc.GetAllocator());
}
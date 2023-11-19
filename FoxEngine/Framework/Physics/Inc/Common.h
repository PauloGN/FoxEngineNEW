#pragma once

//Framework headers
#include <Math/Inc/FoxMath.h>
#include <Core/Inc/Core.h>
#include <Graphics/Inc/Transform.h>
#include <Graphics/Inc/Colors.h>

//Bullet files
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/btBulletDynamicsCommon.h>

//Helper functions
template<class T>
inline void SafeDelete(T*& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

inline btTransform ConvertTobtTransform(const FoxEngine::Graphics::Transform& trans)
{
	return btTransform(trans.rotation, trans.position);
}
#include "Precompiled.h"
#include "PhysicsDebugDrawer.h"

#include <Graphics/Inc/Colors.h>
#include <Graphics/Inc/SimpleDraw.h>

using namespace FoxEngine;
using namespace FoxEngine::Graphics;
using namespace FoxEngine::Physics;

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	SimpleDraw::AddLine(from, to, Color(color.x(), color.y(), color.z(), 1.f));
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3& pointOn, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	//NOTHING HERE ***
}

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("[Physics Warning] %s", warningString);
}

void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	//NOTHING HERE ***
}

void PhysicsDebugDrawer::setDebugMode(int debugMode)
{
	mDebugMode = debugMode;
}

int PhysicsDebugDrawer::getDebugMode() const
{
	return mDebugMode;
}
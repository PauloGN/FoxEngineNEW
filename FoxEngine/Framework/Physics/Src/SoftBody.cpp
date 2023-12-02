#include "Precompiled.h"
#include "SoftBody.h"
#include "PhysicsWorld.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;
using namespace FoxEngine::Physics;

SoftBody::~SoftBody()
{
	ASSERT(mSoftBody == nullptr, "SoftBody: terminate must be called fisrt");
}

void SoftBody::Initialize(Graphics::Mesh& mesh, float mass, const std::vector<int>& fixedNodeIndices)
{
	mMesh = &mesh;
	mSoftBody = PhysicsWorld::Get()->CreateSoftBody(mesh.vertices.size());

	btSoftBody::Material* pm = mSoftBody->appendMaterial();
	pm->m_kAST = 1.f;
	mSoftBody->m_cfg.kMT = 0.2f;
	for (size_t i = 0; i < mesh.vertices.size(); ++i)
	{
		mSoftBody->m_nodes[i].m_x = mesh.vertices[i].position;
	}
	for (size_t i = 0; i < mesh.indices.size(); i += 6)
	{
		int index0 = mesh.indices[i];
		int index1 = mesh.indices[i + 4];
		int index2 = mesh.indices[i + 5];
		int index3 = mesh.indices[i + 2];

		mSoftBody->appendLink(index0, index1);
		mSoftBody->appendLink(index1, index2);
		mSoftBody->appendLink(index2, index3);
		mSoftBody->appendLink(index3, index1);
	}
	for (auto fixedNode : fixedNodeIndices)
	{
		mSoftBody->m_nodes[fixedNode].m_im = 0.f;
	}
	PhysicsWorld::Get()->Register(this);
}

void SoftBody::Terminate()
{
	PhysicsWorld::Get()->Unregister(this);
	SafeDelete(mSoftBody);
}

void SoftBody::Update()
{
	for (size_t i = 0; i < mMesh->vertices.size(); ++i)
	{
		mMesh->vertices[i].position = mSoftBody->m_nodes[i].m_x;
	}
}
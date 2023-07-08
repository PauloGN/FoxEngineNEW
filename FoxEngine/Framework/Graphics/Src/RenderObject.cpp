#include  "Precompiled.h"
#include  "RenderObject.h"

void FoxEngine::Graphics::RenderObject::Terminate()
{
	meshBuffer.Terminate();
	mDiffuseTexture.Terminate();
}

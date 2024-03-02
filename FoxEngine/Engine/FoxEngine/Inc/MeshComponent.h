#pragma once

#include "Component.h"

namespace FoxEngine
{
	class MeshComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Mesh)

		void Initialize() override;
		void Terminate() override;
		void Deserialize(const rapidjson::Value& value) override;

		bool CastShadow() const { return mCastShadow; }
		const Graphics::Model& GetModel() const { return mModel; }

	private:

		bool mCastShadow = true;
		Graphics::Model mModel;

	};
}

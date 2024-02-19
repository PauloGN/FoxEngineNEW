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

		const Graphics::Model& GetModel() const { return mModel; }

	private:
		Graphics::Model mModel;

	};
}

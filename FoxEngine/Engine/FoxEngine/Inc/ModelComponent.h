#pragma once
#include "Component.h"

namespace FoxEngine
{
	class ModelComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Model)

		void Initialize() override;
		void Terminate() override;

		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(const rapidjson::Value& value) override;

		bool CastShadow() const { return mCastShadow; }

		Graphics::ModelId GetModelId()const { return mModelId; }
		const Graphics::Model& GetModel()const;

	private:

		std::string mFileName;
		Graphics::ModelId mModelId;

		bool mCastShadow = true;
		using Animations = std::vector<std::string>;
		Animations mAnimationFileNames;
	};
}
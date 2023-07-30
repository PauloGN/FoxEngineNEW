#pragma once

namespace FoxEngine::Graphics
{
	struct Model;

	namespace ModelIO
	{
		void SaveModel(std::filesystem::path filePath, const Model& model);
		void loadModel(std::filesystem::path filePath, Model& model);

		void SaveMaterial(std::filesystem::path filePath, const Model& model);
		void loadMaterial(std::filesystem::path filePath, Model& model);
	}
}
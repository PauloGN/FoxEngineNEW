#pragma once

namespace FoxEngine::Graphics
{
	class ConstantBuffer
	{
	public:
		ConstantBuffer() = default;
		virtual ~ConstantBuffer();

		void Initialize(uint32_t bufferSize);
		void Terminate();

		void Update(const void* data);

		//Bind the vertex buffer to vertex and pixel shader
		void BindVS(uint32_t slot)const;
		void BindPS(uint32_t slot);

	private:

		ID3D11Buffer* mConstantBuffer = nullptr;
	};
}
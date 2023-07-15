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

		void Update(const void* data)const;

		//Bind the vertex buffer to vertex and pixel shader
		void BindVS(uint32_t slot)const;
		void BindPS(uint32_t slot)const;

	private:

		ID3D11Buffer* mConstantBuffer = nullptr;
	};

	template <class DataType>
	class TypedConstantBuffer final : public ConstantBuffer 
	{
	public:

		void Initialize()
		{
			static_assert(sizeof(DataType) % 16 == 0, "Data type must be 16 byte aligned");
			ConstantBuffer::Initialize(sizeof(DataType));
		}

		void Update(const DataType& data) const
		{
			ConstantBuffer::Update(&data);
		}
	};
	//[][][][]
	//[][][][]
	//[][][][]
	//[][][][]
}
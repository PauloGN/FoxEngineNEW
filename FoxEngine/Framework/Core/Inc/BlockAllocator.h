#pragma once

namespace FoxEngine::Core
{
	/**
	 * \This is a basic block allocator witch performs a single
	 * allocation and deallocation on constructions/destruction. Blocks are
	 * assigned and recycled to the user
	 *
	 * \usage:
	 *
	 *	BlockAllocator blockAllocator(10, 10);
	 *	void* memory = blockAllocator.Allocate();
	 *	....
	 *	blockAllocator.Free(memory);
	 */
	class BlockAllocator 
	{
	public:

		//Rule of 6
		BlockAllocator(const char* name, size_t blockSize, size_t capacity);
		virtual ~BlockAllocator();
		BlockAllocator(const BlockAllocator&) = delete;
		BlockAllocator(const BlockAllocator&&) = delete;
		BlockAllocator& operator=(const BlockAllocator&) = delete;
		BlockAllocator& operator=(const BlockAllocator&&) = delete;

		void* Allocate();
		void Free(void* ptr);

	private:

		std::vector<void*> mFreeBlocks;
		std::string mName;

		//
		void* mData = nullptr;
		size_t mBlockSize = 0;	//how many bytes for a single block
		size_t mCapacity = 0;	//how many blocks
		size_t mBlockAllocatedCurrent = 0;
		size_t mBlockAllocatedTotal = 0;
		size_t mBlocksFreed = 0;
		size_t mBlocksHighest = 0;
	};
}
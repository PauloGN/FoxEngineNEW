#include "Precompiled.h"
#include "BlockAllocator.h"
#include "DebugUtil.h"

using namespace FoxEngine;
using namespace FoxEngine::Core;

FoxEngine::Core::BlockAllocator::BlockAllocator(const char* name, size_t blockSize, size_t capacity) :
	mName(name), mBlockSize(blockSize), mCapacity(capacity), mFreeBlocks(capacity)
{

	ASSERT(blockSize > 0, "BlockAllocator: invalid block size");
	ASSERT(capacity > 0, "BlockAllocator: invalid capacity");

	/**
	 * malloc gives me a void pointer back but I have to say what is the size of it
	 *
	 */
	mData = std::malloc(blockSize * capacity);


	for (size_t i = 0; i < capacity; ++i)
	{
		mFreeBlocks[i] = static_cast<uint8_t*>(mData) + (i * mBlockSize);
	}

	LOG("%s allocated %zu blocks, blocks size: %zu", mName.c_str(), capacity, blockSize);
}

FoxEngine::Core::BlockAllocator::~BlockAllocator()
{

	ASSERT(mBlockAllocatedTotal == mBlocksFreed, "BlockAllocator: not all blocks are freed");
	std::free(mData);

	LOG("%s destructed, Allocated: %zu, Freed: %zu, Highest: %zu", mName.c_str(), mBlockAllocatedCurrent, mBlocksFreed, mBlocksHighest);

}

void* FoxEngine::Core::BlockAllocator::Allocate()
{
	if (mFreeBlocks.empty())
	{
		LOG("%s no free blocks available", mName.c_str());
		return nullptr;
	}

	void* freeBlock = mFreeBlocks.back();
	mFreeBlocks.pop_back();

	++mBlockAllocatedTotal;
	++mBlockAllocatedCurrent;
	mBlocksHighest = std::max(mBlocksHighest, mBlockAllocatedCurrent);

	LOG("%s allocated blocks at %p, Allocated: %zu, HighestCount: %zu", mName.c_str(), freeBlock, mBlockAllocatedCurrent, mBlocksHighest);

	return freeBlock;
}

void FoxEngine::Core::BlockAllocator::Free(void* ptr)
{
	if (ptr == nullptr)
	{
		return;
	}

	const uint8_t* start = static_cast<uint8_t*>(mData);
	const uint8_t* end = static_cast<uint8_t*>(mData) + (mBlockSize * mCapacity);
	const uint8_t* current = static_cast<uint8_t*>(ptr);
	const auto diff = current - start;
	ASSERT(current >= start && current < end && static_cast<size_t>(diff)% mBlockSize == 0, "BlockAllocator: invalid adress being freed!");

	LOG("%s free %p", mName.c_str(), ptr);

	--mBlockAllocatedCurrent;
	++mBlocksFreed;
	mFreeBlocks.emplace_back(ptr);
}

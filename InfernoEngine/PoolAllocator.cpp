#include "PoolAllocator.h"
namespace InfernoEngine {

    Chunk* PoolAllocator::allocateBlock(size_t chunkSize)
    {
        cout << "\nAllocating block (" << mChunksPerBlock << " chunks):\n\n";

        size_t blockSize = mChunksPerBlock * chunkSize;

        // The first chunk of the new block.
        Chunk* blockBegin = reinterpret_cast<Chunk*>(malloc(blockSize));

        Chunk* chunk = blockBegin;

        for (int i = 0; i < mChunksPerBlock - 1; ++i) {
            // Chains all the chunks together 
            chunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + chunkSize);
            chunk = chunk->next;
        }
        // last element in the pool points to the nullptr
        // if calls new again will create another block
        chunk->next = nullptr;
        return blockBegin;
    }


    void* PoolAllocator::allocate(size_t size)
    {
        if (mAlloc == nullptr) {
            mAlloc = allocateBlock(size);
        }

        // The return value is the current position of
        // the allocation pointer:

        Chunk* freeChunk = mAlloc;

        // increase the allocation pointer to the next chunk.
        
        // When no chunks left, the `mAlloc` will be set to `nullptr`, and
        // this will cause allocation of a new block on the next call
        // to `allocate()`.

        mAlloc = mAlloc->next;

        return freeChunk;
    }

    void PoolAllocator::deallocate(void* chunk, size_t size)
    {
        // The freed chunk's next pointer points to the
        // current allocation pointer:

        reinterpret_cast<Chunk*>(chunk)->next = mAlloc;

        // And the allocation pointer is now set
        // to the returned (free) chunk:

        mAlloc = reinterpret_cast<Chunk*>(chunk);
    }

}
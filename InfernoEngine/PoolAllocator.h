#include <iostream>
using namespace std;

namespace InfernoEngine {

    struct Chunk {
       /**
       * Stores the pointer to the next object in the pool.
       * Does not need the size since it the size
       * is predefined by the sizeof(Object) when calling new
       * when initializing the first element of that type in the pool
       */
        Chunk* next;
    };

    class PoolAllocator {
    public:
        PoolAllocator(size_t chunksPerBlock)
            : mChunksPerBlock(chunksPerBlock) {}

        void* allocate(size_t size);
        void deallocate(void* chunk, size_t size);

    private:
        /**
         * Number of chunks per larger block.
         */
        size_t mChunksPerBlock;

        /**
         * Allocation pointer.
         */
        Chunk* mAlloc = nullptr;

        /**
         * Allocates a larger block (pool) for chunks.
         * also returns the beginning address of the pool
         */
        Chunk* allocateBlock(size_t chunkSize);
    };
}
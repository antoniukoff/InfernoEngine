#pragma once

namespace Vladgine
{
    class StackAllocator {
    public:
        StackAllocator(size_t size)
            : m_size(size), m_top(0), m_data(new char[size]) {}

        ~StackAllocator() {
            delete[] m_data;
        }

        // Allocate memory from the stack
        void* alloc(size_t size) {
            if (m_top + size > m_size) {
                // Not enough space
                return nullptr;
            }
            void* ptr = m_data + m_top;
            m_top += size;
            return ptr;
        }

        // Get the current position of the top of the stack (used for deallocation)
        size_t getMarker() const {
            return m_top;
        }

        // Deallocate back to a previous marker
        void freeToMarker(size_t marker) {
            if (marker <= m_size) {
                m_top = marker;
            }
            // else: error handling
        }

        // Clear the entire stack
        void clear() {
            m_top = 0;
        }

    private:
        size_t m_size;  // Total size of the memory block
        size_t m_top;   // Current top of the stack
        char* m_data;   // Actual data block
    };
}

#pragma once
#include <memory>

size_t memoryAllocated = 0;
size_t memoryDeleted = 0;

void* operator new(size_t memorySize){
	memoryAllocated += memorySize;
	return std::malloc(memorySize);
}

void operator delete(void* memoryLocation, size_t memorySize) {
	memoryDeleted += memorySize;
	
	std::free(memoryLocation);
}

void printMemory() {
	printf("Memory leaks: %d\n", memoryAllocated - memoryDeleted);
}
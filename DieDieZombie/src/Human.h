#pragma once
#include "Agent.h"
#include <InfernoEngine/PoolAllocator.h>
class Human :public Agent
{
public:
	Human();
	virtual ~Human();

	void init(float speed, glm::vec2 position);

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombie,
		float deltaTime) override;

	static InfernoEngine::PoolAllocator allocator;

	static void* operator new (size_t size) {
		return allocator.allocate(size);
	}

	static void operator delete(void* ptr, size_t size) {
		return allocator.deallocate(ptr, size);
	}
		
private:
	int _frames;
};


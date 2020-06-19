#pragma once
#include "__vector.h"
using namespace std;

typedef void(*action)(void* obj);

template <class T>
class memorypool
{
private:
	bool				_isPointer;

	size_t				AllSize;

	Vector<T*>			vecPool;
	Vector<T*>			vecUsage;

public:
	memorypool() = delete;
	memorypool(size_t _size);

	memorypool(const memorypool& other) = delete;
	memorypool operator= (const memorypool& other) = delete;

	~memorypool();

public:
	T* pickUpPool();
	void create(__in T* _value);
	void release(__in const size_t _at);
	void release(__in const void* _iter);

	void range_loop(void* fncMemory);

public:
	Vector<T*>& getUsage() { return vecUsage; }
};

#include "memorypool.inl"
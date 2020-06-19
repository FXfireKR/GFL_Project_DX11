#pragma once
#include "memorypool.h"

template<class T>
inline memorypool<T>::memorypool(size_t _size)
	: AllSize(_size)
{
	vecPool.resize(AllSize);
	vecUsage.reserve(AllSize);

	for (size_t i = 0; i < vecPool.size(); ++i)
	{
		vecPool[i] = new T;
	}
}

template<class T>
inline memorypool<T>::~memorypool()
{
	for (T* iter : vecPool)
		SAFE_DELETE(iter);

	for (T* iter : vecUsage)
		SAFE_DELETE(iter);

	vecPool.clear();
	vecUsage.clear();
}

template<class T>
inline T* memorypool<T>::pickUpPool()
{
	if (vecPool.size() < 1)
		return nullptr;

	else
		return (vecPool.pop_back());
}

template<class T>
inline void memorypool<T>::create(T* _value)
{
	vecUsage.push_back(_value);
}

template<class T>
inline void memorypool<T>::release(const size_t _at)
{
	if (vecUsage.size() > _at) {
		T* _temp = vecUsage[_at];
		vecUsage.erase(_at);
		vecPool.push_back(_temp);
	}
}

template<class T>
inline void memorypool<T>::release(const void * _iter)
{
	if (vecUsage.size() > 0)
		vecPool.push_back(vecUsage.erase(_iter));
}
template<class T>
inline void memorypool<T>::range_loop(void * fncMemory)
{
	if (fncMemory != nullptr)
	{
		action fnc_Call = (action)fncMemory;

		for (T* iter : vecUsage)
			fnc_Call(iter);
	}
}

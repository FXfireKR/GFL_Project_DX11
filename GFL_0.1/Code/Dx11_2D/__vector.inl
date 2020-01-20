#include "util.h"		//	유틸리티 헤더
#include "__vector.h"

template<typename T>
inline Vector<T>::Vector()
	: pFront(nullptr), pBack(nullptr), data(nullptr), Capacity(0), Size(0)
{
	_pointer = typeid(T).raw_name()[1] == 'P' ? true : false;
}

template<typename T>
inline Vector<T>::Vector(size_t _capacity)
	: pFront(nullptr), pBack(nullptr), data(nullptr), Capacity(_capacity), Size(0)
{
	if (Capacity > 0)
		NewCollocate(Capacity);
	_pointer = typeid(T).raw_name()[1] == 'P' ? true : false;
}

template<typename T>
inline Vector<T>::Vector(size_t _capacity, T _value)
	: pFront(nullptr), pBack(nullptr), data(nullptr), Capacity(_capacity), Size(_capacity)
{
	if (Capacity > 0)
	{
		NewCollocate(Capacity);

		for (_capacity; _capacity > 0; --_capacity)
			memcpy(&data[_capacity - 1], &_value, sizeof(T));
	}

	_pointer = typeid(T).raw_name()[1] == 'P' ? true : false;
}

template<typename T>
inline Vector<T>::Vector(size_t _capacity, T& _value)
	: pFront(nullptr), pBack(nullptr), data(nullptr), Capacity(_capacity), Size(_capacity)
{
	if (Capacity > 0)
	{
		NewCollocate(Capacity);

		for (_capacity; _capacity > 0; --_capacity)
			memcpy(&data[_capacity - 1], _value, sizeof(T));
	}

	_pointer = typeid(T).raw_name()[1] == 'P' ? true : false;
}

template<typename T>
inline Vector<T>::~Vector()
{
	if (data != nullptr) {
		FREE(data);

		pFront = pBack = nullptr;

		Size = 0;
		Capacity = 0;
	}
}

template<typename T>
inline void Vector<T>::NewCollocate(size_t _capacity, bool back)
{
	if (data != nullptr) {
		T* newMemory = NEW(T, _capacity);
		if (back)
			for (int i = 0; i < Size; ++i)
				memcpy(&newMemory[i], &data[i], sizeof(T));
		else
			for (int i = static_cast<int>(Size); i > 0; --i)
				memcpy(&newMemory[i], &data[i - 1], sizeof(T));

		FREE(data);

		data = newMemory;
		newMemory = nullptr;
	}

	else
		data = NEW(T, _capacity);

	if (Size > 0) {
		pFront = &data[0];
		pBack = &data[Size - 1];
	}
}


template<typename T>
inline T const * Vector<T>::push_back(const T& _value)
{
	//	새로 할당해야할 경우
	if (Size == Capacity)
		NewCollocate(++Capacity);

	if (Size == 0)
		pFront = &data[Size];

	data[Size] = _value;
	pBack = &data[Size];

	++Size;

	return pBack;
}

template<typename T>
inline T const * Vector<T>::push_front(const T & _value)
{
	//	새로 할당해야할 경우
	if (Size == Capacity)
		NewCollocate(++Capacity, false);

	data[0] = _value;
	pBack = &data[Size];

	++Size;

	return pFront;
}

template<typename T>
inline T Vector<T>::pop_back()
{
	T erased = data[Size - 1];

	--Size;
	pBack = &data[Size - 1];

	return erased;
}

template<typename T>
inline T Vector<T>::pop_front()
{
	--Size;

	T erased = data[0];

	for (int i = 0; i < Size; ++i)
		memcpy(&data[i], &data[i + 1], sizeof(T));
	pBack = &data[Size - 1];

	return erased;
}

template<typename T>
inline void Vector<T>::erase(const size_t pos)
{
	assert((pos < Size) && "Bad Access of Operator [], Vector Class");

	if (Size > pos)
	{
		if (pos == 0)
			this->pop_front();

		else if (pos == Size - 1)
			this->pop_back();

		else
		{
			for (size_t i = pos + 1; i < Size; ++i)
				memcpy(&data[i - 1], &data[i], sizeof(T));

			--Size;

			pBack = &data[Size - 1];
		}
	}

}

template<typename T>
inline T Vector<T>::erase(const void * _memAdr)
{
	if (Size > 0)
	{
		T* tempData = pFront;
		const void* frnt = *pFront;

		if (frnt != _memAdr)
		{
			if (*pBack != _memAdr)
			{
				++tempData;
				frnt = *tempData;

				size_t pos = 1;

				while (frnt != _memAdr)
				{
					++tempData;
					++pos;
					frnt = *tempData;
				}

				T erased = data[pos];
				erase(pos);
				return erased;
			}

			else
				return this->pop_back();

		}
		else
			return this->pop_front();
	}

	return T();
}

template<typename T>
inline void Vector<T>::reserve(size_t _capacity)
{
	if (_capacity > Capacity)
	{
		NewCollocate(_capacity);
		Capacity = _capacity;
	}
}

template<typename T>
inline void Vector<T>::resize(size_t _capacity, T _value)
{
	if (_capacity > Capacity)
	{
		NewCollocate(_capacity);
		Size = Capacity = _capacity;

		//if(_value != nullptr && _value != NULL)
		for (_capacity; _capacity > 0; --_capacity)
			memcpy(&data[_capacity - 1], &_value, sizeof(T));


		pFront = &data[0];
		pBack = &data[Size - 1];
	}
}

template<typename T>
inline T & Vector<T>::at(size_t pos)
{
	if (pos < Size && pos > -1)
		return data[pos];

	//	Error This code, Over Access Array !
}

template<typename T>
inline T & Vector<T>::operator[](size_t pos)
{
	assert((pos < Size) && "Bad Access of Operator [], Vector Class");
	return data[pos];

	//	Error This code, Over Access Array !
}
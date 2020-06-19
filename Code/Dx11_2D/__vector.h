#pragma once

template <typename T>
class Vector
{
private:
	T*			pFront;				//	데이터의 최 전방
	T*			pBack;				//	데이터의 최 후방

	T*			data;				//	데이터 저장

	size_t		Size;				//	실질적인 T의 갯수
	size_t		Capacity;			//	예약된 T에 대한 갯수

	bool		_pointer;			//	포인터인가

public:
	typedef T const*			iterator_const;
	typedef T*					iterator;

public:		//	생성 - 소멸자

	Vector();												//	기본 생성자
	Vector(size_t _capacity);								//	생성자 (메모리크기)
	Vector(size_t _capacity, T _value);						//	생성자 (메모리크기, 초기화할 값)
	Vector(size_t _capacity, T& _value);					//	생성자 (메모리크기, 초기화할 값)

	~Vector();												//	소멸자

	Vector(const Vector& other) = delete;					//	복사 생성자 = delete
	Vector operator= (const Vector& other) = delete;		//	대입 연산자 = delete


private:	//	내부기능 함수

	//	size > capacity 인경우에, 메모리 재 할당
	void NewCollocate(size_t _capacity, bool back = true);


public:		//	inl 인라인 함수부

	T const* push_back(const T& _value);						//	최후방 원소삽입
	T const* push_front(const T& _value);						//	최전방 원소삽입

	T pop_back();											//	최후방 원소제거
	T pop_front();											//	최전방 원소제거

	void erase(const size_t pos);								//	원소 번지수로 찾아서 제거
	T erase(const void* _memAdr);							//	원소 메모리를 찾아서 제거

	void reserve(size_t _capacity);								//	메모리 예약
	void resize(size_t _capacity, T _value = NULL);				//	메모리 확장, 초기화

	T& at(size_t pos);											//	원소 접근

public:
	T& operator[] (size_t pos);									//	operator []

public:		//	header 인라인 함수부

	inline T front() const { return *pFront; }					//	최전방 값 반환
	inline T back() const { return *pBack; }					//	최후방 값 반환

	inline iterator begin() { return (&data[0]); }				//	최전방 포인터
	inline iterator end() { return (&data[Size]); }				//	최후방 포인터

	inline iterator_const cbegin() { return (&data[0]); }		//	최전방 포인터 const
	inline iterator_const cend() { return (&data[Size]); }		//	최후방 포인터 const

	inline void clear() { Size = 0; }							//	벡터를 비웁니다.

	inline const bool empty() const								//	벡터가 비었는가
	{
		return Size > 0 ? true : false;
	}

	inline const size_t size() const { return Size; }			//	실질적인 Data 크기
	inline const size_t capacity() const { return Capacity; }	//	메모리 예약된 Data 크기
};


//	Load Teamplate Function
#include "__vector.inl"
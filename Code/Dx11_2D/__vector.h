#pragma once

template <typename T>
class Vector
{
private:
	T*			pFront;				//	�������� �� ����
	T*			pBack;				//	�������� �� �Ĺ�

	T*			data;				//	������ ����

	size_t		Size;				//	�������� T�� ����
	size_t		Capacity;			//	����� T�� ���� ����

	bool		_pointer;			//	�������ΰ�

public:
	typedef T const*			iterator_const;
	typedef T*					iterator;

public:		//	���� - �Ҹ���

	Vector();												//	�⺻ ������
	Vector(size_t _capacity);								//	������ (�޸�ũ��)
	Vector(size_t _capacity, T _value);						//	������ (�޸�ũ��, �ʱ�ȭ�� ��)
	Vector(size_t _capacity, T& _value);					//	������ (�޸�ũ��, �ʱ�ȭ�� ��)

	~Vector();												//	�Ҹ���

	Vector(const Vector& other) = delete;					//	���� ������ = delete
	Vector operator= (const Vector& other) = delete;		//	���� ������ = delete


private:	//	���α�� �Լ�

	//	size > capacity �ΰ�쿡, �޸� �� �Ҵ�
	void NewCollocate(size_t _capacity, bool back = true);


public:		//	inl �ζ��� �Լ���

	T const* push_back(const T& _value);						//	���Ĺ� ���һ���
	T const* push_front(const T& _value);						//	������ ���һ���

	T pop_back();											//	���Ĺ� ��������
	T pop_front();											//	������ ��������

	void erase(const size_t pos);								//	���� �������� ã�Ƽ� ����
	T erase(const void* _memAdr);							//	���� �޸𸮸� ã�Ƽ� ����

	void reserve(size_t _capacity);								//	�޸� ����
	void resize(size_t _capacity, T _value = NULL);				//	�޸� Ȯ��, �ʱ�ȭ

	T& at(size_t pos);											//	���� ����

public:
	T& operator[] (size_t pos);									//	operator []

public:		//	header �ζ��� �Լ���

	inline T front() const { return *pFront; }					//	������ �� ��ȯ
	inline T back() const { return *pBack; }					//	���Ĺ� �� ��ȯ

	inline iterator begin() { return (&data[0]); }				//	������ ������
	inline iterator end() { return (&data[Size]); }				//	���Ĺ� ������

	inline iterator_const cbegin() { return (&data[0]); }		//	������ ������ const
	inline iterator_const cend() { return (&data[Size]); }		//	���Ĺ� ������ const

	inline void clear() { Size = 0; }							//	���͸� ���ϴ�.

	inline const bool empty() const								//	���Ͱ� ����°�
	{
		return Size > 0 ? true : false;
	}

	inline const size_t size() const { return Size; }			//	�������� Data ũ��
	inline const size_t capacity() const { return Capacity; }	//	�޸� ����� Data ũ��
};


//	Load Teamplate Function
#include "__vector.inl"
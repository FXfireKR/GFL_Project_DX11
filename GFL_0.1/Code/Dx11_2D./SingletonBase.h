#pragma once
/*
SingletonBase

�̱��� ���� : ���� ��ü ����
- �ν��Ͻ��� ȣ�� �� ó�� ȣ��(�ν��Ͻ��� ���� �Ǳ� ��)�� �Ǹ� �ν��Ͻ��� �����ؼ�
��ȯ�Ѵ�.
- �ν��Ͻ��� ���� �� ���� ȣ�� �ϰ� �Ǹ� ������ ���� �� �ν��Ͻ��� ��ȯ�Ѵ�.

���ø�(Template, ����) : �Լ� �Ǵ� Ŭ������ ������ ���� Ʋ
- �������� ���� �ڷ����� ���� ������ ���ø��� ����Ͽ� �ϰ� ��ɵ��� �����Ѵ�.
- �Լ� �Ǵ� Ŭ���� �ۼ��� ���ø��� ��� �� �ڷ����� �������ش�.
*/

template <typename T> // template<class T>
class SingletonBase
{
protected:
	static T* instance;	// �̱��� �ν��Ͻ� ����

	SingletonBase() {};
	~SingletonBase() {};

public:
	static T* GetInstance();	// �̱��� �ν��Ͻ� ��������
	void ReleaseInstance();		// �̱��� �ν��Ͻ� ����
};

// �̱��� �ʱ�ȭ
template<typename T>
T* SingletonBase<T>::instance = NULL;

// �̱��� �ν��Ͻ� ��������
template<typename T>
inline T* SingletonBase<T>::GetInstance()
{
	// �̱��� �ν��Ͻ��� �����Ǿ� ���� �ʴٸ� ���� ����
	if (instance == NULL)
		instance = new T;

	return instance;
}

// �̱��� �ν��Ͻ� �����ϱ�
template<typename T>
inline void SingletonBase<T>::ReleaseInstance()
{
	if (instance)
	{
		delete instance;
		instance = NULL;
	}
}

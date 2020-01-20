#pragma once
#include "singleton.h"
#include <bitset>

using namespace std;

/*
bitset : bool Ÿ���� ��� �����̳�

set() : bit ��� 1�� ����
reset() : bit ��� 0���� ����
set(i, value) : i ��° ���� value(true or false)�� ����
[] : �迭 ���� ���� ex) theBit[i] = false;
flip : bit �� ��ȯ, 0 -> 1, 1 -> 0

all() : ��� ��Ʈ�� 1�� �� true ��ȯ
none() : ��� ��Ʈ�� 0�� �� true ��ȯ
any() : 1���� 1�̸� true ��ȯ
count() : ���� 1�� ���� �� ��Ʈ�� ����
*/

#define KEYMAX 256

class KeyManager : public singleton<KeyManager>
{
private:
	bitset<KEYMAX> m_bitKeyUp;
	bitset<KEYMAX> m_bitKeyDown;

public:
	void Init();	// Ű �Ŵ��� �ʱ�ȭ

	bool isKeyDown(int key);	// Ű�� ������ ��
	bool isKeyUp(int key);		// Ű���� ���� ��
	bool isKeyStayDown(int key);	// Ű�� ������ ���� ��
	bool isToggleKey(int key);		// ���Ű �� ����
};


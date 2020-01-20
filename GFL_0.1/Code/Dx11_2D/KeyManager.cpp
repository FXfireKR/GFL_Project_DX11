#include "stdafx.h"
#include "KeyManager.h"


void KeyManager::Init()
{
	// Ű�� ���� ���� ��� false �� �ʱ�ȭ
	m_bitKeyDown.reset();
	m_bitKeyUp.reset();
}

bool KeyManager::isKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		// Ű�� ������ ������ ���� ����
		if (!m_bitKeyDown[key])
		{
			m_bitKeyDown.set(key, true);
			return true;
		}
	}
	else
	{
		m_bitKeyDown.set(key, false);
	}

	return false;
}

bool KeyManager::isKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		// Ű ���� ������ ���·� ó��
		m_bitKeyUp.set(key, true);
	}
	else
	{
		if (m_bitKeyUp[key])
		{
			m_bitKeyUp[key] = false;
			return true;
		}
	}

	return false;
}

bool KeyManager::isKeyStayDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		return true;
	}

	return false;
}

bool KeyManager::isToggleKey(int key)
{
	if (GetAsyncKeyState(key) & 0x0001)
	{
		m_bitKeyDown.flip();
	}

	return m_bitKeyDown[key];
}

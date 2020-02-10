#include "stdafx.h"
#include "KeyManager.h"


void KeyManager::Init()
{
	// 키에 대한 값을 모드 false 로 초기화
	m_bitKeyDown.reset();
	m_bitKeyUp.reset();
}

bool KeyManager::isKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		// 키가 이전에 눌리지 않은 상태
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
		// 키 업이 가능한 상태로 처리
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

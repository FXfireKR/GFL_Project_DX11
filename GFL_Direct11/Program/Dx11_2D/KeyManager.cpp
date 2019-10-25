#include "stdafx.h"
#include "KeyManager.h"

KeyManager::KeyManager()
{
}

KeyManager::~KeyManager()
{
}

HRESULT KeyManager::init()
{
	for (int i = 0; i < KEYBIT; ++i)
	{
		up[i] = false;
		down[i] = false;
	}

	return S_OK;
}

bool KeyManager::isKeyDown(int keycode)
{
	if (GetAsyncKeyState(keycode) & 0x8000)
	{
		if (down[keycode])
			return false;

		else
		{
			down[keycode] = true;
			return true;
		}
	}

	down[keycode] = false;
	return false;
}

bool KeyManager::isKeyStayDown(int keycode)
{
	if (GetAsyncKeyState(keycode) & 0x8000)
		return true;
	return false;
}

bool KeyManager::isKeyUp(int keycode)
{
	//키가 떼졌다면
	if (!(GetAsyncKeyState(keycode) & 0x8000))
	{
		if (up[keycode])
		{
			up[keycode] = false;
			down[keycode] = false;
			return true;
		}
	}
	else
		up[keycode] = true;
	return false;
}

#include "stdafx.h"
#include "KeyManager.h"

KeyManager::KeyManager(){
	this->init();
}

KeyManager::~KeyManager(){}

void KeyManager::init()
{
	// Ű�� ���� ���� ��� false �� �ʱ�ȭ
	bitKeyDown.reset();
	bitKeyUp.reset();
}

const bool KeyManager::isKeyDown(const int & _keyCode)
{
	if (GetAsyncKeyState(_keyCode) & 0x8000) {
		// Ű�� ������ ������ ���� ����
		if (!bitKeyDown[_keyCode]) {
			bitKeyDown[_keyCode] = true;
			return true;
		}
	}
	else
		bitKeyDown[_keyCode] = false;
	return false;
}

const bool KeyManager::isKeyUp(const int & _keyCode)
{
	if (GetAsyncKeyState(_keyCode) & 0x8000)
		bitKeyUp[_keyCode] = true;
	else {
		if (bitKeyUp[_keyCode]) {
			bitKeyUp[_keyCode] = false;
			return true;
		}
	}
	return false;
}

const bool KeyManager::isKeyStayDown(const int & _keyCode)
{
	if (GetAsyncKeyState(_keyCode) & 0x8000)
		return true;
	return false;
}

const bool KeyManager::isToggleKey(const int & _keyCode)
{
	if (GetAsyncKeyState(_keyCode) & 0x0001)
		bitKeyDown.flip();
	return bitKeyDown[_keyCode];
}

#include "stdafx.h"
#include "KeyManager.h"

KeyManager::KeyManager(){
	this->init();
}

KeyManager::~KeyManager(){}

void KeyManager::init()
{
	// 키에 대한 값을 모드 false 로 초기화
	bitKeyDown.reset();
	bitKeyUp.reset();
}

const bool KeyManager::isKeyDown(const int & _keyCode)
{
	if (GetAsyncKeyState(_keyCode) & 0x8000) {
		// 키가 이전에 눌리지 않은 상태
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

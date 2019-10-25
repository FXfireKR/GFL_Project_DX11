#pragma once
#include <bitset>
#include "singleton.h"

#define KEYBIT	256

class KeyManager : public singleton<KeyManager>
{
private:
	bitset<KEYBIT> up;
	bitset<KEYBIT> down;

public:
	KeyManager();
	~KeyManager();

	HRESULT init();
	//void release();

	bool isKeyDown(int keycode);
	bool isKeyStayDown(int keycode);
	bool isKeyUp(int keycode);
};
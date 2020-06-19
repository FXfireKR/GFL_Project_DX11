#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include "singleton.hpp"
#include <bitset>

constexpr int KEYMAX = 256;

class KeyManager : public singleton<KeyManager>
{
private:
	std::bitset<KEYMAX> bitKeyUp;
	std::bitset<KEYMAX> bitKeyDown;

public:
	KeyManager();
	KeyManager(const KeyManager& other) = delete;
	KeyManager operator= (const KeyManager& other) = delete;
	~KeyManager();

private:
	void init();

public:
	const bool isKeyDown(const int& _keyCode);
	const bool isKeyUp(const int& _keyCode);
	const bool isKeyStayDown(const int& _keyCode);
	const bool isToggleKey(const int& _keyCode);
};
#endif // !KEYMANAGER_H
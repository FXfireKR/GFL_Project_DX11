#pragma once
#include "singleton.h"
#include <bitset>

using namespace std;

/*
bitset : bool 타입을 담는 컨테이너

set() : bit 모두 1로 설정
reset() : bit 모두 0으로 설정
set(i, value) : i 번째 값을 value(true or false)로 설정
[] : 배열 접근 가능 ex) theBit[i] = false;
flip : bit 값 전환, 0 -> 1, 1 -> 0

all() : 모든 비트가 1일 때 true 반환
none() : 모든 비트가 0일 때 true 반환
any() : 1개라도 1이면 true 반환
count() : 값이 1로 셋팅 된 비트의 갯수
*/

#define KEYMAX 256

class KeyManager : public singleton<KeyManager>
{
private:
	bitset<KEYMAX> m_bitKeyUp;
	bitset<KEYMAX> m_bitKeyDown;

public:
	void Init();	// 키 매니져 초기화

	bool isKeyDown(int key);	// 키를 눌렀을 때
	bool isKeyUp(int key);		// 키에서 땠을 때
	bool isKeyStayDown(int key);	// 키를 누르고 있을 때
	bool isToggleKey(int key);		// 토글키 온 오프
};


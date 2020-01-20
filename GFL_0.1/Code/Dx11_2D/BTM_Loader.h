#pragma once
#include "BattleMap.h"
/*
	~.btm를 이용한 전투시 사용되는 맵을 불러옵니다.
*/

struct btmPair
{
	string key;
	string value;
};

class BTM_Loader
{
public:
	BTM_Loader();
	BTM_Loader(const BTM_Loader& other) = delete;
	BTM_Loader operator=(BTM_Loader& other) = delete;
	~BTM_Loader();

private:
	btmPair Tokenize(__in const FILE* file);
	string readString(__in const FILE* file);
	POINT readSize(__in const FILE* file);
	D3DXVECTOR3 readVec3(__in const FILE* file);
	int readInteger(__in const FILE* file);
	bool readBoolean(__in const FILE* file);
	bool isCompare(__in const string str, __in const char* comp);
	bool isFind(__in const string str, __in const char* comp);

public:
	BattleMap* LoadBTM(__in const char* _path);

};
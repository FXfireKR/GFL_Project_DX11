#pragma once
#include "singleton.h"

#define DAMAGELOGGER_NUM_LIM	30
#define DAMAGELOGGER_UPSCALE	125.0f

class DamageManager : public singleton<DamageManager>
{
private:
	struct tagLogger
	{
		D3DXVECTOR2 pos;	//	8Byte
		FLOAT alpha;		//	4Byte
		string damage;		//	4Byte
		bool isCritical;	//	1Byte
		bool isRender;		//	1Byte
	};

private:
	map<string, UINT> mPlayerDamage;		//	플레이어 유닛들이 넣은 데미지
	vector<tagLogger*> DamageLogger;		//	데미지 Log하는 녀석

public:
	DamageManager();
	~DamageManager();

	void loadImageList();
	void AllocateMemory();			//	필요한 메모리 로드한다.
	void release();					//	메모리를 해제해준다.
	void Create_Damage(float x, float y, int _damage, bool _isCiritic = false);

	void update();
	void render();
};
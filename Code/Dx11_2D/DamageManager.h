#pragma once
#include "singleton.hpp"

constexpr int		DAMAGELOGGER_NUM_LIM = 30;
constexpr float		DAMAGELOGGER_UPSCALE = 190.0f;

class DamageManager : public singleton<DamageManager>
{
private:
	typedef struct tagLogger
	{
		VEC2 pos;			//	8Byte
		float alpha;		//	4Byte
		string damage;		//	4Byte
		bool isCritical;	//	1Byte
		bool isRender;		//	1Byte
		bool isArmor;
	}DamageLog;

private:
	unordered_map<string, UINT>			mPlayerDamage;		//	플레이어 유닛들이 넣은 데미지
	vector<DamageLog*>					DamageLogger;		//	데미지 Log하는 녀석

public:
	DamageManager();
	~DamageManager();

	void loadImageList();
	void AllocateMemory();			//	필요한 메모리 로드한다.
	void release();					//	메모리를 해제해준다.
	void Create_Damage(float x, float y, int _damage, bool _isCiritic = false);
	void Create_Damage(float x, float y, int _damage, bool _isArmor, bool _isCiritic);

	void update();
	void render();
};
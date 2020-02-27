#pragma once
#include "singleton.h"

#define DAMAGELOGGER_NUM_LIM	30
#define DAMAGELOGGER_UPSCALE	190.0f

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
		bool isArmor;
	};

private:
	map<string, UINT> mPlayerDamage;		//	�÷��̾� ���ֵ��� ���� ������
	vector<tagLogger*> DamageLogger;		//	������ Log�ϴ� �༮

public:
	DamageManager();
	~DamageManager();

	void loadImageList();
	void AllocateMemory();			//	�ʿ��� �޸� �ε��Ѵ�.
	void release();					//	�޸𸮸� �������ش�.
	void Create_Damage(float x, float y, int _damage, bool _isCiritic = false);
	void Create_Damage(float x, float y, int _damage, bool _isArmor, bool _isCiritic);

	void update();
	void render();
};
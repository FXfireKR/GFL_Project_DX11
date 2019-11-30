#pragma once
#include "Bullet.h"
#include "singleton.h"

class BulletManager : public singleton<BulletManager>
{
private:
	vector<Bullet*> vBullet;

public:
	BulletManager();
	~BulletManager();

	HRESULT init();
	void release();
	void update();
	void render();

	HRESULT CreateBullet(string k, float sx, float sy, UINT id, Status st, BULLET_TYPE t, float spd = 25.0f);
	HRESULT CreateBullet(string k, FPNT sp, UINT id, Status st, BULLET_TYPE t, float spd = 25.0f);
	HRESULT CreateBullet(string k, FPNT sp, FLOAT angle, FLOAT limDist, Status st, BULLET_TYPE t, float spd = 25.0f);
};
#pragma once
#include "Bullet.h"
#include "memorypool.h"
#include "singleton.h"

class BulletManager : public singleton<BulletManager>
{
private:
	memorypool<Bullet>* Bullets;

public:
	BulletManager();
	~BulletManager();

	HRESULT init();
	void release();
	void update();
	void render();

	HRESULT CreateBullet(string k, float sx, float sy, UINT id, Status st, TATICDOLL_ALIANCE_TYPE t, float spd = 25.0f);
	HRESULT CreateBullet(string k, D3DXVECTOR2 sp, UINT id, Status st, TATICDOLL_ALIANCE_TYPE t, float spd = 25.0f);
	HRESULT CreateBullet(string k, D3DXVECTOR2 sp, FLOAT angle, FLOAT limDist, Status st, TATICDOLL_ALIANCE_TYPE t, float spd = 25.0f);
};
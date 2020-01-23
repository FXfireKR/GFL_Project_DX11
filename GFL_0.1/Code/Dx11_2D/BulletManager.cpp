#include "stdafx.h"
#include "BulletManager.h"

BulletManager::BulletManager()
	: Bullets(nullptr)
{
}

BulletManager::~BulletManager()
{

}

HRESULT BulletManager::init()
{
	if (Bullets == nullptr)
		Bullets = new memorypool<Bullet>(80);

	//DRAW->LoadImage_d2d("RF_BLT", L"../RemakeResource/EFFECT/riflebullet.png");
	//DRAW->LoadImage_d2d("MG_BLT", L"../RemakeResource/EFFECT/machinegunbullet.png");

	//EFFECT->Create_Sprite("FireFrame", 50, 48);

	return S_OK;
}

void BulletManager::release()
{
	SAFE_DELETE(Bullets);
}

void BulletManager::update()
{
	for (size_t i = Bullets->getUsage().size(); i > 0 ; --i)
	{
		Bullet* Iter = Bullets->getUsage()[i - 1];
		if (!Iter->getFire()) continue;

		Iter->update();

		if (Iter->getArrive())
			Bullets->release(i - 1);
	}

	//for (Bullet* Iter : Bullets->getUsage())
	//{
	//	if (!Iter->getFire()) continue;

	//	Iter->update();

	//	//	메모리 반환
	//	if (Iter->getArrive())
	//		Bullets->release(Iter);
	//}
}
 
void BulletManager::render()
{
	for (Bullet* Iter : Bullets->getUsage())
	{
		if (Iter->getFire())
			Iter->render();
	}
}

HRESULT BulletManager::CreateBullet(string k, float sx, float sy, UINT id, Status st, TATICDOLL_ALIANCE_TYPE t, float spd)
{
	Bullet* _new = Bullets->pickUpPool();

	D3DXVECTOR2 startPoint = D3DXVECTOR2(sx, sy);

	_new->Create_Bullet(k, startPoint, id, st, t, spd);

	Bullets->create(_new);

	return S_OK;
}

HRESULT BulletManager::CreateBullet(string k, D3DXVECTOR2 sp, UINT id, Status st, TATICDOLL_ALIANCE_TYPE t, float spd)
{
	Bullet* _new = Bullets->pickUpPool();

	_new->Create_Bullet(k, sp, id, st, t, spd);

	Bullets->create(_new);

	return S_OK;
}

HRESULT BulletManager::CreateBullet(string k, D3DXVECTOR2 sp, FLOAT angle, FLOAT limDist, Status st, TATICDOLL_ALIANCE_TYPE t, float spd)
{
	Bullet* _new = Bullets->pickUpPool();

	_new->Create_Bullet(k, sp, angle, limDist, st, t, spd);

	Bullets->create(_new);

	return S_OK;
}
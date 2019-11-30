#include "stdafx.h"
#include "BulletManager.h"

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
}

HRESULT BulletManager::init()
{
	vBullet.reserve(100);

	for (int i = 0; i < 100; ++i)
	{
		Bullet* temp = new Bullet;
		temp->init();
		vBullet.push_back(temp);
	}

	//DRAW->LoadImage_d2d("RF_BLT", L"../RemakeResource/EFFECT/riflebullet.png");
	//DRAW->LoadImage_d2d("MG_BLT", L"../RemakeResource/EFFECT/machinegunbullet.png");

	//EFFECT->Create_Sprite("snc", "../RemakeResource/EFFECT/Sonic.png", 128, 128);

	return S_OK;
}

void BulletManager::release()
{
	for (auto& it : vBullet)
		delete it;

	vBullet.clear();
}

void BulletManager::update()
{
	for (auto& it : vBullet)
	{
		if (!it->getFire())continue;

		it->update();

		if (it->getArrive())
		{
			//EFFECT->Create_("snc", 100, rand() % WINDOWSIZE_Y, 1, 1.5F);
			it->init();
		}
	}
}

void BulletManager::render()
{
	for (auto& it : vBullet)
	{
		if(it->getFire())
			it->render();
	}
}

HRESULT BulletManager::CreateBullet(string k, float sx, float sy, UINT id, Status st, BULLET_TYPE t, float spd)
{
	for (auto& it : vBullet)
	{
		if (it->getFire())continue;

		FPNT sp;
		sp.x = sx;
		sp.y = sy;
		it->Create_Bullet(k, sp, id, st, t, spd);
		break;
	}

	return S_OK;
}

HRESULT BulletManager::CreateBullet(string k, FPNT sp, UINT id, Status st, BULLET_TYPE t, float spd)
{
	for (auto& it : vBullet)
	{
		if (it->getFire())continue;

		it->Create_Bullet(k, sp, id, st, t, spd);
		break;
	}

	return S_OK;
}

HRESULT BulletManager::CreateBullet(string k, FPNT sp, FLOAT angle, FLOAT limDist, Status st, BULLET_TYPE t, float spd)
{
	for (auto& it : vBullet)
	{
		if (it->getFire())continue;

		it->Create_Bullet(k, sp, angle, limDist, st, t, spd);
		break;
	}

	return S_OK;
}
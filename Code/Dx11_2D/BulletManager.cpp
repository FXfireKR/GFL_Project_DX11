#include "stdafx.h"
#include "BulletManager.h"
#include "BaseTaticDoll.h"

BulletManager::BulletManager()
	: Bullets(nullptr)
{
}

BulletManager::~BulletManager()
{
	this->release();
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
	for (size_t i = Bullets->getUsage().size(); i > 0; --i)
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

HRESULT BulletManager::CreateBullet(BaseTaticDoll* _this, UINT id, Status st, TATICDOLL_ALIANCE_TYPE t, float limitAngle, float minDist)
{
	//	SG탄환을 사용하는경우 사용하는 함수다.
	//	minDistance :: 데미지가 순수하게 들어가는 최대 사거리
	//	targetAngle :: 주요 타겟을 보고있는 Angle
	//	limitAngle	:: 타겟으로 부터의 최대 Angle 값
	//	홀리 쉣~

	switch (t)
	{
	case ALIANCE_GRIFFON:
	{
		float targetAngle = getAngle(_this->getCharacterPos().x, _this->getCharacterPos().y,
			BDATA->getObject(id)->getCharacterPos().x, BDATA->getObject(id)->getCharacterPos().y);

		float minLimit = targetAngle - RAD(limitAngle);
		float maxLimit = targetAngle + RAD(limitAngle);

		for (auto& enemyID : _this->getRange())
		{
			if (!BDATA->getObject(enemyID)->getAlive())continue;

			float angle = getAngle(_this->getCharacterPos().x, _this->getCharacterPos().y,
				BDATA->getObject(enemyID)->getCharacterPos().x, BDATA->getObject(enemyID)->getCharacterPos().y);


			if (angle > HPI && minLimit < 0.0f)
			{
				minLimit = (PI * 2) + minLimit;
				maxLimit = (PI * 2) + maxLimit;
			}

			if (angle > minLimit  && angle < maxLimit)
			{
				float distance = getDistance(_this->getCharacterPos().x, _this->getCharacterPos().y,
					BDATA->getObject(enemyID)->getCharacterPos().x, BDATA->getObject(enemyID)->getCharacterPos().y);


				//	거리가 멀수록, 명중률과 화력이 감소한다.
				if (distance < minDist)
					BDATA->getObject(enemyID)->Character_GetDamage(st);

				else
				{
					float percent;
					float rangeDist = _this->getCollision("MAX_RANGE")->getLongRad() + BDATA->getObject(enemyID)->getCollision("SELF")->getLongRad() - minDist;
					distance -= minDist;
					percent = 1.0f - (distance / rangeDist);

					st.Accuracy *= static_cast<double>(percent);
					st.AttackPoint = static_cast<int>(static_cast<float>(st.AttackPoint) * percent);
					BDATA->getObject(enemyID)->Character_GetDamage(st);
				}
			}

		}
	}
	break;

	case ALIANCE_IRONBLOD:
	case ALIANCE_PEREDEUS:
	case ALIANCE_SCCOM:
	{

	}
	break;
	}

	return S_OK;
}
#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

HRESULT Bullet::init()
{
	Fire = false;
	Arrive = false;

	return S_OK;
}

HRESULT Bullet::Create_Bullet(string k, D3DXVECTOR2 sp, UINT id, Status st, TATICDOLL_ALIANCE_TYPE t, float spd)
{
	key = k;
	imgSize.x = IMAGEMAP->getImageInfo(key)->Width;
	imgSize.y = IMAGEMAP->getImageInfo(key)->Height;

	pos.x = sp.x;
	pos.y = sp.y;

	//값 복사
	memcpy(&state, &st, sizeof(Status));

	targID = id;

	Angle = 0.0f;
	Speed = spd;

	bAliance = t;
	Fire = true;
	Arrive = false;

	//rc = RectMakeCenter(pos.x - BDATA->getVirtualPos().x, pos.y - BDATA->getVirtualPos().y,
	//	DRAW->getBitmap(key)->GetSize().width / 2, DRAW->getBitmap(key)->GetSize().height / 2);

	return S_OK;
}

HRESULT Bullet::Create_Bullet(string k, D3DXVECTOR2 sp, FLOAT angle, FLOAT limDist, Status st, TATICDOLL_ALIANCE_TYPE t, float spd)
{
	key = k;

	pos.x = sp.x;
	pos.y = sp.y;

	//값 복사
	memcpy(&state, &st, sizeof(Status));

	targID = -1;

	Angle = angle;
	Speed = spd;
	Limit_Distance = limDist;

	bAliance = t;
	Fire = true;
	Arrive = false;

	//rc = RectMakeCenter(pos.x - BDATA->getVirtualPos().x, pos.y - BDATA->getVirtualPos().y,
	//	DRAW->getBitmap(key)->GetSize().width / 2, DRAW->getBitmap(key)->GetSize().height / 2);

	return S_OK;
}

void Bullet::update()
{
	if (targID != -1)
		this->update_TargetBullet();

	else
		this->update_NoneTargetBullet();
}

void Bullet::update_TargetBullet()
{
	if (!Arrive && Fire)
	{
		POINT temp;

		switch (bAliance)
		{
		case ALIANCE_GRIFFON:
		{
			TaticDoll* enemy = BDATA->getObject(targID);
			Angle = getAngle(pos.x, pos.y, enemy->getCharacterPos().x, enemy->getCharacterPos().y - 25);

			temp.x = (int)enemy->getCharacterPos().x;
			temp.y = (int)enemy->getCharacterPos().y - 25;
		}
		break;

		case ALIANCE_IRONBLOD:
		case ALIANCE_PEREDEUS:
		case ALIANCE_SCCOM:
		{
			TaticDoll* enemy = PLAYER->getIOPdoll_crntSquad(targID);
			Angle = getAngle(pos.x, pos.y, enemy->getCharacterPos().x, enemy->getCharacterPos().y - 25);

			temp.x = (int)enemy->getCharacterPos().x;
			temp.y = (int)enemy->getCharacterPos().y - 25;
		}
		break;
		}

		pos.x += cosf(Angle) * Speed * (DELTA * DeltaAcl);
		pos.y -= sinf(Angle) * Speed * (DELTA * DeltaAcl);

		rc = RectMakeCenter(pos.x - CameraPositionX, pos.y - CameraPositionY, 10, 10);

		POINT p;
		p.x = pos.x - CameraPositionX;
		p.y = pos.y - CameraPositionY;


		switch (bAliance)
		{
		case ALIANCE_GRIFFON:
		{
			D3DXVECTOR2 enemyPos = BDATA->getObject(targID)->getCharacterPos();
			TaticDoll* enemy = BDATA->getObject(targID);

			if (getDistance(pos.x, pos.y, enemyPos.x, enemyPos.y) < 30.0f)
			{
				Arrive = true;

				if (enemy->getAlive())
					enemy->Character_GetDamage(state);
			}

			else
			{
				if (!enemy->getAlive())
					Arrive = true;
			}
		}
		break;

		case ALIANCE_IRONBLOD:
		case ALIANCE_PEREDEUS:
		case ALIANCE_SCCOM:
		{
			if (targID < 100)
			{
				D3DXVECTOR2 enemyPos = PLAYER->getIOPdoll_crntSquad(targID)->getCharacterPos();
				TaticDoll* enemy = PLAYER->getIOPdoll_crntSquad(targID);

				if (getDistance(pos.x, pos.y, enemyPos.x, enemyPos.y) < 30.0f)
				{
					Arrive = true;

					if (enemy->getAlive())
						enemy->Character_GetDamage(state);
				}

				else
				{
					if (!enemy->getAlive())
						Arrive = true;
				}
			}
		}
		break;
		}

	}
}

void Bullet::update_NoneTargetBullet()
{
	if (!Arrive && Fire)
	{
		POINT p;
		p.x = pos.x - CameraPositionX;
		p.y = pos.y - CameraPositionY;

		switch (bAliance)
		{
		case ALIANCE_GRIFFON:
			//전체 피격박스 체크
			//for (auto& enemy : BDATA->getEnemySquad(BDATA->getEngageSquadID())->mSquad)
			//{
			//	//죽어있으면 넘어가고, 아니면 충돌체크
			//	if (!enemy.second->getAlive())continue;
			//	if (PtInRect(&(enemy.second->getCharacterRect()), p))
			//	{
			//		Arrive = true;
			//		enemy.second->Character_GetDamage(state);
			//	}

			//	else
			//		Limit_Distance <= 0.0f ? Arrive = true : Limit_Distance -= Speed;

			//}

			Limit_Distance <= 0.0f ? Arrive = true : Limit_Distance -= Speed * (DELTA*DeltaAcl);

			break;

		case ALIANCE_IRONBLOD:
		case ALIANCE_PEREDEUS:
		case ALIANCE_SCCOM:
		{
			//전체 피격박스 체크
			for (auto& enemy : PLAYER->getPlayerSquad(PLAYER->getCurrentSquad())->mSquad)
			{
				//죽어있으면 넘어가고, 아니면 충돌체크
				if (!enemy.second->getAlive())continue;
				if (PtInRect(&(enemy.second->getCharacterRect()), p))
				{
					Arrive = true;
					enemy.second->Character_GetDamage(state);
				}

				else
					Limit_Distance <= 0.0f ? Arrive = true : Limit_Distance -= Speed * (DELTA*DeltaAcl);
			}
		}
			break;
		}

		pos.x += cosf(Angle) * Speed * (DELTA*DeltaAcl);
		pos.y -= sinf(Angle) * Speed * (DELTA*DeltaAcl);


		//rc = RectMakeCenter(pos.x - BDATA->getVirtualPos().x, pos.y - BDATA->getVirtualPos().y,
		//	DRAW->getBitmap(key)->GetSize().width / 2, DRAW->getBitmap(key)->GetSize().height / 2);
	}
}

void Bullet::render()
{
	DRAW->render(key, DV2(imgSize.x * 0.4f, imgSize.y * 0.4f), DV2(pos.x, pos.y), DCR(1, 1, 1, 1), DV3(0, 0, DGR(Angle)));
}
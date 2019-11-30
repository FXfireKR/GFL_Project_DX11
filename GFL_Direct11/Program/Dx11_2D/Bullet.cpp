#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet() : img(nullptr)
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

HRESULT Bullet::Create_Bullet(string k, FPNT sp, UINT id, Status st, BULLET_TYPE t, float spd)
{
	key = k;
	if (img == nullptr)
		img = new Image(k);

	pos.x = sp.x;
	pos.y = sp.y;

	//값 복사
	memcpy(&state, &st, sizeof(Status));

	targID = id;

	Angle = 0.0f;
	Speed = spd;

	type = t;
	Fire = true;
	Arrive = false;

	/*rc = RectMakeCenter(pos.x - BDATA->getVirtualPos().x, pos.y - BDATA->getVirtualPos().y,
		DRAW->getBitmap(key)->GetSize().width / 2, DRAW->getBitmap(key)->GetSize().height / 2);*/

	return S_OK;
}

HRESULT Bullet::Create_Bullet(string k, FPNT sp, FLOAT angle, FLOAT limDist, Status st, BULLET_TYPE t, float spd)
{
	key = k;
	if (img == nullptr)
		img = new Image(k);

	pos.x = sp.x;
	pos.y = sp.y;

	//값 복사
	memcpy(&state, &st, sizeof(Status));

	targID = 999;

	Angle = angle;
	Speed = spd;
	Limit_Distance = limDist;

	type = t;
	Fire = true;
	Arrive = false;

	//rc = RectMakeCenter(pos.x - BDATA->getVirtualPos().x, pos.y - BDATA->getVirtualPos().y,
	//	DRAW->getBitmap(key)->GetSize().width / 2, DRAW->getBitmap(key)->GetSize().height / 2);

	return S_OK;
}

void Bullet::update()
{
	if (targID != 999)
		this->update_TargetBullet();

	else
		this->update_NoneTargetBullet();

	img->setRotate(D3DXVECTOR3(0, 0, DGR(Angle)));
	img->setTextureKey(key);
	img->update();
}

void Bullet::update_TargetBullet()
{
	if (!Arrive && Fire)
	{
		POINT temp;

		switch (type)
		{
		case GRF_BLT:
		{
			auto enemy = BDATA->getEnemyMember(BDATA->getEngageSquadID(), targID);
			Angle = getAngle(pos.x, pos.y, enemy->getCharacterPos().x, enemy->getCharacterPos().y - 25);

			temp.x = (int)enemy->getCharacterPos().x;
			temp.y = (int)enemy->getCharacterPos().y - 25;
		}
		break;

		case SNV_BLT:
		{
			if (targID < 100) {
				auto enemy = PLAYER->getIOPdoll_crntSquad(targID);
				Angle = getAngle(pos.x, pos.y, enemy->getCharacterPos().x, enemy->getCharacterPos().y - 25);

				temp.x = (int)enemy->getCharacterPos().x;
				temp.y = (int)enemy->getCharacterPos().y - 25;
			}

			else {
				auto enemy = BDATA->getEnemyMember(BDATA->getEngageSquadID(), targID - 100);
				Angle = getAngle(pos.x, pos.y, enemy->getCharacterPos().x, enemy->getCharacterPos().y - 25);

				temp.x = (int)enemy->getCharacterPos().x;
				temp.y = (int)enemy->getCharacterPos().y - 25;
			}
		}
		break;
		}

		pos.x += cosf(Angle)*Speed * DELTA;
		pos.y -= sinf(Angle)*Speed * DELTA;

		rc = RectMakeCenter(pos.x - VPOS->x, pos.y - VPOS->y, 10, 10);

		POINT p;
		p.x = pos.x - VPOS->x;
		p.y = pos.y - VPOS->y;
		img->setTrans(D3DXVECTOR2(pos.x, pos.y));

		switch (type)
		{
		case GRF_BLT: 
		{
			FPNT e = BDATA->getEnemyMember(BDATA->getEngageSquadID(), targID)->getCharacterPos();

			if (getDistance(pos.x, pos.y, e.x, e.y) < 30.0f)
			{
				Arrive = true;

				auto enemy = BDATA->getEnemyMember(BDATA->getEngageSquadID(), targID);
				if (enemy->getAlive())
					enemy->Character_GetDamage(state);
			}

			else
			{
				auto enemy = BDATA->getEnemyMember(BDATA->getEngageSquadID(), targID);
				if (!enemy->getAlive())
					Arrive = true;
			}
		}
					  break;

		case SNV_BLT: {
			if (targID < 100)
			{
				FPNT e = PLAYER->getIOPdoll_crntSquad(targID)->getCharacterPos();

				if (getDistance(pos.x, pos.y, e.x, e.y) < 30.0f)
				{
					Arrive = true;

					auto enemy = PLAYER->getIOPdoll_crntSquad(targID);
					if (enemy->getAlive())
						enemy->Character_GetDamage(state);
				}

				else
				{
					auto enemy = PLAYER->getIOPdoll_crntSquad(targID);
					if (!enemy->getAlive())
						Arrive = true;
				}
			}

			else
			{
				if (PtInRect(&BDATA->getEnemyMember(BDATA->getEngageSquadID(), targID - 100)->getCharacterRect(), p))
				{
					Arrive = true;

					auto enemy = BDATA->getEnemyMember(BDATA->getEngageSquadID(), targID - 100);
					if (enemy->getAlive())
						enemy->Character_GetDamage(state);
				}

				else
				{
					auto enemy = BDATA->getEnemyMember(BDATA->getEngageSquadID(), targID - 100);
					if (!enemy->getAlive())
						Arrive = true;
				}
			}
		}
					  break;
		}
		/*
		if (PtInRect(&BDATA->getEnemyMember(BDATA->getEngageSquadID(), targID)->getCharacterRect(), p))
		{
			Arrive = true;

			//GetDamage!
			switch (type)
			{
			case GRF_BLT:
			{
				auto enemy = BDATA->getEnemyMember(BDATA->getEngageSquadID(), targID);
				if (enemy->getAlive())
					enemy->Character_GetDamage(state);
			}
			break;

			case SNV_BLT:
			{
				auto enemy = PLAYER->getIOPdoll_crntSquad(targID);
				if (enemy->getAlive())
					enemy->Character_GetDamage(state);
			}
			break;
			}
		}

		else
		{
			//GetDamage!
			switch (type)
			{
			case GRF_BLT:
			{
				auto enemy = BDATA->getEnemyMember(BDATA->getEngageSquadID(), targID);
				if (!enemy->getAlive())
					Arrive = true;
			}
			break;

			case SNV_BLT:
			{
				auto enemy = PLAYER->getIOPdoll_crntSquad(targID);
				if (!enemy->getAlive())
					Arrive = true;
			}
			break;
			}
		}
		*/

	}
}

void Bullet::update_NoneTargetBullet()
{
	if (!Arrive && Fire)
	{
		POINT p;
		p.x = pos.x - VPOS->x;
		p.y = pos.y - VPOS->y;

		switch (type)
		{
		case GRF_BLT:
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

			Limit_Distance <= 0.0f ? Arrive = true : Limit_Distance -= Speed * DELTA;

			break;

		case SNV_BLT:
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
					Limit_Distance <= 0.0f ? Arrive = true : Limit_Distance -= Speed * DELTA;

			}
			break;
		}

		pos.x += cosf(Angle) * Speed * DELTA;
		pos.y -= sinf(Angle) * Speed * DELTA;

		img->setTrans(D3DXVECTOR2(pos.x, pos.y));

		//rc = RectMakeCenter(pos.x - BDATA->getVirtualPos().x, pos.y - BDATA->getVirtualPos().y,
		//	DRAW->getBitmap(key)->GetSize().width / 2, DRAW->getBitmap(key)->GetSize().height / 2);
	}
}

void Bullet::render()
{
	img->render();

	//자동적으로 미사일이 보는 방향으로 이미지를 회전시킨다.
	//if (!Arrive && Fire)
	//{
	//	//if (DRAW->getBitmap(key) != nullptr)
	//	{
	//		float a = 360.0f - DGR(Angle);
	//		D2D1_POINT_2F dxPoint = { rc.left, rc.top };
	//		//Rendtrg->SetTransform(Matrix3x2F::Rotation(a, dxPoint));

	//		D2D1_RECT_F dxArea;
	//		dxArea.left = pos.x - BDATA->getVirtualPos().x;
	//		dxArea.top = pos.y - BDATA->getVirtualPos().y;
	//		dxArea.right = dxArea.left + (DRAW->getBitmap(key)->GetSize().width / 2);
	//		dxArea.bottom = dxArea.top + (DRAW->getBitmap(key)->GetSize().height / 2);

	//		Rendtrg->DrawBitmap(DRAW->getBitmap(key), dxArea);

	//		//  다른 이미지에 간섭되지 않도록 초기화 해준다  
	//		Rendtrg->SetTransform(Matrix3x2F::Rotation(NULL, dxPoint));

	//		DRAW->render_Rect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
	//		//DRAW->render_Rect(dxArea.left, dxArea.top, dxArea.right - dxArea.left, dxArea.bottom - dxArea.top);
	//	}
	//}
}
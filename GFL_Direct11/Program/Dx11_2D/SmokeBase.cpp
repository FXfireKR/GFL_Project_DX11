#include "stdafx.h"
#include "SmokeBase.h"

SmokeBase::SmokeBase()
{
}

SmokeBase::~SmokeBase()
{
}

HRESULT SmokeBase::init()
{
	image = nullptr;

	fired = false;
	arrived = false;

	Bullet_Speed = SMOKE_SPEED;
	Bullet_Heigh = SMOKE_HIEGH;

	//EFFECT->Create_Sprite("smoke_1", "../RemakeResource/EFFECT/smoke_up2.png", 53, 98);
	//EFFECT->Create_Sprite("smoke_2", "../RemakeResource/EFFECT/Smoke_slide.png", 178, 86);

	return S_OK;
}

void SmokeBase::release()
{
	if (image != nullptr)
		image->release();
	SAFE_DEL(image);
}

HRESULT SmokeBase::Insert_SmokeImage(string key)
{
	if (image == nullptr)
		image = new Image(key);	//할당

	return S_OK;
}

HRESULT SmokeBase::Create_Smoke(float x, float y, float epx, float epy, float dgr, float dec)
{
	posX = x;
	posY = y;
	startX = x;
	startY = y;
	eposX = epx;
	eposY = epy;
	ShootAngle = dgr;
	DecreaseAngle = dec;

	centerPos.x = eposX > posX ? (eposX - posX)*0.5f + posX : (posX - eposX)*0.5f + eposX;
	centerPos.y = 50.0f;	//임의값

	tAngle = 0.0f;
	WaitCount = 0.0f;

	Angle = getAngle(x, y, epx, epy);
	distance = getDistance(x, y, epx, epy);

	perAngle = (Bullet_Speed / distance) * SMOKE_TOTAL_ANGLE;
	stacAngle = SMOKE_FIRST_ANGLE;

	fired = true;
	arrived = false;
	t = 0.0f;

	image->setScale(D3DXVECTOR2(0, 0));

	//rc = RectMakeCenter(posX, posY, image->getWidth() / 2, image->getHeight() / 2);
	rc = RectMake(posX, posY, image->getImageSize().x, image->getImageSize().y);

	return S_OK;
}

void SmokeBase::update()
{
	ImGui::Text("Yutan Coord X : %.2f", posX);
	ImGui::Text("Yutan Coord Y : %.2f", posY);

	if (!arrived && fired)
	{
		/*posX += cosf(Angle) * Bullet_Speed * DELTA;
		posY += -(sinf(Angle) * Bullet_Speed* DELTA * 0.5f) - (sinf(RadianAngle(stacAngle))*Bullet_Heigh);

		if (stacAngle < SMOKE_TOTAL_ANGLE)
			stacAngle += perAngle;
		else
			stacAngle = SMOKE_TOTAL_ANGLE;*/

		QuadraticBezier_Update();

		rc = RectMake(posX, posY, image->getImageSize().x, image->getImageSize().y);

		distance = getDistance(posX, posY, eposX, eposY);

		RECT er;
		float h_spd = (SMOKE_SPEED / 2.0f);
		er.left = eposX - h_spd;
		er.top = eposY - h_spd;
		er.right = eposX + h_spd;
		er.bottom = eposY + h_spd;

		RECT temp;

		WaitCount += DELTA;

		image->setTrans(D3DXVECTOR2(posX, posY));
		image->update();

		//if (IntersectRect(&temp, &er, &rc))
		if (t > 0.99f)
			arrived = true;

		if (WaitCount > 2.25f)
			arrived = true;
	}
}

void SmokeBase::QuadraticBezier_Update()
{
	// 1.0초를 기준으로
	t += DELTA;

	posX = (BEZIER_TIMER - t) * (BEZIER_TIMER - t) * startX + 2 * (BEZIER_TIMER - t) * t * centerPos.x + t * t * eposX;
	posY = (BEZIER_TIMER - t) * (BEZIER_TIMER - t) * startY + 2 * (BEZIER_TIMER - t) * t * centerPos.y + t * t * eposY;


	tAngle = (ShootAngle < 90.0f) ? ShootAngle - (t * DecreaseAngle) : ShootAngle + (t * DecreaseAngle);

	if (ShootAngle < 90.0f)
	{
		if (tAngle > 180.0f)
			if (tAngle < 360.0f + ShootAngle - DecreaseAngle)
				tAngle = 360.0f + ShootAngle - DecreaseAngle;
	}
	else
	{
		if(tAngle > ShootAngle + DecreaseAngle)
			tAngle = ShootAngle - DecreaseAngle;
	}

	image->setRotate(D3DXVECTOR3(0, 0, (tAngle)));

}

void SmokeBase::render(FLOAT acl)
{
	if (!arrived && fired)
	{
		if (image != nullptr)
		{
			if (rc.right - BDATA->getVirtualPos().x > 0)
			{
				image->setScale(D3DXVECTOR2(acl, acl));
				image->render();

			}
		}
	}

}

void SmokeBase::render(float x, float y)
{
}

void SmokeBase::reset()
{
	fired = false;
	arrived = false;

	Bullet_Speed = SMOKE_SPEED;
	Bullet_Heigh = SMOKE_HIEGH;

	tAngle = 0.0f;
	WaitCount = 0.0f;
}
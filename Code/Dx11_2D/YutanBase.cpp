#include "stdafx.h"
#include "YutanBase.h"

YutanBase::YutanBase()
{
}

YutanBase::~YutanBase()
{
}

HRESULT YutanBase::init()
{
	fired = false;
	arrived = false;

	Bullet_Speed = SMOKE_SPEED;
	Bullet_Heigh = SMOKE_HIEGH;

	return S_OK;
}

void YutanBase::release()
{
}

HRESULT YutanBase::InsertUtanImage(string key)
{
	imageKey = key;
	return S_OK;
}

//	@ CreateUtan(Start Pos X, Start Pos Y, End Pos X, End Pos Y, Start Degree, End Degree)
HRESULT YutanBase::CreateUtan(float x, float y, float epx, float epy, float dgr, float dec)
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
	centerPos.y = eposY > posY ? (eposY - posY)*0.5f + posY : (posY - eposY)*0.5f + eposY;	//임의값

	tAngle = 0.0f;
	WaitCount = 0.0f;

	Angle = getAngle(x, y, epx, epy);
	distance = getDistance(x, y, epx, epy);

	perAngle = (Bullet_Speed / distance) * SMOKE_TOTAL_ANGLE;
	stacAngle = SMOKE_FIRST_ANGLE;

	fired = true;
	arrived = false;
	t = 0.0f;
	Bullet_Heigh = 100.0f;

	image = IMAGEMAP->getImageInfo(imageKey);

	//rc = RectMakeCenter(posX, posY, image->getWidth() / 2, image->getHeight() / 2);
	rc = RectMake(posX, posY, image->Width, image->Height);

	return S_OK;
}

void YutanBase::update()
{
	if (!arrived && fired)
	{

		QuadraticBezier_Update();

		rc = RectMake(posX, posY, image->Width, image->Height);

		distance = getDistance(posX, posY, eposX, eposY);

		RECT er;
		float h_spd = (SMOKE_SPEED / 2.0f);
		er.left = eposX - h_spd;
		er.top = eposY - h_spd;
		er.right = eposX + h_spd;
		er.bottom = eposY + h_spd;

		WaitCount += DELTA();

		if (t > 0.99f)
			arrived = true;

		if (WaitCount > 2.25f)
			arrived = true;
	}
}

void YutanBase::QuadraticBezier_Update()
{
	// 1.0초를 기준으로
	t += DELTA();

	float valY = (centerPos.y - Bullet_Heigh);
	valY = valY < FLOAT_EPSILON ? 0 : valY;

	posX = (BEZIER_TIMER - t) * (BEZIER_TIMER - t) * startX + 2 * (BEZIER_TIMER - t) * t * centerPos.x + t * t * eposX;
	posY = (BEZIER_TIMER - t) * (BEZIER_TIMER - t) * startY + 2 * (BEZIER_TIMER - t) * t * valY + t * t * eposY;


	tAngle = (ShootAngle < 90.0f) ? ShootAngle - (t * DecreaseAngle) : ShootAngle + (t * DecreaseAngle);

	if (ShootAngle < 90.0f)
	{
		if (tAngle > 180.0f)
			if (tAngle < 360.0f + ShootAngle - DecreaseAngle)
				tAngle = 360.0f + ShootAngle - DecreaseAngle;
	}
	else
	{
		if (tAngle > ShootAngle + DecreaseAngle)
			tAngle = ShootAngle - DecreaseAngle;
	}
}

void YutanBase::render(FLOAT acl)
{
	if (!arrived && fired) {
		if (rc.right - CameraPositionX > 0)
			DRAW->render(imageKey, D3DXVECTOR2(acl, acl), D3DXVECTOR3(posX, posY, 0), D3DXCOLOR(1, 1, 1, 1), D3DXVECTOR3(0, 0, tAngle));
	}
}

void YutanBase::render(float x, float y)
{
}

void YutanBase::reset()
{
	fired = false;
	arrived = false;

	Bullet_Speed = SMOKE_SPEED;
	Bullet_Heigh = SMOKE_HIEGH;

	tAngle = 0.0f;
	WaitCount = 0.0f;
}
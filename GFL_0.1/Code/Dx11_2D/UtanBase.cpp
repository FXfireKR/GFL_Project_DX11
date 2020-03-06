#include "stdafx.h"
#include "UtanBase.h"

UtanBase::UtanBase()
{
}

UtanBase::~UtanBase()
{
}

HRESULT UtanBase::init()
{
	fired = false;
	arrived = false;

	Bullet_Speed = SMOKE_SPEED;
	Bullet_Heigh = SMOKE_HIEGH;

	//EFFECT->Create_Sprite("smoke_1", "../RemakeResource/EFFECT/smoke_up2.png", 53, 98);
	//EFFECT->Create_Sprite("smoke_2", "../RemakeResource/EFFECT/Smoke_slide.png", 178, 86);

	return S_OK;
}

void UtanBase::release()
{
}

HRESULT UtanBase::Insert_UtanImage(string key)
{
	imageKey = key;
	return S_OK;
}

HRESULT UtanBase::Create_Utan(float x, float y, float epx, float epy, float dgr, float dec)
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
	Bullet_Speed = DELTA;
	Bullet_Heigh = 100.0f;

	image = IMAGEMAP->getImageInfo(imageKey);

	//rc = RectMakeCenter(posX, posY, image->getWidth() / 2, image->getHeight() / 2);
	rc = RectMake(posX, posY, image->Width, image->Height);

	return S_OK;
}

void UtanBase::update()
{
	//ImGui::Text("Yutan Coord X : %.2f", posX);
	//ImGui::Text("Yutan Coord Y : %.2f", posY);

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

		WaitCount += DELTA;

		if (t > 0.99f)
			arrived = true;

		if (WaitCount > 2.25f)
			arrived = true;
	}
}

void UtanBase::QuadraticBezier_Update()
{
	// 1.0초를 기준으로
	t += Bullet_Speed;

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

	//image->setRotate(D3DXVECTOR3(0, 0, (tAngle)));
	//
}

void UtanBase::render(FLOAT acl)
{
	if (!arrived && fired)
	{
		if (rc.right - CameraPositionX > 0)
			DRAW->render(imageKey, D3DXVECTOR2(acl, acl), D3DXVECTOR3(posX, WINSIZEY - posY, 0), D3DXCOLOR(1, 1, 1, 1), D3DXVECTOR3(0, 0, tAngle));
	}
}

void UtanBase::render(float x, float y)
{
}

void UtanBase::reset()
{
	fired = false;
	arrived = false;

	Bullet_Speed = SMOKE_SPEED;
	Bullet_Heigh = SMOKE_HIEGH;

	tAngle = 0.0f;
	WaitCount = 0.0f;
}

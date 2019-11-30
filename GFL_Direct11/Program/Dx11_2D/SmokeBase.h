#pragma once

#include "Image.h"

#define SMOKE_FIRST_ANGLE		60.0f
#define SMOKE_LAST_ANGLE		360.0f - (float)(SMOKE_FIRST_ANGLE)
#define SMOKE_TOTAL_ANGLE		(float)(SMOKE_LAST_ANGLE - SMOKE_FIRST_ANGLE)

#define	SMOKE_HIEGH		9.25f
#define SMOKE_SPEED		10.25f

#define BEZIER_TIMER	1

class SmokeBase
{
private:
	Image* image;

	RECT rc;

	float ShootAngle, DecreaseAngle;
	float Angle, perAngle, stacAngle;
	float startX, startY;
	float t;
	float eposX, eposY;
	float posX, posY;
	float distance;
	float Bullet_Speed;
	float Bullet_Heigh;
	float fst_Angle, lst_Angle;
	float tAngle;
	float WaitCount;

	FPNT centerPos;

	bool fired;
	bool arrived;

public:
	SmokeBase();
	~SmokeBase();

	HRESULT init();
	void release();
	HRESULT Insert_SmokeImage(string key);
	HRESULT Create_Smoke(float x, float y, float epx, float epy, float dgr = 45.0f, float dec = 90.0f);
	void update();
	void QuadraticBezier_Update();
	void render(FLOAT acl = 1.0F);
	void render(float x, float y);

	void reset();

public:
	inline bool getFired() { return fired; }
	inline bool getArrived() { return arrived; }

	inline void setArrived(bool a) { arrived = a; }
	inline void setFired(bool a) { fired = a; }

	inline void setBulletSpeed(float spd) { Bullet_Speed = spd; }
	inline void setBulletHeigh(float hei) { Bullet_Heigh = hei; }
};


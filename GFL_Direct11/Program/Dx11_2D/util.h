#pragma once

#define DEG_TO_RAD	0.017453f
#define PI			3.141592654f
#define PI2			6.283185308f

#define RAD(x) static_cast<float>(x * PI / 180.0f)
#define DGR(x) static_cast<float>(x * 180.0f / PI)

#define FLOAT_EPSILON	0.001f
#define FLOAT_TO_INT(f1) static_cast<int>(f1+ FLOAT_EPSILON)
#define FLOAT_EQUAL(f1,f2) (fabs(f1-f2) <= FLOAT_EPSILON)

enum EQUIPTYPE
{
	EPT_ACESORY = 0,
	EPT_BULLET = 10,
	EPT_FRAME = 20,
	EPT_ACESORY2 = 30,
	EPT_SPECIAL = 40
};

enum EQUIPTOTAL_TYPE
{
	EPC_OPTICAL = 1,		//옵티컬
	EPC_EOTEC = 2,			//이오텍
	EPC_REDDOT = 3,			//레도사

	EPC_APB = 11,			//철갑탄
	EPC_SPDB = 12,			//고속탄
	EPC_SPCB = 13,			//권총탄
	EPC_BSB = 14,			//벅샷
	EPC_SLGB = 15,			//슬러그탄

	EPC_FRAME = 21,			//외골격
	EPC_BPROF = 22,			//방탄판
	EPC_BBOX = 23,			//탄통
	EPC_SUIT = 24,			//광학도트
	EPC_CPSET = 25,			//연산칩

	EPC_NVITION = 34,		//야투경
	EPC_SPRESOR = 35,		//소음기
	EPC_EXTBAR = 36			//연장총열
};

struct FRMAE_MOTION_POINT 
{ 
	UINT x, y; 
}; typedef FRMAE_MOTION_POINT FMPT;

struct POINTUINT
{
	UINT x, y;
}; typedef POINTUINT PUNT;

struct STATEUINT
{
	UINT max, current;
}; typedef STATEUINT SPNT;

struct FLOATPOINT
{
	float x, y;
}; typedef FLOATPOINT FPNT;

static float getDistance(float x1, float y1, float x2, float y2)
{
	float x = x2 - x1;
	float y = y2 - y1;
	float result = sqrtf(pow(x, 2) + pow(y, 2));

	return result;
}

static float getAngle(float x1, float y1, float x2, float y2)
{
	float x = x2 - x1;
	float y = y2 - y1;
	float d = sqrtf(x*x + y * y);
	float angle = acosf(x / d);
	//float angle = atanf(y/x);

	//acosf(x) x는 -1~1의 값을 가진다 (라디안 값)
	//cos의 역함수이며, 치역은 (0~180 degree)
	//180도가 넘어가면 181도가 아니라 179도가 된다

	if (y > 0) angle = PI * 2 - angle;

	return angle;
}

static float RadianAngle(float _dgree)
{
	float _radian = _dgree * PI / 180.0f;

	return _radian;
}

static float DgreeAngle(float _radian)
{
	float Dgree = _radian / PI * 180.0f;
	return Dgree;
}

static RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x, y, x + width, y + height };
	return rc;
}

static RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2) };
	return rc;
}

static void ShakeCamera(OUT float& x, OUT float& y, IN int size)
{
	x = x + (rand() % size) - size / 2;
	y = y + (rand() % size) - size / 2;
}

static float rGetAngle(float x1, float y1, float x2, float y2)
{
	float x = x2 - x1;
	float y = y2 - y1;
	float d = sqrtf((x * x) + (y * y));
	float angle = acosf(x / d);
	//float angle = atanf(y/x);

	//acosf(x) x는 -1~1의 값을 가진다 (라디안 값)
	//cos의 역함수이며, 치역은 (0~180 degree)
	//180도가 넘어가면 181도가 아니라 179도가 된다

	if (y < 0) angle = (PI * 2) - angle;

	return angle;
}

static float dGetAngle(float x1, float y1, float x2, float y2)
{
	float x = x2 - x1;
	float y = y2 - y1;
	float d = sqrtf(pow(x, 2) + pow(y, 2));
	float angle = acosf(x / d);

	//float angle = atanf(y/x);

	//acosf(x) x는 -1~1의 값을 가진다 (라디안 값)
	//cos의 역함수이며, 치역은 (0~180 degree)
	//180도가 넘어가면 181도가 아니라 179도가 된다

	//Radian
	if (y < 0) angle = (PI * 2) - angle;

	angle = DGR(angle);

	return angle;
}

static bool AABB_Collition(const DXRect & rc1, const DXRect & rc2)
{
	if ((rc1.getTransX() - rc1.getScaleX()*0.5f) > (rc2.getTransX() + rc2.getScaleX()*0.5f) || (rc1.getTransX() + rc2.getScaleX()*0.5f) < (rc2.getTransX() - rc2.getScaleX()*0.5f))return false;
	if ((rc1.getTransY() + rc1.getScaleY()*0.5f) < (rc2.getTransY() - rc2.getScaleY()*0.5f) || (rc1.getTransY() - rc2.getScaleY()*0.5f) > (rc2.getTransY() + rc2.getScaleY()*0.5f))return false;

	return true;
}

static bool AABB_Collition(FLOAT x1, FLOAT y1, FLOAT wid1, FLOAT hei1, FLOAT x2, FLOAT y2, FLOAT wid2, FLOAT hei2)
{
	if (((x1 - wid1) > (x2 + wid2)) || ((x1 + wid1) < (x2 - wid2)))return false;
	if (((y1 - hei1) > (y2 + hei2)) || ((y1 + hei1) < (y2 - hei2)))return false;

	return true;
}

static bool API_Collition(const DXRect & rc1, const DXRect & rc2)
{
	RECT rt1, rt2, temp;
	rt1.left = rc1.getTransX() - rc1.getScaleX()*0.5f;
	rt1.right = rc1.getTransX() + rc1.getScaleX()*0.5f;
	rt1.top = rc1.getTransY() - rc1.getScaleY()*0.5f;
	rt1.bottom = rc1.getTransY() + rc1.getScaleY()*0.5f;

	rt2.left = rc2.getTransX() - rc2.getScaleX()*0.5f;
	rt2.right = rc2.getTransX() + rc2.getScaleX()*0.5f;
	rt2.top = rc2.getTransY() - rc2.getScaleY()*0.5f;
	rt2.bottom = rc2.getTransY() + rc2.getScaleY()*0.5f;


	return IntersectRect(&temp, &rt1, &rt2);
}

static bool API_Collition(OUT DXRect& intersect, const DXRect & rc1, const DXRect & rc2)
{
	RECT rt1, rt2, temp;
	rt1.left = rc1.getTransX() - rc1.getScaleX()*0.5f;
	rt1.right = rc1.getTransX() + rc1.getScaleX()*0.5f;
	rt1.top = rc1.getTransY() - rc1.getScaleY()*0.5f;
	rt1.bottom = rc1.getTransY() + rc1.getScaleY()*0.5f;

	rt2.left = rc2.getTransX() - rc2.getScaleX()*0.5f;
	rt2.right = rc2.getTransX() + rc2.getScaleX()*0.5f;
	rt2.top = rc2.getTransY() - rc2.getScaleY()*0.5f;
	rt2.bottom = rc2.getTransY() + rc2.getScaleY()*0.5f;

	bool result = IntersectRect(&temp, &rt1, &rt2);

	if (result)
	{
		intersect.setScaleX(temp.right - temp.left);
		intersect.setScaleY(temp.bottom - temp.top);
		intersect.setTransX(temp.left + intersect.getScaleX()*0.5f);
		intersect.setTransY(temp.bottom - intersect.getScaleY()*0.5f);
	}

	return result;
}

static bool ptInRect(D3DXVECTOR2 scale, POINT pos, POINT mPos)
{
	FLOAT left = pos.x - (scale.x / 2.0F);
	FLOAT right = pos.x + (scale.x / 2.0F);
	FLOAT bottom = pos.y + (scale.y / 2.0F);
	FLOAT top = pos.y - (scale.y / 2.0F);

	if (mPos.x > left && mPos.x < right)
		if (mPos.y > top && mPos.y < bottom)
			return true;

	return false;
}
#pragma once

#include <cassert>
#include <assert.h>
#include <sal.h>
#include <stdlib.h>

#define NEW(t, s)			(__new<t>(s))
#define FREE(m)				(__free(m))

#define DEG_TO_RAD	0.017453f
#define HPI			1.570796326f
#define PI			3.141592654f
#define TPI			HPI + PI
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

typedef void(*ButtonPointer)(void* obj);
struct tagButton
{
	D2D_RECT_F			box;				//버튼 크기
	ButtonPointer		ClickAction;		//클릭했을때의 함수포인터

	tagButton() {}
	tagButton(D2D_RECT_F pt, void* adr) : box(pt), ClickAction((ButtonPointer)adr) {}
	tagButton(FLOAT x, FLOAT y, FLOAT w, FLOAT h, void* adr) : ClickAction((ButtonPointer)adr)
	{
		box.left = x;
		box.right = x + w;
		box.top = y;
		box.bottom = y + h;
	}

}; typedef tagButton Button;

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

	if (y > 0)
		angle = PI * 2 - angle;

	return angle;
}

static RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x, y, x + width, y + height };
	return rc;
}

static RECT RectMakeCenter(int _x, int _y, int _w, int _h)
{
	RECT rc;

	rc.left = _x - _w;
	rc.right = _x + _w;
	rc.top = _y - _h;
	rc.bottom = _y + _h;

	return rc;
}

static bool AABB_Collition(FLOAT x1, FLOAT y1, FLOAT wid1, FLOAT hei1, FLOAT x2, FLOAT y2, FLOAT wid2, FLOAT hei2)
{
	if (((x1 - wid1) > (x2 + wid2)) || ((x1 + wid1) < (x2 - wid2)))return false;
	if (((y1 - hei1) > (y2 + hei2)) || ((y1 + hei1) < (y2 - hei2)))return false;

	return true;
}


static bool ptInRect(D3DXVECTOR2 scale, POINT pos, POINT mPos)
{
	FLOAT left = pos.x - (scale.x * 0.5F);
	FLOAT right = pos.x + (scale.x * 0.5F);
	FLOAT bottom = pos.y + (scale.y * 0.5F);
	FLOAT top = pos.y - (scale.y * 0.5F);

	if (mPos.x > left && mPos.x < right)
		if (mPos.y > top && mPos.y < bottom)
			return true;

	return false;
}

static bool ptInRect(D2D_RECT_F rc, POINT mPos)
{
	if (mPos.x > rc.left && mPos.x < rc.right)
		if (mPos.y > rc.top && mPos.y < rc.bottom)
			return true;

	return false;
}

static D2D_RECT_F D2D_RectMake(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
	D2D_RECT_F rc;
	rc.left = left;
	rc.top = top;
	rc.right = right;
	rc.bottom = bottom;

	return rc;
}

static D2D_RECT_F D2D_RectMakeCenter(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
	D2D_RECT_F rc;
	rc.left = x - width;
	rc.top = y - height;
	rc.right = x + width;
	rc.bottom = y + height;

	return rc;
}

template <typename T>
static char* ChangeToLPC(T _value)
{
	string typeName = typeid(_value).name();
	char value[128];

	if (typeName.compare("int") == 0)
		sprintf(value, "%d", (int)_value);

	else if (typeName.compare("float") == 0 || typeName.compare("double") == 0)
		sprintf(value, "%lf", _value);


	return value;
}

template<typename T>
static T* __new(__in size_t _size = 1)
{
	T* newPointer = reinterpret_cast<T*>(malloc(sizeof(T) * _size));
	return (newPointer == nullptr) ? nullptr : newPointer;
}

static void* __new(__in size_t _size = 1)
{
	void* newPointer = malloc(_size);
	return (newPointer == nullptr) ? nullptr : newPointer;
}

static void __free(__in void* _memory)
{
	if (_memory != nullptr)
	{
		free(_memory);
		_memory = nullptr;
	}
}
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

#define Check(hr) assert(SUCCEEDED(hr))

template <typename T>
struct STATE {
	T max, curr;
};

//	���������� �Ҽ� ����
enum TATICDOLL_ALIANCE_TYPE
{
	ALIANCE_NONE = 0,		//	�߸�
	ALIANCE_GRIFFON,		//	�׸���
	ALIANCE_IRONBLOD,		//	ö������
	ALIANCE_SCCOM,			//	��Ÿ �� ����
	ALIANCE_PEREDEUS		//	�䷯���콺
};

//	���������� ���� ����
enum TATICDOLL_WEAPON_TYPE
{
	TWT_NONE = -1,
	TWT_AR,
	TWT_RF,
	TWT_SR,
	TWT_MG,
	TWT_SG,
	TWT_HG,
	TWT_SMG
};

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
	EPC_OPTICAL = 1,		//��Ƽ��
	EPC_EOTEC = 2,			//�̿���
	EPC_REDDOT = 3,			//������

	EPC_APB = 11,			//ö��ź
	EPC_SPDB = 12,			//���ź
	EPC_SPCB = 13,			//����ź
	EPC_BSB = 14,			//����
	EPC_SLGB = 15,			//������ź

	EPC_FRAME = 21,			//�ܰ��
	EPC_BPROF = 22,			//��ź��
	EPC_BBOX = 23,			//ź��
	EPC_SUIT = 24,			//���е�Ʈ
	EPC_CPSET = 25,			//����Ĩ

	EPC_NVITION = 34,		//������
	EPC_SPRESOR = 35,		//������
	EPC_EXTBAR = 36			//�����ѿ�
};

//	���������� �ɷ�ġ
struct Status
{
	STATE<int> HitPoint;		//	ü��
	STATE<int> ArmorPoint;		//	���� ��ġ	[ SG ���� ]

	int Armor;					//	��ü ����
	int ArmorPierce;			//	���� �ļ��ġ		[ ���������������, �尩�� ���� ���ֿ��� ���� 1�� ���� ]
	double ArmorIgnore;			//	���� ���� ����		[ 0 ~ 1.0 ]

	STATE<int> ShieldPoint;		//	���� ��
	int ShieldPierce;			//	���� �ļ��ġ			[ ���������������, ������ ���� ���ֿ��� ���� �ټ�����. ]
	double ShieldIgnore;		//	���� ���� Ȯ��		[ 0 ~ 1.0 ]

	int AttackPoint;			//	���ݷ�

	double Accuracy;			//	���߷�		[ 0 ~ 1.0 ]
	double Avoid;				//	ȸ����		[ 0 ~ 1.0 ]

	double CriticPoint;			//	ġ��Ÿ��		[ 0 ~ 100.0 ]
	double CriticAcl;			//	ġ�����		[ 1 ~ INF ]

	double AttackDelay;			//	�������� ~ �� ���ݱ����� ���ð�
	double AimDelay;			//	��� ~ ����������� ���ؽð�

	Status()
	{
		memset(this, 0, sizeof(Status));
	}
};

//	���ҽ� Scene �ε�� Release�� ����� ����
struct LoadResourceData
{
	string resourceKey;
	LOADRESOURCE_TYPE type;

	LoadResourceData(string key, LOADRESOURCE_TYPE lType)
		: resourceKey(key), type(lType) {}

};

//��ȭ�� ��½� �ʿ��� �͵��� �����ص�
struct Convers
{
	string text, SpeakName;		//	��ȭ����, ȭ���̸�
	string bgmKey;				//	������� �����̸�
	string bkKey;				//	����̹��� �����̸�
	string curSound;			//	���� ����Ǵ� BGM

	int Speaker;				//	ȭ��
	vector<string> vImageKey;	//	ȭ�� �̹���Ű
};

typedef void(*ButtonPointer)(void* obj);
struct tagButton
{
	D2D_RECT_F			box;				//��ư ũ��
	ButtonPointer		ClickAction;		//Ŭ���������� �Լ�������
	D3DXCOLOR			boxImgColor;

	tagButton() {}
	tagButton(D2D_RECT_F pt, void* adr) : box(pt), ClickAction((ButtonPointer)adr) {}
	tagButton(FLOAT x, FLOAT y, FLOAT w, FLOAT h, void* adr) : ClickAction((ButtonPointer)adr)
	{
		box.left = x;
		box.right = x + w;
		box.top = y;
		box.bottom = y + h;
		boxImgColor = D3DXCOLOR(1, 1, 1, 1);
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

static float getAngle(D3DXVECTOR2 _1, D3DXVECTOR2 _2) {

	float x = _2.x - _1.x;
	float y = _2.y - _1.y;
	float d = sqrtf(x*x + y * y);
	float angle = acosf(x / d);
	//float angle = atanf(y/x);

	//acosf(x) x�� -1~1�� ���� ������ (���� ��)
	//cos�� ���Լ��̸�, ġ���� (0~180 degree)
	//180���� �Ѿ�� 181���� �ƴ϶� 179���� �ȴ�

	if (y > 0)
		angle = PI * 2 - angle;

	return angle;
}

static float getAngle(float x1, float y1, float x2, float y2)
{
	float x = x2 - x1;
	float y = y2 - y1;
	float d = sqrtf(x*x + y * y);
	float angle = acosf(x / d);
	//float angle = atanf(y/x);

	//acosf(x) x�� -1~1�� ���� ������ (���� ��)
	//cos�� ���Լ��̸�, ġ���� (0~180 degree)
	//180���� �Ѿ�� 181���� �ƴ϶� 179���� �ȴ�

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

static RECT RectMakeCenter(float _x, float _y, float _w, float _h)
{
	RECT rc;

	rc.left = static_cast<LONG>(_x - _w);
	rc.right = static_cast<LONG>(_x + _w);
	rc.top = static_cast<LONG>(_y - _h);
	rc.bottom = static_cast<LONG>(_y + _h);

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

static D2D_RECT_F D2DRectMake(FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
	D2D_RECT_F rc;
	rc.left = left;
	rc.top = top;
	rc.right = left + width;
	rc.bottom = top + height;

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
		sprintf(value, "%d", _value);

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

static bool str_compare(string _str, const char* _compare)
{
	return _str.compare(_compare) == 0 ? true : false;
}

static void StatusInput(Status* by, Status* from)
{
	int saveHP = by->HitPoint.curr;
	int saveAP = by->ArmorPoint.curr;
	int saveSP = by->ShieldPoint.curr;

	*by = *from;

	by->HitPoint.curr = saveHP;
	by->ArmorPoint.curr = saveAP;
	by->ShieldPoint.curr = saveSP;
}

static string ConvertFormat(const string _text, ...) 
{
	va_list args;

	va_start(args, _text);
	size_t len = vsnprintf(NULL, 0, _text.c_str(), args);
	va_end(args);

	vector<char> vec(len + 1);

	va_start(args, _text);
	vsnprintf(&vec[0], len + 1, _text.c_str(), args);
	va_end(args);

	return &vec[0];
}
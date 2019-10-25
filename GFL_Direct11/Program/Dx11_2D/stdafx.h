// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <conio.h>

// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.
#include <assert.h>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <fstream>
#include <sstream>

using namespace std;


#include "dxComponent.h"
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "imgui.lib")

//irrklang :: SoundEngine

//include Header :: irrklang
//	../SoundLIB/include

//include library :: irrklang
//	../SoundLIB/lib/Win32-visualStudio

//irrklang Engine Line
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

#include "KeyManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "ShaderManager.h"
#include "SoundManager.h"

// 전역 디파인
#define WINSIZEX 1280
#define WINSIZEY 720

#define PI 3.14159265

#define DELTA	TimeManager::getInstance()->getDeltaTime()


#define KEYMANAGER		KeyManager::getInstance()
#define SCENE			SceneManager::getInstance()
#define FPSTIMER		TimeManager::getInstance()
#define SHADER			ShaderManager::getInstance()
#define SOUNDMANAGER	SoundManager::getInstance()

#define SAFE_REL(p) {if(p!=nullptr){p->Release();p=nullptr;}}
#define SAFE_DEL(p) {if(p!=nullptr){delete p; p=nullptr;}}
#define SAFE_DEL_ARY(p) {if(p!=nullptr){delete[] p; p=nullptr;}}

#define DGR(r) {(float)(r*180.0f/PI)}
#define RAD(d) {(float)(d*PI/180.0f)}

// 전역 변수
extern HINSTANCE	g_hInst;
extern HWND			g_hWnd;
extern POINT		_ptMouse;
extern int			KeyCode;
extern D3DXVECTOR2	virtualPos;


static void ShakeCamera(OUT float& x, OUT float& y, IN int size)
{
	x = x + (rand() % size) - size/2;
	y = y + (rand() % size) - size/2;
}

static float getDistance(float x1, float y1, float x2, float y2)
{
	float x = x2 - x1;
	float y = y2 - y1;
	float result = sqrtf(pow(x, 2) + pow(y, 2));

	return result;
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

struct tagDoubleFloat
{
	float x, y;
}; typedef tagDoubleFloat FPNT;

struct tagSTATUSPOINT
{
	int max, current;
};  typedef tagSTATUSPOINT STATUSPOINT;
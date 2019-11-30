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
#include <atlbase.h>

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

#include "util.h"

enum LOADRESOURCE_TYPE
{
	RESOURCE_SOUND = 0,
	RESOURCE_IMAGE,
	RESOURCE_MAP,
	RESOURCE_EQUIP,
	RESOURCE_TEXT
};

extern mutex			locker;

#include "KeyManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "ShaderManager.h"
#include "LoadManager.h"
#include "Direct2DManager.h"
#include "ImageManager.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "BattleDataBase.h"
#include "Player.h"
#include "BulletManager.h"
#include "TextManager.h"

// 전역 디파인
#define WINSIZEX 1280
#define WINSIZEY 720

#define PI 3.14159265

#define CIPHING_BLOC_SIZE	32

#define KEYMANAGER		KeyManager::getInstance()
#define SCENE			SceneManager::getInstance()
#define FPSTIMER		TimeManager::getInstance()
#define SHADER			ShaderManager::getInstance()
#define LOADMANAGER		LoadManager::getInstance()
#define DRAWMANAGER		Direct2DManager::getInstance()
#define SOUNDMANAGER	SoundManager::getInstance()
#define IMAGEMANAGER	ImageManager::getInstance()
#define BDATA			BattleDataBase::getInstance()
#define PLAYER			Player::getInstance()
#define BEFCT			BulletManager::getInstance()
#define EFFECT			EffectManager::getInstance()
#define TEXT			TextManager::getInstance()

#define VPOS			BattleDataBase::getInstance()->p_getVirtualPos()
#define DELTA			TimeManager::getInstance()->getDeltaTime()



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
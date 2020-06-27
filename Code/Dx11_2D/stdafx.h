#ifndef STDAFX_H
#define STDAFX_H

//	if Debug, Imgui Active
#ifdef _DEBUG
#define IMGUIAPI_DEBUG
#pragma warning(disable : 4996)
#pragma warning(disable : 4100)		//	참조되지 않은 선언 입니다.
#pragma warning(disable : 4189)		//	초기화 되었으나, 사용되지않은 선언 입니다.

#else
//#define IMGUIAPI_DEBUG
#pragma warning(disable : 4996)
#pragma warning(disable : 4100)		//	참조되지 않은 선언 입니다.
#pragma warning(disable : 4189)		//	초기화 되었으나, 사용되지않은 선언 입니다.

#endif
//	Release Build can't ImGui Active

//#include <crtdbg.h>
//#define CRTDBG_MAP_ALLOC 1
////	MemoryLeak Check
//
////	Debug Flag
//#ifdef _DEBUG
//static int nFlag = _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//
//#endif
//#endif


#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include <iostream>
#include <windows.h>

// c header
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//	cpp header
#include <assert.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <deque>
#include <stack>
#include <algorithm>
#include <thread>
#include <mutex>
#include <fstream>
#include <sstream>
#include <atlbase.h>			//	wstring - string change Header

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////			Include Library Files				//////////////////////////

//	irrklang Engine Line
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

//	Spine2.5 Library Line
#include <spine.h>
#pragma comment(lib, "spineDebug.lib")

//	ImGui Incoude
#ifdef IMGUIAPI_DEBUG
#include <ImGui/imgui.h>
#include <ImGui/imguiDx11.h>
#pragma comment(lib, "ImGui/imgui.lib")
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////


// extern define
static const UINT	WINSIZEX = 1280;
static const UINT	WINSIZEY = 720;
static const UINT	INITX = 640;
static const UINT	INITY = 360;
static const float	FRAMELOCK = 60.0f;

#define SAFE_DELETE(p) {if(p != nullptr){delete p; p = nullptr;}}

//	delete function
//template<typename T>	inline const void SAFE_DELETE(T* p) { if (p != nullptr) { delete(p); (p) = nullptr; } }
template<typename T>	inline const void SAFE_RELEASE(T* p) { if (p != nullptr) { (p)->Release(); (p) = nullptr; } }
template<typename T>	inline const void SAFE_DEL_ARRAY(T* p) { if (p != nullptr) { delete[](p); (p) = nullptr; } }

//	DirectX Ciomponent header include
#include "DXComponent.h"
#include "GameComponent.h"

#include "util.h"

//	type define new type
typedef signed int						SINT;
typedef D3DXVECTOR2						VEC2;
typedef D3DXVECTOR3						VEC3;
typedef D3DXVECTOR2						Vector2;
typedef D3DXVECTOR3						Vector3;
typedef D3DXCOLOR						COLR;
typedef D3DXMATRIX						MATRIX;
typedef ID3D11Buffer					DBuffer;
typedef ID3D11ShaderResourceView		ShaderResourceView;

// extern global value
extern HINSTANCE	g_hInst;
extern HWND			g_hWnd;
extern POINT		g_ptMouse;
extern int			whlCount;
extern float		DeltaAcl;
extern float		CameraPositionX;
extern float		CameraPositionY;
extern mutex		locker;
extern D3DXCOLOR	worldColor;

extern wstring		PTShaderFile;
extern wstring		PT_BaseShaderFile;
extern wstring		PT_BaseShaderFile2;
extern wstring		PT_NoiseShaderFile;
extern wstring		PT_AlphaShaderFile;
extern wstring		PT_Alpha2ShaderFile;
extern wstring		PNTShaderFile;

//	Manager header include
#include "ShaderManager.h"
#include "GameTimer.h"
#include "ViewCamera.h"
#include "Direct2DManager.h"
#include "SceneManager.h"
#include "KeyManager.h"
#include "image.h"
#include "ImageManager.h"
#include "SoundManager.h"
#include "uiAtlasLoader.h"
#include "LoadManager.h"
#include "TextManager.h"
#include "DamageManager.h"
#include "EquipLoader.h"
#include "BulletManager.h"
#include "EffectManager.h"
#include "BattleDataBase.h"
#include "MapManager.h"

#define TIMER			GameTimer::getInstance()
#define SHADER			ShaderManager::getInstance()
#define KEYMANAGER		KeyManager::getInstance()
#define D2D				Direct2DManager::getInstance()
#define DRAW			Image::getInstance()
#define IMAGEMAP		ImageManager::getInstance()
#define SOUND			SoundManager::getInstance()
#define CAMERA			ViewCamera::getInstance()
#define DWRITE			TextManager::getInstance()
#define SCENE			SceneManager::getInstance()
#define LOAD			LoadManager::getInstance()
#define DAMAGE			DamageManager::getInstance()
#define	EQUIP			EquipLoader::getInstance()
#define BULLET			BulletManager::getInstance()
#define EFFECT			EffectManager::getInstance()
#define BDATA			BattleDataBase::getInstance()
#define PLAYER			BDATA->getPlayerData()
#define MAP				MapManager::getInstance()
#define BTLMAP			MAP->getBattleMap()

static const float			DELTA()		{ return static_cast<float>(TIMER->DeltaTime()); }

#endif
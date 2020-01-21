#include "stdafx.h"
#include "LoadScene.h"

LoadScene::LoadScene()
{
	TEXT->Create_TextField("LOADING_PERCENT", L"맑은고딕", "", 40);
	TEXT->Create_TextField("LOADING_TOOLTIP", L"맑은고딕", "", 25);

	percent = 0.0f;
	toolCheckTimer = 0;
}

LoadScene::~LoadScene()
{
}

void LoadScene::init()
{
}

void LoadScene::release()
{
}

void LoadScene::update()
{
	if (toolCheckTimer < FLOAT_EPSILON)
	{
		switch (rand() % 4)
		{
		case 0:
			TEXT->Change_Text("LOADING_TOOLTIP", "옵티컬 (OPTICAL) 스코프는, 치명확률과 치명배율및 전술인형의 교전거리를 증가시킵니다.");
			break;

		case 1:
			TEXT->Change_Text("LOADING_TOOLTIP", "무조건적인 공격은 좋은 작전이 아닙니다.");
			break;

		case 2:
			TEXT->Change_Text("LOADING_TOOLTIP", "이오텍 (EOTEC) 스코프는, 화력과 명중률이 필요한 전술인형에게 좋습니다.");
			break;

		case 3:
			TEXT->Change_Text("LOADING_TOOLTIP", "SR 전술인형들의 장점은 높은화력과 명중률입니다. 하지만 발이 느립니다.");
			break;
		}

		toolCheckTimer = 4.0f;
	}
	else
		toolCheckTimer -= DELTA;

	int p = LOADMANAGER->getLoadPointer();
	FLOAT size = static_cast<FLOAT>(1.0f / LOADMANAGER->getLoadListSize());

	if (LOADMANAGER->getLoadListSize() > 0)
		percent = static_cast<FLOAT>(p * size * WINSIZEX);

	else {
		percent = WINSIZEX;
	}

	LOADMANAGER->update();

	if (!LOADMANAGER->getAutoInit())
	{
		//if (LOADMANAGER->getLoadListSize() < 1)
		//{
		//	SCENE->Change_Scene(LOADMANAGER->getNextScene());
		//}
	}
}

void LoadScene::render()
{
	DRAW->render(LOADMANAGER->getLoadImageKey(), D3DXVECTOR2(WINSIZEX * 0.5f, WINSIZEY * 0.5f),
		D3DXVECTOR2(WINSIZEX*0.5f, (WINSIZEY*0.5f)));

	D2DX->renderRect(D2D_RectMake(0, WINSIZEY - 120, WINSIZEX, WINSIZEY), ColorF(0, 0, 0, 1), true);
	//DRAWMANAGER->renderRect(D2D_RectMake(percent + 40, 590, percent + 50, 610), ColorF(1, 1, 0, 0.1), true);
	//DRAWMANAGER->renderRect(D2D_RectMake(percent + 30, 590, percent + 40, 610), ColorF(1, 1, 0, 0.22), true);
	//DRAWMANAGER->renderRect(D2D_RectMake(percent + 20, 590, percent + 30, 610), ColorF(1, 1, 0, 0.35), true);
	//DRAWMANAGER->renderRect(D2D_RectMake(percent + 10, 590, percent + 20, 610), ColorF(1, 1, 0, 0.5), true);
	//DRAWMANAGER->renderRect(D2D_RectMake(percent, 590, percent + 10, 610), ColorF(1, 1, 0, 0.6), true);
	D2DX->renderRect(D2D_RectMake(0, 599, percent, 601), ColorF(1, 1, 0, 0.8), true);


	//LoadLight->render("LOADCIRCLE");
	TEXT->TextRender("LOADING_TOOLTIP", 0, WINSIZEY - 80, WINSIZEX, 80, ColorF(1, 1, 1), DWRITE_TEXT_ALIGNMENT_CENTER);
}
#include "stdafx.h"
#include "LoadScene.h"

LoadScene::LoadScene()
{
	//TEXT->Create_TextField("LOADING_PERCENT", L"맑은고딕", "", 40);
	//TEXT->Create_TextField("LOADING_TOOLTIP", L"맑은고딕", "", 25);

	percent = 0.0f;
	destPercent = 0.0f;
	toolCheckTimer = 0.0f;
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
	CAMERA->setCameraFix(true);
	CAMERA->CameraReset();

	if (toolCheckTimer < FLOAT_EPSILON)
	{
		switch (rand() % 4)
		{
		case 0:
			//TEXT->Change_Text("LOADING_TOOLTIP", "옵티컬 (OPTICAL) 스코프는, 치명확률과 치명배율및 전술인형의 교전거리를 증가시킵니다.");
			break;

		case 1:
			//TEXT->Change_Text("LOADING_TOOLTIP", "무조건적인 공격은 좋은 작전이 아닙니다.");
			break;

		case 2:
			//TEXT->Change_Text("LOADING_TOOLTIP", "이오텍 (EOTEC) 스코프는, 화력과 명중률이 필요한 전술인형에게 좋습니다.");
			break;

		case 3:
			//TEXT->Change_Text("LOADING_TOOLTIP", "SR 전술인형들의 장점은 높은화력과 명중률입니다. 하지만 발이 느립니다.");
			break;
		}

		toolCheckTimer = 4.0f;
	}
	else
		toolCheckTimer -= DELTA();


	/*int p = LOAD->getLoadPointer();
	FLOAT size = static_cast<FLOAT>(1.0f / static_cast<float>(LOAD->getLoadListSize()));

	destPercent = LOAD->getLoadListSize() > 0 ?
		static_cast<float>(static_cast<float>(p) * size * static_cast<float>(WINSIZEX)) : static_cast<float>(WINSIZEX);

	if (percent < destPercent) {
		if (destPercent - percent > FLOAT_EPSILON)
			percent += DELTA() * 100.0f;
		else
			percent = destPercent;
	}*/

	if (LOAD->loadUpdate()) {
		if (!LOAD->getAutoInit())
			if (LOAD->getLoadListSize() < 1)
				SCENE->changeScene(LOAD->getNextScene());
	}

	percent = LOAD->getCurPercent();
	destPercent = LOAD->getTargPercent();

	//LOAD->update();

	

	/*
	LOAD->update();

	

	else {
		if (!LOAD->getAutoInit()) {
			if (LOAD->getLoadListSize() < 1)
				SCENE->changeScene(LOAD->getNextScene());
		}
		else
			SCENE->changeScene(LOAD->getNextScene(), true);

		percent = 0.0f;
		destPercent = 0.0f;
		toolCheckTimer = 0.0f;
	}*/
}

void LoadScene::render()
{
	DRAW->render(LOAD->getLoadImageKey(), VEC2(WINSIZEX, WINSIZEY), 
		VEC2(WINSIZEX * 0.5f, WINSIZEY * 0.5f), COLR(1, 1, 1, 1), VEC3(0, 0, 0), false);

	D2D->renderRect(D2D_RectMake(0, WINSIZEY - 120, WINSIZEX, WINSIZEY), ColorF(0, 0, 0, 1), true);
	D2D->renderRect(D2D_RectMake(0, 599, percent, 601), ColorF(1, 1, 0, 0.8f), true);

	//TEXT->TextRender("LOADING_TOOLTIP", 0, WINSIZEY - 80, WINSIZEX, 80, ColorF(1, 1, 1), DWRITE_TEXT_ALIGNMENT_CENTER);
}
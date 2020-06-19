#include "stdafx.h"
#include "LoadScene.h"

LoadScene::LoadScene()
{
	//TEXT->Create_TextField("LOADING_PERCENT", L"�������", "", 40);
	//TEXT->Create_TextField("LOADING_TOOLTIP", L"�������", "", 25);

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
			//TEXT->Change_Text("LOADING_TOOLTIP", "��Ƽ�� (OPTICAL) ��������, ġ��Ȯ���� ġ������� ���������� �����Ÿ��� ������ŵ�ϴ�.");
			break;

		case 1:
			//TEXT->Change_Text("LOADING_TOOLTIP", "���������� ������ ���� ������ �ƴմϴ�.");
			break;

		case 2:
			//TEXT->Change_Text("LOADING_TOOLTIP", "�̿��� (EOTEC) ��������, ȭ�°� ���߷��� �ʿ��� ������������ �����ϴ�.");
			break;

		case 3:
			//TEXT->Change_Text("LOADING_TOOLTIP", "SR ������������ ������ ����ȭ�°� ���߷��Դϴ�. ������ ���� �����ϴ�.");
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
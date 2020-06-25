#include "stdafx.h"
#include "ChapterScene.h"

ChapterScene::ChapterScene()
{
	mButton.insert(make_pair("Return", Button(25, 50, 20, 50, ReturnLobbyButton)));

	mButton.insert(make_pair("Chapter1", Button(25, 50, 20, 50, Chapter1Button)));
	mButton.insert(make_pair("Chapter2", Button(25, 50, 20, 50, Chapter2Button)));
	mButton.insert(make_pair("Chapter3", Button(25, 50, 20, 50, Chapter3Button)));
	mButton.insert(make_pair("Chapter4", Button(25, 50, 20, 50, Chapter4Button)));

	curImageKey = "ChapterImage_1";
	moveDist = 0.0f;
	bkAlpha = 0.0f;
	position = D3DXVECTOR2(0, 0);
}

ChapterScene::~ChapterScene()
{
}

void ChapterScene::init()
{
	worldColor.a = 0.0f;
	mButton["Return"].box = D2D_RectMakeCenter(100, 60, 80, 40);

	mButton["Chapter1"].box = D2D_RectMakeCenter(400, 200, 300, 50);
	mButton["Chapter2"].box = D2D_RectMakeCenter(400, 350, 300, 50);
	mButton["Chapter3"].box = D2D_RectMakeCenter(400, 500, 300, 50);
	mButton["Chapter4"].box = D2D_RectMakeCenter(400, 650, 300, 50);

	curImageKey = "ChapterImage_1";
	moveDist = 0.0f;
	bkAlpha = 0.0f;
	moveAngle = 0.0f;
	position = D3DXVECTOR2(0, 0);

	SOUND->Play_Sound(SOUND_CHANNEL::CH_SOUND1, "ChapterLoop");
	SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);
	worldColor.a = 0.0f;

	isSceneChanged = false;
}

void ChapterScene::release()
{
}

void ChapterScene::update()
{
	//	static float DebugX = 850.0f, DebugY = 350.0f;
	//	ImGui::DragFloat("DebugX", &DebugX, 0.1f, 0, WINSIZEX);
	//	ImGui::DragFloat("DebugY", &DebugY, 0.1f, 0, WINSIZEX);

	if (isSceneChanged)
	{
		if (worldColor.a > 0.0f)
		{
			worldColor.a -= DELTA();
			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, worldColor.a > 0.15f ? 0.15f : worldColor.a);
		}

		else
		{
			worldColor.a = 0.0f;

			SOUND->Stop_Sound(SOUND_CHANNEL::CH_SOUND1, "ChapterLoop");
			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);

			/*if (SceneKey.compare("LOBBY") == 0)
				SOUNDMANAGER->Play_Sound(SOUND_CHANNEL::CH_SOUND1, "LobbyLoop", 0.5f);*/

			SCENE->changeScene(SceneKey);
			SCENE->initScene(SceneKey);
		}
	}

	else
	{
		flow_Background_Image();

		if (worldColor.a < 1.0f)
		{
			worldColor.a += DELTA();
			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, worldColor.a < 0.15f ? worldColor.a : 0.15f);
		}

		else
		{
			worldColor.a = 1.0f;

			if (KEYMANAGER->isKeyDown(VK_LBUTTON))
			{
				for (auto& it : mButton)
				{
					if (ptInRect(it.second.box, g_ptMouse))
						it.second.ClickAction(this);
				}
			}
		}
	}
}

void ChapterScene::render()
{
	DRAW->render(curImageKey, VEC2(320, 512), position, D3DXCOLOR(1, 1, 1, bkAlpha));

	for (auto& it : mButton)
		D2D->renderRect(it.second.box, ColorF(0, 0.8, 0.0));
}

void ChapterScene::flow_Background_Image()
{
	if (moveDist < 0.1f)
	{
		if (bkAlpha > 0.0f)
			bkAlpha -= DELTA() * 0.1f;
		else
		{
			bkAlpha = 0.0f;

			//	Moving Dist Setting
			moveDist = (rand() % 500) + 250;

			//	Image Change
			switch (rand() % 5)
			{
			case 0:
				curImageKey = "ChapterImage_1";
				break;

			case 1:
				curImageKey = "ChapterImage_2";
				break;

			case 2:
				curImageKey = "ChapterImage_3";
				break;

			case 3:
				curImageKey = "ChapterImage_4";
				break;

			case 4:
				curImageKey = "ChapterImage_5";
				break;
			}

			if (rand() % 2 == 0)
			{
				position.y = WINSIZEY;
				moveAngle = 90.0f;
			}
			else
			{
				position.y = 0;
				moveAngle = 270.0f;
			}

			position.x = rand() % (WINSIZEX - 400) + 200;

		}
	}

	else
	{
		if (bkAlpha < 0.5f)
			bkAlpha += DELTA() * 0.1f;

		else
		{
			bkAlpha = rand() % 100;
			bkAlpha *= 0.01f;
		}

		position.x += cosf(RAD(moveAngle)) * DELTA() * 50.0f;
		position.y -= sinf(RAD(moveAngle)) * DELTA() * 50.0f;

		moveDist -= DELTA() * 50.0f;
	}
}

void ChapterScene::ReturnLobbyButton(void * obj)
{
	ChapterScene* object = (ChapterScene*)obj;

	object->SceneKey = "LOBBY";
	object->isSceneChanged = true;
}

void ChapterScene::Chapter1Button(void * obj)
{
	ChapterScene* object = (ChapterScene*)obj;
	object->SceneKey = "STORY";
	object->isSceneChanged = true;

	LOAD->setAutoInit(true);
	LOAD->setStoryScriptPath("ChapterScript_0_0");
	LOAD->setNextScene("STORY");
	//LOADMANAGER->setLoadImageKey("LoadBK_Test");
}

void ChapterScene::Chapter2Button(void * obj)
{
	ChapterScene* object = (ChapterScene*)obj;
	object->SceneKey = "WORLD";
	object->isSceneChanged = true;

	MAP->Load_MapFile("_TextTable/MapTestFile.txt");

	LOAD->setAutoInit(true);
	LOAD->setNextScene("WORLD");
}

void ChapterScene::Chapter3Button(void * obj)
{

}

void ChapterScene::Chapter4Button(void * obj)
{

}

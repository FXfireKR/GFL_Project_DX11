#include "stdafx.h"
#include "MainLoadScene.h"

MainLoadScene::MainLoadScene()
{
	bgmVolume = 0.0f;
	bgImageAlpha = 0.0f;
	increase = true;
}

MainLoadScene::~MainLoadScene()
{
}

void MainLoadScene::init()
{
	LOADMANAGER->Add_LoadTray("LoadBk_InEquip", "../../_Assets/CG/19wintercg7-1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 23
	LOADMANAGER->Add_LoadTray("LobbyBk", "../../_Assets/CG/17xhcg4-2.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 23
	LOADMANAGER->Add_LoadTray("LoadBK_Test", "../../_Assets/CG/17xh-shootingrange.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 23

	LOADMANAGER->Add_LoadTray("TitleLoop", "../../_SoundSource/TitleLoop.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	LOADMANAGER->setAutoInit(false);
	LOADMANAGER->setNextScene("LOGO");
	LOADMANAGER->setLoadImageKey("LoadBK");
	SCENE->Change_Scene("LOAD");
}

void MainLoadScene::release()
{
}

void MainLoadScene::update()
{
	if (!SOUNDMANAGER->isValidKey("TitleLoop"))
	{
		if (!SOUNDMANAGER->isPlay(SOUND_CHANNEL::CH_SOUND1, "TitleLoop"))
		{
			SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);
			SOUNDMANAGER->Play_Sound(SOUND_CHANNEL::CH_SOUND1, "TitleLoop", 0.5f);

			bgmVolume = BGM_VOLUME_START;
		}
	}
	
	if (bgmVolume < BGM_VOLUME_MAX)
		bgmVolume += DELTA > 0.02f ? 0.02f : DELTA;

	else
		bgmVolume = BGM_VOLUME_MAX;

	if (increase)
	{
		if (bgImageAlpha < 1.0f)
			bgImageAlpha += DELTA > 0.02f ? 0.02f : DELTA;

		else
		{
			bgImageAlpha = 1.0f;
			increase = false;
		}
	}
	else
	{
		if (bgImageAlpha > 0.0f)
			bgImageAlpha -= DELTA > 0.02f ? 0.02f : DELTA;

		else
		{
			bgImageAlpha = 0.0f;
			increase = true;
		}
	}


	SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, bgmVolume);
}

void MainLoadScene::render()
{
	DRAW->render("LoadBK", DV2(WINSIZEX*0.5f, WINSIZEY*0.5f), DV2(WINSIZEX*0.5f, WINSIZEY*0.5f));

	TEXT->Change_Text("LOADING_PERCENT", "게임시작");
	TEXT->TextRender("LOADING_PERCENT",0, WINSIZEY - 150, WINSIZEX, 150, ColorF(1, 1, 1, bgImageAlpha), DWRITE_TEXT_ALIGNMENT_CENTER);
}
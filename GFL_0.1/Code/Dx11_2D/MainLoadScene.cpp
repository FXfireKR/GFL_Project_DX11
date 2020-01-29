#include "stdafx.h"
#include "MainLoadScene.h"

MainLoadScene::MainLoadScene()
{
	bgmVolume = 0.0f;
	bgImageAlpha = 0.0f;
	LogoAlpha = 0.0f;

	increase = true;

	BGM_VOLUME_SPEED = DELTA * 0.1f;
	ALPHA_SPEED = DELTA * 0.8f;

	uiAtlasLoader* loader = new uiAtlasLoader;
	loader->LoaduiAtlasData("../../_TextTable/LoginUi.atlas");
	loader->LoaduiAtlasData("../../_TextTable/LobbyUi.atlas");
	delete loader;
}

MainLoadScene::~MainLoadScene()
{
}

void MainLoadScene::init()
{
	LOADMANAGER->Add_LoadTray("LoadBk_InEquip", "../../_Assets/CG/19wintercg7-1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 23
	LOADMANAGER->Add_LoadTray("LobbyBk", "../../_Assets/CG/17xhcg4-2.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);					// 23
	LOADMANAGER->Add_LoadTray("LoadBK_Test", "../../_Assets/CG/17xh-shootingrange.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 23
	LOADMANAGER->Add_LoadTray("LobbyBackGround", "../../_Assets/Texture2D/LobbyBG.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 23

	LOADMANAGER->Add_LoadTray("LobbyLoop", "../../_SoundSource/LobbyLoop.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
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
	
	BGM_VOLUME_SPEED = DELTA * 0.1f;
	ALPHA_SPEED = DELTA * 0.25f;

	if (LogoAlpha < 1.0f)
		LogoAlpha += DELTA > 0.02f ? 0.02f : DELTA;
	else
		LogoAlpha = 1.0f;

	if (bgmVolume < BGM_VOLUME_MAX)
		bgmVolume += BGM_VOLUME_SPEED > 0.02f ? 0.02f : BGM_VOLUME_SPEED;

	else
		bgmVolume = BGM_VOLUME_MAX;

	if (increase)
	{
		if (bgImageAlpha < BUTTON_ALPHA_MAX)
			bgImageAlpha += ALPHA_SPEED > 0.02f ? 0.02f : ALPHA_SPEED;

		else
		{
			bgImageAlpha = BUTTON_ALPHA_MAX;
			increase = false;
		}
	}
	else
	{
		if (bgImageAlpha > BUTTON_ALPHA_MIN)
			bgImageAlpha -= ALPHA_SPEED > 0.02f ? 0.02f : ALPHA_SPEED;

		else
		{
			bgImageAlpha = BUTTON_ALPHA_MIN;
			increase = true;
		}
	}

	ImGui::Text("BGM Volume : %.2f", bgmVolume);

	SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, bgmVolume);

	if (KEYMANAGER->isKeyUp(VK_LBUTTON))
	{
		SCENE->Change_Scene("LOBBY");
		SCENE->Init_Scene();

		SOUNDMANAGER->Stop_Sound(SOUND_CHANNEL::CH_SOUND1, "TitleLoop");

		if (!SOUNDMANAGER->isValidKey("LobbyLoop"))
		{
			if (!SOUNDMANAGER->isPlay(SOUND_CHANNEL::CH_SOUND1, "LobbyLoop"))
			{
				SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.15f);
				SOUNDMANAGER->Play_Sound(SOUND_CHANNEL::CH_SOUND1, "LobbyLoop", 0.5f);
			}
		}
	}
}

void MainLoadScene::render()
{
	DRAW->render("LoadBK", DV2(WINSIZEX*0.5f, WINSIZEY*0.5f), DV2(WINSIZEX*0.5f, WINSIZEY*0.5f));

	uiAtlas atlas = IMAGEMAP->getUiAtlas("GameStart");
	DRAW->render(atlas.textureKey, atlas.alphaTexKey, DV2(200, 50), DV2(WINSIZEX*0.5f, 100), atlas.mixTexCoord, atlas.maxTexCoord, DCR(1, 1, 1, bgImageAlpha));

	atlas = IMAGEMAP->getUiAtlas("LogoImage");
	DRAW->render(atlas.textureKey, atlas.alphaTexKey, DV2(150, 60), DV2(150, WINSIZEY - 60), atlas.mixTexCoord, atlas.maxTexCoord, DCR(1, 1, 1, LogoAlpha));
}
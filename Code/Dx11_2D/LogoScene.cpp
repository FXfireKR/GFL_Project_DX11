#include "stdafx.h"
#include "LogoScene.h"

LogoScene::LogoScene()
{
	bgmVolume = 0.0f;
	bgImageAlpha = 0.0f;
	LogoAlpha = 0.0f;

	increase = true;
	beenClicked = false;

	BGM_VOLUME_SPEED = DELTA() * 0.1f;
	ALPHA_SPEED = DELTA() * 0.8f;

	//	Atlas Loading being used in Atlas parts
	uiAtlasLoader::LoadUiAtlasData("LoginUi.atlas");
	uiAtlasLoader::LoadUiAtlasData("LobbyUi.atlas");
	uiAtlasLoader::LoadUiAtlasData("GuidUi.atlas");
	uiAtlasLoader::LoadUiAtlasData("AvgUi.atlas");
	uiAtlasLoader::LoadUiAtlasData("SlotUi.atlas");
	uiAtlasLoader::LoadUiAtlasData("CommonUi.atlas");
	uiAtlasLoader::LoadUiAtlasData("HomeUi.atlas");
	uiAtlasLoader::LoadUiAtlasData("DeployUi.atlas");

	PLAYER->testCreate();
}

LogoScene::~LogoScene()
{
}

void LogoScene::init()
{
	//	캐릭터들의 사운드 리스트 받아오기
	for (auto& it : PLAYER->getPlayerTaticDoll().getAllDolls())
		it.second->LoadTray_SoundList();

	LOAD->Add_LoadTray("ShootRange", "CG/17xh-shootingrange.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// 사격장, 배치 로드 이미지
	LOAD->Add_LoadTray("LobbyBackGround", "Texture2D/LobbyBG.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// 로비 메인화면

	LOAD->Add_LoadTray("chapter_base_0_font", "Texture2D/chapter_base_0_font.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);	// 
	LOAD->Add_LoadTray("chapter_base_1_font", "Texture2D/chapter_base_1_font.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);	// 

	LOAD->Add_LoadTray("chapter1_base", "Texture2D/chapter1_base.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 
	//LOAD->Add_LoadTray("ChapterImage_1", "Texture2D/ChapterImage_1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);	// 
	//LOAD->Add_LoadTray("ChapterImage_1", "Texture2D/ChapterImage_1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);	// 
	//LOAD->Add_LoadTray("ChapterImage_1", "Texture2D/ChapterImage_1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);	// 

	LOAD->Add_LoadTray("ChapterImage_1", "Texture2D/ChapterImage_1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 
	LOAD->Add_LoadTray("ChapterImage_2", "Texture2D/ChapterImage_2.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 
	LOAD->Add_LoadTray("ChapterImage_3", "Texture2D/ChapterImage_3.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 
	LOAD->Add_LoadTray("ChapterImage_4", "Texture2D/ChapterImage_4.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 
	LOAD->Add_LoadTray("ChapterImage_5", "Texture2D/ChapterImage_5.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 

	LOAD->Add_LoadTray("gradiantBlack", "Texture2D/gradiantBlack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("AllCard", "Texture2D/AllCard.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("TurnBack", "Texture2D/TurnBack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOAD->Add_LoadTray("LobbyLoop", "LobbyLoop.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);					//	LobbyTheme
	LOAD->Add_LoadTray("TitleLoop", "TitleLoop.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);					//	TitleTheme
	LOAD->Add_LoadTray("ChapterLoop", "ChapterLobbyLoop.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);		//	ChapterTheme
	LOAD->Add_LoadTray("FormationLoop", "GF_Achieve_loop.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);		//	FormationTheme

	//DAMAGE->loadImageList();
	//DAMAGE->AllocateMemory();

	LOAD->setAutoInit(false);
	LOAD->setNextScene("LOGO");
	LOAD->setLoadImageKey("LoadLogo");
	LOAD->mallocThread();

	SCENE->changeScene("LOAD");
	
}

void LogoScene::release()
{
}

void LogoScene::update()
{
	CAMERA->setCameraFix(true);
	CAMERA->CameraReset();

	if (!SOUND->isValidKey("TitleLoop"))
	{
		if (!SOUND->isPlay(SOUND_CHANNEL::CH_SOUND1, "TitleLoop"))
		{
			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);
			SOUND->Play_Sound(SOUND_CHANNEL::CH_SOUND1, "TitleLoop", 0.0f);
			SOUND->setVolume("TitleLoop", SOUND_CHANNEL::CH_SOUND1, 0.0f);

			SINT randomUint = rand() % PLAYER->getPlayerTaticDoll().getAllDolls().size();
			SOUND->Play_Effect(SOUND_CHANNEL::CH_EFFECT, 
				(PLAYER->getPlayerTaticDoll().getAllDolls().at(randomUint))->keys.SOUND_TITLECALL, 0.2f);

			bgmVolume = BGM_VOLUME_START;
		}
	}

	BGM_VOLUME_SPEED = DELTA() * 0.1f;
	ALPHA_SPEED = DELTA() * 0.25f;

	if (LogoAlpha < 1.0f)
		LogoAlpha += DELTA() > 0.02f ? 0.02f : DELTA();
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

	SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, bgmVolume);
	SOUND->setVolume("TitleLoop", SOUND_CHANNEL::CH_SOUND1, bgmVolume);

	if (KEYMANAGER->isKeyUp(VK_LBUTTON))
	{
		if (!beenClicked)
			beenClicked = true;
	}

	if (beenClicked)
	{
		if (worldColor.a > 0.0f)
		{
			worldColor.a -= DELTA();
			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, worldColor.a > bgmVolume ? bgmVolume : worldColor.a);
			SOUND->setVolume("TitleLoop", SOUND_CHANNEL::CH_SOUND1, worldColor.a > bgmVolume ? bgmVolume : worldColor.a);
		}

		else
		{
			worldColor.a = 0.0f;

			SCENE->changeScene("LOBBY", true);

			SOUND->setVolume(CH_SOUND1, 0.0f);
			SOUND->setVolume("TitleLoop", CH_SOUND1, 0.0f);
			
			SOUND->Stop_Sound(CH_SOUND1, "TitleLoop");
		}
	}
	else
		worldColor.a = 1.0f;
}

void LogoScene::render()
{
	DRAW->render("LoadLogo", VEC2(WINSIZEX, WINSIZEY), VEC2(WINSIZEX*0.5f, WINSIZEY*0.5f));

	const uiAtlas* atlas = IMAGEMAP->getUiAtlas("GameStart");
	DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(200, 50), VEC2(WINSIZEX*0.5f, 620), 
		atlas->mixTexCoord, atlas->maxTexCoord, COLR(1, 1, 1, bgImageAlpha));

	atlas = IMAGEMAP->getUiAtlas("LogoImage");
	DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(150, 60), VEC2(150, 60),
		atlas->mixTexCoord, atlas->maxTexCoord, COLR(1, 1, 1, LogoAlpha));
}
#include "stdafx.h"
#include "LobbyScene.h"

LobbyScene::LobbyScene()
{
	TEXT->Create_TextField("Conversation", L"¸¼Àº°íµñ", "", 15);

	mButton.insert(make_pair("Combat", Button(25, 50, 20, 50, CombatButton)));
	mButton.insert(make_pair("Factory", Button(25, 50, 20, 50, FactoryButton)));
	mButton.insert(make_pair("Formation", Button(25, 50, 20, 50, FormationButton)));
	mButton.insert(make_pair("Research", Button(25, 50, 20, 50, ReserchButton)));
	mButton.insert(make_pair("AideConv", Button(25, 50, 20, 50, AideConverButton)));

	aideDoll = nullptr;
	isConvers = false;
	isSceneChanged = false;
}

LobbyScene::~LobbyScene()
{
}

void LobbyScene::init()
{
	worldColor.a = 0.0f;

	for (auto& it : mButton)
		it.second.boxImgColor = D3DXCOLOR(1, 1, 1, 1);

	mButton["Combat"].box = D2D_RectMakeCenter(950, 500, 90, 50);
	mButton["Formation"].box = D2D_RectMakeCenter(1150, 500, 90, 50);
	mButton["Research"].box = D2D_RectMakeCenter(950, 400, 80, 40);
	mButton["Factory"].box = D2D_RectMakeCenter(1150, 400, 80, 40);
	mButton["AideConv"].box = D2D_RectMakeCenter(AXIS_AIDECOV_BUTTON.x, AXIS_AIDECOV_BUTTON.y, 210, 450);

	aideDoll = ((PLAYER->getPlayerTaticDoll()).getAllTacDoll()).begin()->second;

	isConvers = false;
	isSceneChanged = false;
	ConvAlpha = 0.0f;

	SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);

	{
		//PLAYER->testFuc();
		//EFFECT->Init_EffectManager();

		//for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())		//	get List of Player's TacticalDoll
		//	it.second->LoadTray_List();										//	Add All Player's LoadTrayList

		//LOADMANAGER->setAutoInit(false);
		//LOADMANAGER->setNextScene("LOBBY");
		//LOADMANAGER->setLoadImageKey("LoadBK");
		//SCENE->Change_Scene("LOAD");
	}
}

void LobbyScene::release()
{
}

void LobbyScene::update()
{
	CAMERA->setCameraFix(true);
	CAMERA->CameraReset();

	if (isSceneChanged)
	{
		if (worldColor.a > 0.0f)
		{
			worldColor.a -= DELTA;
			SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, worldColor.a > 0.15f ? 0.15f : worldColor.a);

			ConvAlpha = worldColor.a > ConvAlpha ? ConvAlpha : worldColor.a;
		}

		else
		{
			worldColor.a = 0.0f;

			SOUNDMANAGER->Stop_Sound(SOUND_CHANNEL::CH_VOICE, curConvKey);
			SOUNDMANAGER->Stop_Sound(SOUND_CHANNEL::CH_SOUND1, "LobbyLoop");
			SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);

			SCENE->Change_Scene(SceneKey);
			SCENE->Init_Scene();
		}
	}

	else
	{	
		if (!SOUNDMANAGER->isValidKey("LobbyLoop"))
		{
			if (!SOUNDMANAGER->isPlay(SOUND_CHANNEL::CH_SOUND1, "LobbyLoop"))
			{
				SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);
				SOUNDMANAGER->Play_Sound(SOUND_CHANNEL::CH_SOUND1, "LobbyLoop", 0.0f);
				SOUNDMANAGER->setVolume("LobbyLoop", SOUND_CHANNEL::CH_SOUND1, 0.0f);
			}
		}

		if (worldColor.a < 1.0f)
		{
			worldColor.a += DELTA;

			SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, worldColor.a < 0.25f ? worldColor.a : 0.25f);
			SOUNDMANAGER->setVolume("LobbyLoop", SOUND_CHANNEL::CH_SOUND1, worldColor.a < 0.25f ? worldColor.a : 0.25f);
		}

		else
		{
			worldColor.a = 1.0f;

			SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.25f);
			SOUNDMANAGER->setVolume("LobbyLoop", SOUND_CHANNEL::CH_SOUND1, 0.25f);

			if (KEYMANAGER->isKeyDown(VK_LBUTTON))
			{
				for (auto& it : mButton)
				{
					if (ptInRect(it.second.box, g_ptMouse))
						it.second.ClickAction(this);
				}
			}

			else
			{
				for (auto& it : mButton)
				{
					if (ptInRect(it.second.box, g_ptMouse))
						it.second.boxImgColor.r = it.second.boxImgColor.g = it.second.boxImgColor.b = it.second.boxImgColor.a = 0.5f;
					else
						it.second.boxImgColor.r = it.second.boxImgColor.g = it.second.boxImgColor.b = it.second.boxImgColor.a = 1.0f;
				}
			}

			if (isConvers)
			{
				ConvAlpha = ConvAlpha < 1.0f ? ConvAlpha + DELTA * 5.0f : 1.0f;

				if (!SOUNDMANAGER->isPlay(SOUND_CHANNEL::CH_VOICE, curConvKey))
					isConvers = false;
			}
			else
				ConvAlpha = ConvAlpha > 0.0f ? ConvAlpha - DELTA * 10.0f : 0.0f;
		}
	}
}

void LobbyScene::render()
{
	DRAW->render("LobbyBackGround", DV2(WINSIZEX*0.5f, WINSIZEY*0.5f), DV2(WINSIZEX*0.5f, WINSIZEY*0.5f));

	DRAW->render(aideDoll->original_key, "alpha", DV2(512, 512), DV2(WINSIZEX*0.4f, WINSIZEY*0.8f));

	uiAtlas atlas = IMAGEMAP->getUiAtlas("Research");
	DRAW->render(atlas.textureKey, atlas.alphaTexKey, DV2(80, 40), DV2(mButton["Research"].box.left + 80, mButton["Research"].box.top + 40),
		atlas.mixTexCoord, atlas.maxTexCoord, mButton["Research"].boxImgColor);

	atlas = IMAGEMAP->getUiAtlas("Combat");
	DRAW->render(atlas.textureKey, atlas.alphaTexKey, DV2(90, 50), DV2(mButton["Combat"].box.left + 90, mButton["Combat"].box.top + 50),
		atlas.mixTexCoord, atlas.maxTexCoord, mButton["Combat"].boxImgColor);

	atlas = IMAGEMAP->getUiAtlas("Factory");
	DRAW->render(atlas.textureKey, atlas.alphaTexKey, DV2(80, 40), DV2(mButton["Factory"].box.left + 80, mButton["Factory"].box.top + 40),
		atlas.mixTexCoord, atlas.maxTexCoord, mButton["Factory"].boxImgColor);

	atlas = IMAGEMAP->getUiAtlas("Formation");
	DRAW->render(atlas.textureKey, atlas.alphaTexKey, DV2(90, 50), DV2(mButton["Formation"].box.left + 90, mButton["Formation"].box.top + 50),
		atlas.mixTexCoord, atlas.maxTexCoord, mButton["Formation"].boxImgColor);

	//	Conversation With AideDoll
	{
		atlas = IMAGEMAP->getUiAtlas("LobbyConv");
		DRAW->render(atlas.textureKey, atlas.alphaTexKey, DV2(180, 100), DV2(WINSIZEX*0.6f, 200), atlas.mixTexCoord, atlas.maxTexCoord, DCR(1, 1, 1, ConvAlpha));
		TEXT->TextRender("Conversation", (WINSIZEX*0.6f) - 160, 160, 330, 200, ColorF(1, 1, 0, ConvAlpha));
	}

	//for (auto& it : mButton)
	//	D2DX->renderRect(it.second.box, ColorF(0, 0.8, 0.0));	
}

//	Story Scene Action
/*
	LOADMANAGER->setAutoInit(true);
	LOADMANAGER->setStoryScriptPath("TestScript3");
	LOADMANAGER->setNextScene("STORY");
	LOADMANAGER->setLoadImageKey("LoadBK_Test");
	SCENE->Change_Scene("LOAD");
*/

//	World Scene Action
/*
	//LOADMANAGER->setAutoInit(true);
	//LOADMANAGER->setNextScene("WORLD");
	//LOADMANAGER->setLoadImageKey("LoadBK_Test");
	SCENE->Change_Scene("WORLD");
	SCENE->Init_Scene();
*/

//	Battle Scene Action
/*
	//LOADMANAGER->Add_LoadTray("arSound", "../../_SoundSource/Battle_AR.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	//LOADMANAGER->Add_LoadTray("mgSound1", "../../_SoundSource/Battle_170.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	//LOADMANAGER->Add_LoadTray("mgSound2", "../../_SoundSource/Battle_171.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	//for (auto& it : BDATA->getSquadSNV()->Call_Squad(1)->mSquad)
	//	it.second->LoadTray_List();

	//for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())		//	get List of Player's TacticalDoll
	//	it.second->LoadTray_List();

	//LOADMANAGER->Add_LoadTray(BTLMAP->imgKey, BTLMAP->imgPath, LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	//LOADMANAGER->setAutoInit(true);
	//LOADMANAGER->setNextScene("BATTLE");
	//LOADMANAGER->setLoadImageKey("LoadBK_Test");
	//SCENE->Change_Scene("LOAD");
*/


void LobbyScene::CombatButton(void * obj)
{
	LobbyScene* object = (LobbyScene*)obj;

	object->SceneKey = "CHAPTER";
	object->isSceneChanged = true;
}

void LobbyScene::FactoryButton(void * obj)
{
	LobbyScene* object = (LobbyScene*)obj;

	object->SceneKey = "EQUIP";
	object->isSceneChanged = true;
}

void LobbyScene::FormationButton(void * obj)
{
	LobbyScene* object = (LobbyScene*)obj;

	object->SceneKey = "SQUAD";
	object->isSceneChanged = true;
}

void LobbyScene::ReserchButton(void * obj)
{
}

void LobbyScene::AideConverButton(void * obj)
{
	LobbyScene* object = (LobbyScene*)obj;

	if (!object->isConvers)
	{
		int randomConvers = rand() % 3;
		switch (randomConvers)
		{
		case 0:
			SOUNDMANAGER->Play_Sound(SOUND_CHANNEL::CH_VOICE, object->aideDoll->SOUND_DIALOGUE1, 0.25f);
			object->curConvKey = object->aideDoll->SOUND_DIALOGUE1;
			break;

		case 1:
			SOUNDMANAGER->Play_Sound(SOUND_CHANNEL::CH_VOICE, object->aideDoll->SOUND_DIALOGUE2, 0.25f);
			object->curConvKey = object->aideDoll->SOUND_DIALOGUE2;
			break;

		case 2:
			SOUNDMANAGER->Play_Sound(SOUND_CHANNEL::CH_VOICE, object->aideDoll->SOUND_DIALOGUE3, 0.25f);
			object->curConvKey = object->aideDoll->SOUND_DIALOGUE3;
			break;
		}
	
		TEXT->Change_Text("Conversation", object->aideDoll->vecDialogue[randomConvers].c_str());
		object->isConvers = true;
	}
}

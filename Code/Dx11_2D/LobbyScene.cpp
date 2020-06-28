#include "stdafx.h"
#include "LobbyScene.h"

LobbyScene* LobbyScene::object = nullptr;

LobbyScene::LobbyScene()
{
	//DWRITE_FONT_WEIGHT_MEDIUM
	DWRITE->Create_TextField("Conversation", L"¸¼Àº°íµñ", "", 16, DWRITE_FONT_WEIGHT_MEDIUM);

	mButton.insert(make_pair("Combat", Button(25, 50, 20, 50, CombatButton)));
	mButton.insert(make_pair("Factory", Button(25, 50, 20, 50, FactoryButton)));
	mButton.insert(make_pair("Formation", Button(25, 50, 20, 50, FormationButton)));
	mButton.insert(make_pair("Research", Button(25, 50, 20, 50, ReserchButton)));
	mButton.insert(make_pair("AideConv", Button(25, 50, 20, 50, AideConverButton)));

	mButton.insert(make_pair("MakerIcon", Button(25, 50, 20, 50, AideConverButton)));
	mButton.insert(make_pair("SettingIcon", Button(25, 50, 20, 50, AideConverButton)));
	mButton.insert(make_pair("AideIcon", Button(25, 50, 20, 50, AideConverButton)));
	mButton.insert(make_pair("ViewIcon", Button(25, 50, 20, 50, AideConverButton)));

	aideDoll = nullptr;
	isConvers = false;
	isSceneChanged = false;

	curConvKey = SceneKey = "";
}

LobbyScene::~LobbyScene()
{
	this->release();
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

	//	Random Aid
	aideDoll = ((PLAYER->getPlayerTaticDoll()).getAllDolls()).begin()->second;

	isConvers = false;
	isSceneChanged = false;
	ConvAlpha = 0.0f;

	SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);

	{
		//PLAYER->testFuc();
		//EFFECT->Init_EffectManager();
		//
		//for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())		//	get List of Player's TacticalDoll
		//	it.second->LoadTray_List();										//	Add All Player's LoadTrayList
		//
		//LOADMANAGER->setAutoInit(false);
		//LOADMANAGER->setNextScene("LOBBY");
		//LOADMANAGER->setLoadImageKey("LoadBK");
		//SCENE->Change_Scene("LOAD");
	}
}

void LobbyScene::release()
{
	mButton.clear();
}

void LobbyScene::update()
{
	CAMERA->setCameraFix(true);
	CAMERA->CameraReset();

	//	@ Debug Lobby Main T-Doll

	//static UINT aidePtr = 0;

	//if (KEYMANAGER->isKeyDown(VK_RBUTTON)) {
	//	if (((PLAYER->getPlayerTaticDoll()).getAllDolls()).count(aidePtr))
	//		aideDoll = ((PLAYER->getPlayerTaticDoll()).getAllDolls()).at(aidePtr);
	//}

	//	=======================================

	if (isSceneChanged)
	{
		if (worldColor.a > 0.0f)
		{
			worldColor.a -= DELTA();
			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, worldColor.a > 0.15f ? 0.15f : worldColor.a);

			ConvAlpha = worldColor.a > ConvAlpha ? ConvAlpha : worldColor.a;
		}

		else
		{
			worldColor.a = 0.0f;

			SOUND->Stop_Sound(SOUND_CHANNEL::CH_VOICE, curConvKey);
			SOUND->Stop_Sound(SOUND_CHANNEL::CH_SOUND1, "LobbyLoop");
			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);

			SCENE->changeScene(SceneKey, true);
		}
	}

	else
	{
		if (!SOUND->isValidKey("LobbyLoop"))
		{
			if (!SOUND->isPlay(SOUND_CHANNEL::CH_SOUND1, "LobbyLoop"))
			{
				SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);
				SOUND->Play_Sound(SOUND_CHANNEL::CH_SOUND1, "LobbyLoop", 0.0f);
				SOUND->setVolume("LobbyLoop", SOUND_CHANNEL::CH_SOUND1, 0.0f);
			}
		}

		if (worldColor.a < 1.0f)
		{
			worldColor.a += DELTA();

			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, worldColor.a < 0.25f ? worldColor.a : 0.25f);
			SOUND->setVolume("LobbyLoop", SOUND_CHANNEL::CH_SOUND1, worldColor.a < 0.25f ? worldColor.a : 0.25f);
		}

		else
		{
			worldColor.a = 1.0f;

			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.25f);
			SOUND->setVolume("LobbyLoop", SOUND_CHANNEL::CH_SOUND1, 0.25f);

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
				ConvAlpha = ConvAlpha < 1.0f ? ConvAlpha + DELTA() * 5.0f : 1.0f;

				if (!SOUND->isPlay(SOUND_CHANNEL::CH_VOICE, curConvKey))
					isConvers = false;
			}
			else
				ConvAlpha = ConvAlpha > 0.0f ? ConvAlpha - DELTA() * 10.0f : 0.0f;
		}
	}
}

void LobbyScene::render()
{
	DRAW->render("LobbyBackGround", VEC2(WINSIZEX, WINSIZEY), VEC2(WINSIZEX*0.5f, WINSIZEY*0.5f));

	DRAW->render(aideDoll->keys.original_key, "alpha", VEC2(1024, 1024), VEC2(WINSIZEX*0.4f, WINSIZEY*0.8f));

	const uiAtlas* atlas = IMAGEMAP->getUiAtlas("Research");
	DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(80, 40), VEC2(mButton["Research"].box.left + 80, mButton["Research"].box.top + 40),
		atlas->mixTexCoord, atlas->maxTexCoord, mButton["Research"].boxImgColor);

	atlas = IMAGEMAP->getUiAtlas("Combat");
	DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(90, 50), VEC2(mButton["Combat"].box.left + 90, mButton["Combat"].box.top + 50),
		atlas->mixTexCoord, atlas->maxTexCoord, mButton["Combat"].boxImgColor);

	atlas = IMAGEMAP->getUiAtlas("Factory");
	DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(80, 40), VEC2(mButton["Factory"].box.left + 80, mButton["Factory"].box.top + 40),
		atlas->mixTexCoord, atlas->maxTexCoord, mButton["Factory"].boxImgColor);

	atlas = IMAGEMAP->getUiAtlas("Formation");
	DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(90, 50), VEC2(mButton["Formation"].box.left + 90, mButton["Formation"].box.top + 50),
		atlas->mixTexCoord, atlas->maxTexCoord, mButton["Formation"].boxImgColor);

	//	Conversation With AideDoll
	{
		atlas = IMAGEMAP->getUiAtlas("ConverTdollShape");
		DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(200, 100), VEC2(220, 200), 
			atlas->mixTexCoord, atlas->maxTexCoord, COLR(1, 1, 1, ConvAlpha));
		DWRITE->TextRender("Conversation", (220) - 180, 160, 310, 200, ColorF(1, 1, 0, ConvAlpha));

		//	Setting Icon
		atlas = IMAGEMAP->getUiAtlas("SettingIcon");
		DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(200, 100), VEC2(220, 200), 
			atlas->mixTexCoord, atlas->maxTexCoord);
		
		//	Maker Icon
		atlas = IMAGEMAP->getUiAtlas("MakerIcon");
		DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(200, 100), VEC2(220, 200), 
			atlas->mixTexCoord, atlas->maxTexCoord);

		//	View Icon
		atlas = IMAGEMAP->getUiAtlas("ViewIcon");
		DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(200, 100), VEC2(220, 200), 
			atlas->mixTexCoord, atlas->maxTexCoord);

		// Aide T-Doll Icon
		atlas = IMAGEMAP->getUiAtlas("AideIcon");
		DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(200, 100), VEC2(220, 200), 
			atlas->mixTexCoord, atlas->maxTexCoord);
	}

}

void LobbyScene::CombatButton(void * obj)
{
	object = (LobbyScene*)obj;

	//object->SceneKey = "TEST";
	object->SceneKey = "CHAPTER";
	object->isSceneChanged = true;
}

void LobbyScene::FactoryButton(void * obj)
{
	object = (LobbyScene*)obj;

	object->SceneKey = "EQUIP";
	object->isSceneChanged = true;
}

void LobbyScene::FormationButton(void * obj)
{
	object = (LobbyScene*)obj;

	object->SceneKey = "SQUAD";
	object->isSceneChanged = true;
}

void LobbyScene::ReserchButton(void * obj)
{
}

void LobbyScene::AideConverButton(void * obj)
{
	object = (LobbyScene*)obj;

	if (!object->isConvers)
	{
		int randomConvers = rand() % 3;
		switch (randomConvers)
		{
		case 0:
			SOUND->Play_Sound(SOUND_CHANNEL::CH_VOICE, object->aideDoll->keys.SOUND_DIALOGUE1, 0.25f);
			object->curConvKey = object->aideDoll->keys.SOUND_DIALOGUE1;
			break;

		case 1:
			SOUND->Play_Sound(SOUND_CHANNEL::CH_VOICE, object->aideDoll->keys.SOUND_DIALOGUE2, 0.25f);
			object->curConvKey = object->aideDoll->keys.SOUND_DIALOGUE2;
			break;

		case 2:
			SOUND->Play_Sound(SOUND_CHANNEL::CH_VOICE, object->aideDoll->keys.SOUND_DIALOGUE3, 0.25f);
			object->curConvKey = object->aideDoll->keys.SOUND_DIALOGUE3;
			break;
		}

		DWRITE->Change_Text("Conversation", object->aideDoll->keys.vecDialogue[randomConvers].c_str());
		object->isConvers = true;
	}
}

void LobbyScene::ChangeAideButton(void * obj)
{
	object = (LobbyScene*)obj;
}

void LobbyScene::SettingButton(void * obj)
{
}

void LobbyScene::ViewButton(void * obj)
{
}

void LobbyScene::MakerButton(void * obj)
{
}

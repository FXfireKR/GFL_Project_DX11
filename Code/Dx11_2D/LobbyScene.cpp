#include "stdafx.h"
#include "LobbyScene.h"

LobbyScene* LobbyScene::object = nullptr;

LobbyScene::LobbyScene()
{
	//DWRITE_FONT_WEIGHT_MEDIUM
	DWRITE->Create_TextField("Conversation", L"¸¼Àº°íµñ", "", 16, DWRITE_FONT_WEIGHT_MEDIUM);
	DWRITE->Create_TextField("ICONTITLE", L"¸¼Àº°íµñ", "", 20, DWRITE_FONT_WEIGHT_EXTRA_BOLD);

	mButton.insert(make_pair(SBUTTONS::COMBAT, Button(25, 50, 20, 50, CombatButton)));
	mButton.insert(make_pair(SBUTTONS::FORMATION, Button(25, 50, 20, 50, FormationButton)));
	mButton.insert(make_pair(SBUTTONS::RESEARCH, Button(25, 50, 20, 50, ReserchButton)));
	mButton.insert(make_pair(SBUTTONS::FACTORY, Button(25, 50, 20, 50, FactoryButton)));
	mButton.insert(make_pair(SBUTTONS::AIDECONV, Button(25, 50, 20, 50, AideConverButton)));

	mButton.insert(make_pair(SBUTTONS::SELECT_AIDE, Button(25, 50, 20, 50, ChangeAideButton)));
	mButton.insert(make_pair(SBUTTONS::SELECT_SET, Button(25, 50, 20, 50, SettingButton)));
	mButton.insert(make_pair(SBUTTONS::SELECT_DEV, Button(25, 50, 20, 50, MakerButton)));
	mButton.insert(make_pair(SBUTTONS::SELECT_VIEW, Button(25, 50, 20, 50, ViewButton)));
	
	saveAideID = 0;

	aideDoll = nullptr;
	isConvers = false;
	isSceneChanged = false;

	sceneState = STATE::NORMAL;

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

	mButton[SBUTTONS::COMBAT].box = D2D_RectMakeCenter(950, 500, 90, 50);
	mButton[SBUTTONS::FORMATION].box = D2D_RectMakeCenter(1150, 500, 90, 50);
	mButton[SBUTTONS::RESEARCH].box = D2D_RectMakeCenter(950, 400, 80, 40);
	mButton[SBUTTONS::FACTORY].box = D2D_RectMakeCenter(1150, 400, 80, 40);
	mButton[SBUTTONS::AIDECONV].box = D2D_RectMakeCenter(AXIS_AIDECOV_BUTTON.x, AXIS_AIDECOV_BUTTON.y, 210, 450);

	
	mButton[SBUTTONS::SELECT_SET].box = D2DRectMake(WINSIZEX - 100, WINSIZEY - 50, 100, 50);
	mButton[SBUTTONS::SELECT_DEV].box = D2DRectMake(WINSIZEX - 210, WINSIZEY - 50, 100, 50);
	mButton[SBUTTONS::SELECT_AIDE].box = D2DRectMake(WINSIZEX - 320, WINSIZEY - 50, 100, 50);

	mButton[SBUTTONS::SELECT_VIEW].box = D2DRectMake(1200, 50, 50, 50);

	if (PLAYER->getPlayerTaticDoll().getAllDolls().count(saveAideID))
		aideDoll = (PLAYER->getPlayerTaticDoll()).getAllDolls().at(saveAideID);
	else
		aideDoll = (PLAYER->getPlayerTaticDoll()).getAllDolls().begin()->second;

	sceneState = STATE::NORMAL;

	isConvers = false;
	isSceneChanged = false;
	ConvAlpha = 0.0f;

	SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);
	CAMERA->CameraReset();
}

void LobbyScene::release()
{
	mButton.clear();
}

void LobbyScene::update()
{
	CAMERA->setCameraFix(true);

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

			switch (sceneState)
			{
			case STATE::NONE:
				updateNone();
				break;

			case STATE::NORMAL:
				updateNormal();
				break;

			case STATE::AIDE:
				updateAide();
				break;

			case STATE::SETTING:
				updateSetting();
				break;

			case STATE::DEVLOPERS:
				updateDevlopers();
				break;
			}

			
		}
	}
}

void LobbyScene::render()
{
	DRAW->render("LobbyBackGround", VEC2(WINSIZEX, WINSIZEY), VEC2(WINSIZEX*0.5f, WINSIZEY*0.5f));

	switch (sceneState)
	{
	case STATE::NONE:
		renderNone();
		break;

	case STATE::NORMAL:
		renderNormal();
		break;

	case STATE::AIDE:
		renderAide();
		break;

	case STATE::SETTING:
		renderSetting();
		break;

	case STATE::DEVLOPERS:
		renderDevlopers();
		break;
	}

}

void LobbyScene::updateNone()
{
	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
		sceneState = STATE::NORMAL;
}

void LobbyScene::renderNone()
{
	DRAW->render(aideDoll->keys.original_key, "alpha", VEC2(1024, 1024), VEC2(WINSIZEX*0.4f, WINSIZEY*0.8f));
}

void LobbyScene::updateNormal()
{
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

void LobbyScene::renderNormal()
{
	DRAW->render(aideDoll->keys.original_key, "alpha", VEC2(1024, 1024), VEC2(WINSIZEX*0.4f, WINSIZEY*0.8f));

	const uiAtlas* atlas = IMAGEMAP->getUiAtlas("Research");
	DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(80, 40), 
		VEC2(mButton[SBUTTONS::RESEARCH].box.left + 80, mButton[SBUTTONS::RESEARCH].box.top + 40),
		atlas->mixTexCoord, atlas->maxTexCoord, mButton[SBUTTONS::RESEARCH].boxImgColor);

	atlas = IMAGEMAP->getUiAtlas("Combat");
	DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(90, 50), 
		VEC2(mButton[SBUTTONS::COMBAT].box.left + 90, mButton[SBUTTONS::COMBAT].box.top + 50),
		atlas->mixTexCoord, atlas->maxTexCoord, mButton[SBUTTONS::COMBAT].boxImgColor);

	atlas = IMAGEMAP->getUiAtlas("Factory");
	DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(80, 40), 
		VEC2(mButton[SBUTTONS::FACTORY].box.left + 80, mButton[SBUTTONS::FACTORY].box.top + 40),
		atlas->mixTexCoord, atlas->maxTexCoord, mButton[SBUTTONS::FACTORY].boxImgColor);

	atlas = IMAGEMAP->getUiAtlas("Formation");
	DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(90, 50), 
		VEC2(mButton[SBUTTONS::FORMATION].box.left + 90, mButton[SBUTTONS::FORMATION].box.top + 50),
		atlas->mixTexCoord, atlas->maxTexCoord, mButton[SBUTTONS::FORMATION].boxImgColor);


	//	Conversation With AideDoll
	{
		atlas = IMAGEMAP->getUiAtlas("ConverTdollShape");
		DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(200, 100), VEC2(220, 200),
			atlas->mixTexCoord, atlas->maxTexCoord, COLR(1, 1, 1, ConvAlpha));
		DWRITE->TextRender("Conversation", (220) - 180, 160, 310, 200, ColorF(1, 1, 0, ConvAlpha));

		//	Setting Icon
		atlas = IMAGEMAP->getUiAtlas("SettingIcon");
		//D2D->renderRect(mButton[SBUTTONS::SELECT_SET].box, ColorF(1, 0, 0), true);
		DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(50, 25), 
			VEC2(mButton[SBUTTONS::SELECT_SET].box.left + 50, mButton[SBUTTONS::SELECT_SET].box.top + 25),
			atlas->mixTexCoord, atlas->maxTexCoord);
		DWRITE->Change_Text("ICONTITLE", "¼³Á¤");
		DWRITE->TextRender("ICONTITLE", mButton[SBUTTONS::SELECT_SET].box.left + 45, 
			mButton[SBUTTONS::SELECT_SET].box.top + 12, ColorF(1, 1, 1, 1));

		//	Maker Icon
		atlas = IMAGEMAP->getUiAtlas("MakerIcon");
		//D2D->renderRect(mButton[SBUTTONS::SELECT_DEV].box, ColorF(1, 0, 0), true);
		DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(50, 25),
			VEC2(mButton[SBUTTONS::SELECT_DEV].box.left + 50, mButton[SBUTTONS::SELECT_DEV].box.top + 25),
			atlas->mixTexCoord, atlas->maxTexCoord);
		DWRITE->Change_Text("ICONTITLE", "°³¹ß");
		DWRITE->TextRender("ICONTITLE", mButton[SBUTTONS::SELECT_DEV].box.left + 45, 
			mButton[SBUTTONS::SELECT_DEV].box.top + 12, ColorF(1,1,1,1));

		// Aide T-Doll Icon
		atlas = IMAGEMAP->getUiAtlas("AideIcon");
		//D2D->renderRect(mButton[SBUTTONS::SELECT_AIDE].box, ColorF(1, 0, 0), true);
		DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(50, 25),
			VEC2(mButton[SBUTTONS::SELECT_AIDE].box.left + 50, mButton[SBUTTONS::SELECT_AIDE].box.top + 25),
			atlas->mixTexCoord, atlas->maxTexCoord);
		DWRITE->Change_Text("ICONTITLE", "ºÎ°ü");
		DWRITE->TextRender("ICONTITLE", mButton[SBUTTONS::SELECT_AIDE].box.left + 45, 
			mButton[SBUTTONS::SELECT_AIDE].box.top + 12, ColorF(1, 1, 1, 1));

		//	View Icon
		atlas = IMAGEMAP->getUiAtlas("ViewIcon");
		//D2D->renderRect(mButton[SBUTTONS::SELECT_VIEW].box, ColorF(1, 0, 0), true);
		DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(25, 25),
			VEC2(mButton[SBUTTONS::SELECT_VIEW].box.left + 25, mButton[SBUTTONS::SELECT_VIEW].box.top + 25),
			atlas->mixTexCoord, atlas->maxTexCoord);
	}
}

void LobbyScene::updateAide()
{
}

void LobbyScene::renderAide()
{
}

void LobbyScene::updateSetting()
{
}

void LobbyScene::renderSetting()
{
}

void LobbyScene::updateDevlopers()
{
}

void LobbyScene::renderDevlopers()
{
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
	object = (LobbyScene*)obj;
}

void LobbyScene::ViewButton(void * obj)
{
	object = (LobbyScene*)obj;

	if (object->sceneState == STATE::NORMAL)
		object->sceneState = STATE::NONE;
}

void LobbyScene::MakerButton(void * obj)
{
	object = (LobbyScene*)obj;
}

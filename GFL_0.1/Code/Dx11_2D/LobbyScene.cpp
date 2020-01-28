#include "stdafx.h"
#include "LobbyScene.h"

LobbyScene::LobbyScene()
{
}

LobbyScene::~LobbyScene()
{
}

void LobbyScene::init()
{
	mButton.insert(make_pair("EQUIP", Button(25, 50, 20, 50, EquipButton)));
	mButton.insert(make_pair("WORLD", Button(25, 50, 20, 50, WorldMapButton)));
	mButton.insert(make_pair("STORY", Button(25, 50, 20, 50, StoryButton)));
	mButton.insert(make_pair("WORLD2", Button(25, 50, 20, 50, WorldButton)));

	mButton["EQUIP"].box = D2D_RectMake(800, 250, 850, 270);
	mButton["WORLD"].box = D2D_RectMake(800, 350, 850, 370);
	mButton["STORY"].box = D2D_RectMake(800, 450, 850, 470);
	mButton["WORLD2"].box = D2D_RectMake(800, 550, 850, 570);


	//LOADMANAGER->Add_LoadTray("bk_1", "../../_Textures/bk_Streat.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 23
	//LOADMANAGER->Add_LoadTray("LoadBk_InEquip", "../../_Assets/CG/19wintercg7-1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 23
	//LOADMANAGER->Add_LoadTray("LobbyBk", "../../_Assets/CG/17xhcg4-2.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 23
	//LOADMANAGER->Add_LoadTray("LoadBK_Test", "../../_Assets/CG/17xh-shootingrange.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 23


	//LOADMANAGER->Add_LoadTray("shellEffect", "../../_Assets/Texture2D/fire1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE, 5, 1);		// test

	//PLAYER->testFuc();
	//EFFECT->Init_EffectManager();

	//for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())		//	get List of Player's TacticalDoll
	//	it.second->LoadTray_List();										//	Add All Player's LoadTrayList

	//LOADMANAGER->setAutoInit(false);
	//LOADMANAGER->setNextScene("LOBBY");
	//LOADMANAGER->setLoadImageKey("LoadBK");
	//SCENE->Change_Scene("LOAD");

}

void LobbyScene::release()
{
}

void LobbyScene::update()
{
	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		for (auto& it : mButton)
		{
			if (ptInRect(it.second.box, g_ptMouse))
				it.second.ClickAction(this);
		}
	}
}

void LobbyScene::render()
{
	DRAW->render("LobbyBk", DV2(WINSIZEX, WINSIZEY), DV2(WINSIZEX*0.5f, WINSIZEY*0.5f));

	D2DX->renderRect(mButton["EQUIP"].box, ColorF(0, 0.8, 0.0), true);
	D2DX->renderRect(mButton["WORLD"].box, ColorF(0, 0.8, 0.0), true);
	D2DX->renderRect(mButton["STORY"].box, ColorF(0, 0.8, 0.0), true);
	D2DX->renderRect(mButton["WORLD2"].box, ColorF(1, 0, 0.0), true);
}

void LobbyScene::EquipButton(void * obj)
{
	SCENE->Change_Scene("EQUIP");
	SCENE->Init_Scene();
}

void LobbyScene::WorldMapButton(void * obj)
{
	//EQUIP->ExitTray_EquipImage();

	LOADMANAGER->Add_LoadTray("arSound", "../../_SoundSource/Battle_AR.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray("mgSound1", "../../_SoundSource/Battle_170.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray("mgSound2", "../../_SoundSource/Battle_171.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	for (auto& it : BDATA->getSquadSNV()->Call_Squad(1)->mSquad)
		it.second->LoadTray_List();

	for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())		//	get List of Player's TacticalDoll
		it.second->LoadTray_List();

	LOADMANAGER->Add_LoadTray(BTLMAP->imgKey, BTLMAP->imgPath, LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOADMANAGER->setAutoInit(true);
	LOADMANAGER->setNextScene("BATTLE");
	LOADMANAGER->setLoadImageKey("LoadBK_Test");
	SCENE->Change_Scene("LOAD");
}

void LobbyScene::StoryButton(void* obj)
{
	LOADMANAGER->setAutoInit(true);
	LOADMANAGER->setStoryScriptPath("TestScript3");
	LOADMANAGER->setNextScene("STORY");
	LOADMANAGER->setLoadImageKey("LoadBK_Test");
	SCENE->Change_Scene("LOAD");
}

void LobbyScene::WorldButton(void* obj)
{
	//LOADMANAGER->setAutoInit(true);
	//LOADMANAGER->setNextScene("WORLD");
	//LOADMANAGER->setLoadImageKey("LoadBK_Test");
	SCENE->Change_Scene("WORLD");
	SCENE->Init_Scene();
}

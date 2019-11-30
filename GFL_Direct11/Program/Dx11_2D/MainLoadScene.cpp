 #include "stdafx.h"
#include "MainLoadScene.h"
#include "NTW_20.h"


MainLoadScene::MainLoadScene()
{
	image = new Image("LoadBK");
	image->setTrans(D3DXVECTOR2(WINSIZEX*0.5f, WINSIZEY*0.5f));
	//image->setScale(D3DXVECTOR2());
}

MainLoadScene::~MainLoadScene()
{
	if (image != nullptr)
	{
		image->release();
		SAFE_DEL(image);
	}
}

HRESULT MainLoadScene::init()
{
	//To Loading List
	//LOADMANAGER->setAutoInit(false);

	LOADMANAGER->setAutoInit(true);
	LOADMANAGER->setNextScene("TEST");
	LOADMANAGER->Add_LoadTray("bk_1", "../../_Textures/bk_Streat.ab",	LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 23

	PLAYER->testFuc();
	for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())
		it.second->LoadTray_List();
	
	Jaeger::LoadTray();
	EFFECT->Init_EffectManager();

	//BDATA->getSquadSNV()->Insert_SquadMember(1, new AEGIS_GA);

	//LOADMANAGER->Add_LoadTray("Lobby", "../../_SoundSource/Lobby.ab",	LOADRESOURCE_TYPE::RESOURCE_SOUND);		// 23
	//LOADMANAGER->Add_LoadTray("Main", "../../_SoundSource/Main.ab",		LOADRESOURCE_TYPE::RESOURCE_SOUND);		// 11
	//LOADMANAGER->Add_LoadTray("ost", "../../_SoundSource/ost_1.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);			// 11

	return S_OK;
}

void MainLoadScene::release()
{
}

void MainLoadScene::update()
{
	LOADMANAGER->update();
	image->update();

	if (!LOADMANAGER->getAutoInit()) {
		if (LOADMANAGER->getLoadListSize() < 1)
		{
			SCENE->Change_Scene(LOADMANAGER->getNextScene());
			SCENE->Init_Scene();
		}
	}
}

void MainLoadScene::ImageUpdate()
{
	
}

void MainLoadScene::render()
{
	image->render();
}
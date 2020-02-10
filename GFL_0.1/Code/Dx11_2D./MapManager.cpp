#include "stdafx.h"
#include "MapManager.h"


MapManager::MapManager()
{
	pManager = nullptr;
	battleMap = nullptr;

	BTM_Loader* loader = new BTM_Loader;
	battleMap = loader->LoadBTM("../../_Assets/BattleMap/bg2.btm");

	//	require 
	//IMAGEMAP->InsertImage_FromeFile("bg2", "../../_Assets/bg2.png");

	SAFE_DELETE(loader);
}

MapManager::~MapManager()
{
	this->release();
}

HRESULT MapManager::init()
{
	//LOADMANAGER->Add_LoadTray("N_Normal", "../../_Textures/Panel.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	//LOADMANAGER->Add_LoadTray("N_Heliport", "../../_Textures/Heliport.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	//LOADMANAGER->Add_LoadTray("N_HQ", "../../_Textures/HQ.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	//DRAW->LoadImage_d2d("N_Normal", L"../RemakeResource/MAP/Panel.png");
	//DRAW->LoadImage_d2d("N_Heliport", L"../RemakeResource/MAP/Heliport.png");
	//DRAW->LoadImage_d2d("N_HQ", L"../RemakeResource/MAP/HQ.png");

	//DRAW->LoadImage_d2d("S_Normal", L"../RemakeResource/MAP/Panel_S.png");
	//DRAW->LoadImage_d2d("S_Heliport", L"../RemakeResource/MAP/Heliport_S.png");
	//DRAW->LoadImage_d2d("S_HQ", L"../RemakeResource/MAP/HQ_S.png");

	//DRAW->LoadImage_d2d("G_Normal", L"../RemakeResource/MAP/Panel_G.png");
	//DRAW->LoadImage_d2d("G_Heliport", L"../RemakeResource/MAP/Heliport_G.png");
	//DRAW->LoadImage_d2d("G_HQ", L"../RemakeResource/MAP/HQ_G.png");

	//IMAGEMANAGER->addImage("SelPanel", "../Image/MapImgData/SelectPanel.bmp", PANELSIZE_X, PANELSIZE_Y, true, RGB(255, 0, 255));

	if (pManager == nullptr)
	{
		pManager = new PanelManager;
		pManager->init();
	}

	return S_OK;
}

void MapManager::release()
{
	if (pManager != nullptr)
	{
		pManager->Release_Panel();
		SAFE_DEL(pManager);
	}

	SAFE_DELETE(battleMap);
}

void MapManager::update()
{
	pManager->Update_Panel();
}

void MapManager::render()
{

	pManager->Render_Panel();

}

void MapManager::CreatePanel_()
{
	pManager->Create_Panel(g_ptMouse.x, g_ptMouse.y);
}

void MapManager::Check_ActionPoint()
{
	MaxActionPoint = pManager->Check_ActionPoint_For();
}

void MapManager::Load_MapFile(string path)
{
	pManager->LoadPanel_Data(path);
}
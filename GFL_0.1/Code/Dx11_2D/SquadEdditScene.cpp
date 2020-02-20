#include "stdafx.h"
#include "SquadEdditScene.h"

SquadEdditScene::SquadEdditScene()
{
}

SquadEdditScene::~SquadEdditScene()
{
}

void SquadEdditScene::init()
{
	worldColor.a = 0.0f;

	mode = SQUAD;

	FocusSquad = 1;
	FocusBox = -1;

	showEquip = false;

	Allocate_Box();
	
	//TEXT->Create_TextField("SQUAD", L"¸¼Àº°íµñ", "NULL", 28, DWRITE_FONT_WEIGHT_BOLD);

	mButton.insert(make_pair("INSERT", Button(270, 690, 100, 45, InsertSelect)));
	mButton.insert(make_pair("DELETE", Button(800, 690, 100, 45, DeleteSelect)));
	mButton.insert(make_pair("LEFT", Button(25, 350, 50, 50, LeftSelect)));
	mButton.insert(make_pair("RIGHT", Button(WINSIZEX - 75, 350, 50, 50, RightSelect)));

	//LOADMANAGER->Add_LoadTray("TEST_bk", L"../RemakeResource/sq.png");
	//LOADMANAGER->Add_LoadTray("card_bk", L"../RemakeResource/card_bk.png");
	//LOADMANAGER->Add_LoadTray("small_bk", L"../RemakeResource/small_bk.png");

	for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())		//	get List of Player's TacticalDoll
		it.second->LoadTray_ImageList();

	//	Loading Setting
	LOADMANAGER->setAutoInit(false);
	LOADMANAGER->setNextScene("SQUAD");
	LOADMANAGER->setLoadImageKey("LoadBk_InEquip");
	SCENE->Change_Scene("LOAD");
}

void SquadEdditScene::release()
{
}

void SquadEdditScene::update()
{
	worldColor.a = worldColor.a < 1.0f ? worldColor.a + DELTA : 1.0f;

	CAMERA->setCameraFix(true);
	CAMERA->CameraReset();

	switch (mode)
	{
	case ALL:
		All_Update();
		break;

	case SQUAD:
		Squad_Update();
		break;
	}
}

void SquadEdditScene::render()
{
	switch (mode)
	{
	case ALL:
		All_Render();
		break;

	case SQUAD:
		Squad_Render();
		break;
	}
}

void SquadEdditScene::Squad_Update()
{
	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		for (auto& it : mButton)		
		{
			if (ptInRect(it.second.box, g_ptMouse))
				it.second.ClickAction(this);		
		}

		for (int i = 0; i < vBox.size(); ++i)
		{
			if (PtInRect(&vBox.at(i), g_ptMouse))
				FocusBox = i;
		}
	}

	KeyInputAction();
}

void SquadEdditScene::Squad_Render()
{
	//	N Á¦´ë ÀÌ¹ÌÁö Ãâ·Â

	if (FocusBox != -1)
		D2DX->renderRect(vBox.at(FocusBox).left, vBox.at(FocusBox).top, vBox.at(FocusBox).right - vBox.at(FocusBox).left,
			vBox.at(FocusBox).bottom - vBox.at(FocusBox).top, ColorF(0, 1, 0));

	for (auto& it : mButton)
		D2DX->renderRect(it.second.box.left, it.second.box.top, it.second.box.right - it.second.box.left,
			it.second.box.bottom - it.second.box.top, ColorF(0.0, 0.8f, 0.0));

	for (auto& it : PLAYER->getPlayerSquad(FocusSquad)->mSquad)
	{
		int i = it.second->getID()->SquadMem_ID;
		float wid = vBox.at(i).right - vBox.at(i).left;
		float hei = vBox.at(i).bottom - vBox.at(i).top;

		DRAW->render(it.second->cardNormalKey, DV2(wid * 0.5f, hei * 0.5f), DV2(vBox.at(i).left + (wid * 0.5f), vBox.at(i).top + (hei * 0.5f)));
		//DRAW->render("card_bk", DV2(150, 450), DV3(vBox.at(i).left, vBox.at(i).top, 0));

		//DRAW->render(it.second->cardNormalKey, vBox.at(i).left + 1, vBox.at(i).top + 1, 148, 350, 0, 0, 256, 512);
		//DRAW->render("card_bk", vBox.at(i).left, vBox.at(i).top, 150, 450);	
		//DRAW->TextDraw_Center(it.second->getName(), L"³ª´®°íµñ", 20, vBox.at(i).left + 70, vBox.at(i).bottom - 112, { 255,255,255 });

		if (showEquip)
		{
			int sz = 0;

			for (auto& equip : it.second->p_getEquip())
			{
				if (equip.second != nullptr)
					equip.second->render(vBox.at(i).left + (vBox.at(i).right - vBox.at(i).left) / 2, 
						vBox.at(i).top + 50 + (sz * 120), 0.5F, 0.8F);
				++sz;
			}

		}
	}

	for (int i = 0; i < vBox.size(); ++i)
		D2DX->renderRect(vBox.at(i).left, vBox.at(i).top,
			vBox.at(i).right - vBox.at(i).left, vBox.at(i).bottom - vBox.at(i).top, ColorF(1, 0, 0));
}

void SquadEdditScene::All_Update()
{
	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < vBox.size(); ++i)
		{
			if (PtInRect(&vBox.at(i), g_ptMouse))
			{
				PLAYER->getPlayerTaticDoll().InsertSquad_IOPtacDoll(FocusSquad, i);
				mode = SHOWMODE::SQUAD;
				Allocate_Box();
				break;
			}
		}
	}
}

void SquadEdditScene::All_Render()
{
	for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())
	{
		int i = it.second->getID()->All_ID;

		//DRAW->render(it.second->cardNormalKey, DV2(200, 200), DV3(vBox.at(i).left, vBox.at(i).top, 0));
		//DRAW->render("small_bk", DV2(200, 200), DV3(vBox.at(i).left, vBox.at(i).top, 0));

		D2DX->renderRect(vBox.at(i).left, vBox.at(i).top, 
			vBox.at(i).right - vBox.at(i).left, vBox.at(i).bottom - vBox.at(i).top, ColorF(1, 0, 0));

		DRAW->render(it.second->cardNormalKey, DV2(100, 100), DV2(vBox.at(i).left + 100, vBox.at(i).top + 100));

		//DRAW->TextDraw_Center(it.second->getName(), L"³ª´®°íµñ", 20, vBox.at(i).left + 100, vBox.at(i).top + 155, { 255,255,255 });
	}
}

void SquadEdditScene::KeyInputAction()
{
	if (KEYMANAGER->isKeyDown(VK_SPACE))
	{
		LOADMANAGER->setAutoInit(true);
		LOADMANAGER->setNextScene("LOBBY");
		LOADMANAGER->setLoadImageKey("LoadBK_Test");
		SCENE->Change_Scene("LOAD");
		SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);
		worldColor.a = 0.0f;
	}
	

	//ÀåÂøµÈ Àåºñ È®ÀÎ
	if (KEYMANAGER->isKeyUp(VK_TAB))
		showEquip = showEquip ? false : true;

	/*if (KEYMANAGER->isKeyUp(VK_F2))
		SCENE->Change_Scene("Equip");*/
}

void SquadEdditScene::Allocate_Box()
{
	vBox.clear();

	//µå·ÎÀ× ¹Ú½º
	for (int i = 0; i < PLAYER->getPlayerSquad(FocusSquad)->mSquad.size(); ++i)
	{
		RECT rc;
		rc.left = 100 + (i * 170);
		rc.top = 180;
		rc.right = rc.left + 150;
		rc.bottom = rc.top + 450;

		vBox.push_back(rc);
	}
}

void SquadEdditScene::Allocate_Box_All()
{
	vBox.clear();

	//µå·ÎÀ× ¹Ú½º
	for (int i = 0; i < PLAYER->getPlayerTaticDoll().getTotalTacDollNum(); ++i)
	{
		RECT rc;
		rc.left = 50 + ((i % 5) * 220);
		rc.top = 200 + ((i / 5) * 220);
		rc.right = rc.left + 200;
		rc.bottom = rc.top + 200;

		vBox.push_back(rc);
	}
}

void SquadEdditScene::InsertSelect(void * obj)
{
	SquadEdditScene* object = (SquadEdditScene*)obj;

	object->FocusBox = -1;
	object->mode = SHOWMODE::ALL;
	object->showEquip = false;
	object->Allocate_Box_All();
}

void SquadEdditScene::DeleteSelect(void * obj)
{
	SquadEdditScene* object = (SquadEdditScene*)obj;

	if (object->FocusBox != -1)
	{
		if (PLAYER->getPlayerTaticDoll().getSquadMember(object->FocusSquad, object->FocusBox) != nullptr)
		{
			PLAYER->getPlayerTaticDoll().ExitSquad_IOPtacDoll(object->FocusSquad, object->FocusBox);

			object->vBox.pop_back();
			object->FocusBox = -1;
		}
	}
}

void SquadEdditScene::LeftSelect(void * obj)
{
	SquadEdditScene* object = (SquadEdditScene*)obj;

	if (object->FocusSquad > 1)
	{
		--(object->FocusSquad);
		object->Allocate_Box();
	}

	object->FocusBox = -1;
}

void SquadEdditScene::RightSelect(void * obj)
{
	SquadEdditScene* object = (SquadEdditScene*)obj;

	if (PLAYER->getPlayerSquad(object->FocusSquad + 1) != nullptr)
	{
		++(object->FocusSquad);
		object->Allocate_Box();
	}

	object->FocusBox = -1;
}


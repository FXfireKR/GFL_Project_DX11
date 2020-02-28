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
	LOADMANAGER->Add_LoadTray("Squad_1_Already", "../../_Assets/Texture2D/Squad1Already.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// �κ� ����ȭ��
	LOADMANAGER->Add_LoadTray("Squad_2_Already", "../../_Assets/Texture2D/Squad2Already.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// �κ� ����ȭ��
	LOADMANAGER->Add_LoadTray("Squad_3_Already", "../../_Assets/Texture2D/Squad3Already.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// �κ� ����ȭ��

	LOADMANAGER->Add_LoadTray("Squad_1_Leader", "../../_Assets/Texture2D/Squad1Leader.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// �κ� ����ȭ��
	LOADMANAGER->Add_LoadTray("Squad_2_Leader", "../../_Assets/Texture2D/Squad2Leader.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// �κ� ����ȭ��
	LOADMANAGER->Add_LoadTray("Squad_3_Leader", "../../_Assets/Texture2D/Squad3Leader.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// �κ� ����ȭ��

	LOADMANAGER->Add_LoadTray("SquadEmit", "../../_Assets/Texture2D/SquadEmit.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// �κ� ����ȭ��


	worldColor.a = 0.0f;

	mode = SQUAD;

	FocusSquad = 1;
	FocusBox = -1;

	showEquip = false;

	Allocate_Box();
	
	//TEXT->Create_TextField("SQUAD", L"�������", "NULL", 28, DWRITE_FONT_WEIGHT_BOLD);

	//mButton.insert(make_pair("INSERT", Button(270, 690, 100, 45, InsertSelect)));
	//mButton.insert(make_pair("DELETE", Button(800, 690, 100, 45, DeleteSelect)));
	//mButton.insert(make_pair("LEFT", Button(25, 350, 50, 50, LeftSelect)));
	//mButton.insert(make_pair("RIGHT", Button(WINSIZEX - 75, 350, 50, 50, RightSelect)));

	mButton.insert(make_pair("RETURN", Button(15, 10, 153, 126, ReturnBase_Select)));		//	������ ���� �Ǵ� ����â���� ����

	mButton.insert(make_pair("SQUAD",   Button(400, 50, 110, 50, ReturnBase_Select)));		//	�д� ����
	mButton.insert(make_pair("SQUADFS", Button(550, 50, 110, 50, ReturnBase_Select)));		//	ȭ������ �Ҵ� ����

	mButton.insert(make_pair("EQUIP", Button(550, 50, 110, 50, ReturnBase_Select)));		//	��� Ȯ�� ��ư

	mButton.insert(make_pair("SQUAD1", Button(15, 200, 150, 100, ChangeSquad_Select)));		//	1 ����
	mButton.insert(make_pair("SQUAD2", Button(15, 320, 150, 100, ChangeSquad_Select)));		//	2 ����
	mButton.insert(make_pair("SQUAD3", Button(15, 440, 150, 100, ChangeSquad_Select)));		//	3 ����

	mButton.insert(make_pair("CHARA1", Button(180,  200, 200, 456, InsertSelect)));			//	1�� ĳ����
	mButton.insert(make_pair("CHARA2", Button(400,  200, 200, 456, InsertSelect)));			//	2�� ĳ����
	mButton.insert(make_pair("CHARA3", Button(620,  200, 200, 456, InsertSelect)));			//	3�� ĳ����
	mButton.insert(make_pair("CHARA4", Button(840,  200, 200, 456, InsertSelect)));			//	4�� ĳ����
	mButton.insert(make_pair("CHARA5", Button(1060, 200, 200, 456, InsertSelect)));			//	5�� ĳ����


	vCharacter.clear();
	vCharacter.resize(5, "");

	//LOADMANAGER->Add_LoadTray("TEST_bk", L"../RemakeResource/sq.png");
	//LOADMANAGER->Add_LoadTray("card_bk", L"../RemakeResource/card_bk.png");
	//LOADMANAGER->Add_LoadTray("small_bk", L"../RemakeResource/small_bk.png");

	for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())		//	get List of Player's TacticalDoll
		it.second->LoadTray_ImageList();

	changeScene = false;

	//	Loading Setting
	LOADMANAGER->setAutoInit(false);
	LOADMANAGER->setNextScene("SQUAD");
	LOADMANAGER->setLoadImageKey("ShootRange");
	SCENE->Change_Scene("LOAD");
}

void SquadEdditScene::release()
{
}

void SquadEdditScene::update()
{
	CAMERA->setCameraFix(true);
	CAMERA->CameraReset();

	if (!changeScene)
	{
		worldColor.a = worldColor.a < 1.0f ? worldColor.a + DELTA : 1.0f;

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

	else
	{
		if (worldColor.a > 0.0f)
			worldColor.a -= DELTA * 2.0f;

		else
		{
			worldColor.a = 0.0f;

			LOADMANAGER->setAutoInit(true);
			LOADMANAGER->setNextScene("LOBBY");
			LOADMANAGER->setLoadImageKey("ShootRange");
			SCENE->Change_Scene("LOAD");
			SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);
		}
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
	}

	KeyInputAction();
}

void SquadEdditScene::Squad_Render()
{
	for (int i = 0; i < vCharacter.size(); ++i)
	{
		auto key = vCharacter[i];
		if (key.size() < 2) continue;

		float wid = 200;
		float hei = 456;

		DRAW->render(key, DV2(wid * 0.5f, hei * 0.5f), DV2(180 + (i * 220) + (wid * 0.5f), 200 + (hei * 0.5f)));
	}

	for (auto& it : mButton)
		D2DX->renderRect(it.second.box.left, it.second.box.top, it.second.box.right - it.second.box.left,
			it.second.box.bottom - it.second.box.top, ColorF(0.0, 0.8f, 0.0));
}

void SquadEdditScene::All_Update()
{
	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < vBox.size(); ++i)
		{
			if (PtInRect(&vBox.at(i), g_ptMouse))
			{
				vBox.clear();
				mode = SHOWMODE::SQUAD;

				if (i != 0)
				{
					PLAYER->getPlayerTaticDoll().InsertSquad_IOPtacDoll(FocusSquad, i - 1);
										
					for (int i = 0; i < vCharacter.size(); ++i)
					{
						if (i < PLAYER->getPlayerSquad(FocusSquad)->mSquad.size())
							vCharacter[i] = PLAYER->getPlayerSquad(FocusSquad)->mSquad[i]->cardNormalKey;

						else
							vCharacter[i] = "";
					}
				}

				else
				{
					if (PLAYER->getPlayerTaticDoll().getSquadMember(FocusSquad, FocusBox) != nullptr)
					{
						PLAYER->getPlayerTaticDoll().ExitSquad_IOPtacDoll(FocusSquad, FocusBox);

						
					}
				}

				break;
			}
		}
	}
}

void SquadEdditScene::All_Render()
{
	for (int i = 0; i < vBox.size(); ++i)
	{
		if (i != 0)
		{
			auto& it = PLAYER->getPlayerTaticDoll().getAllTacDoll().at(i - 1);
			int id = it->getID()->All_ID;

			D2DX->renderRect(vBox.at(i).left, vBox.at(i).top,
				vBox.at(i).right - vBox.at(i).left, vBox.at(i).bottom - vBox.at(i).top, ColorF(1, 0, 0));

			DRAW->render(it->cardNormalKey, DV2(80, 130), DV2(vBox.at(i).left + 80, vBox.at(i).top + 130));
		}

		else
			DRAW->render("SquadEmit", DV2(80, 130), DV2(vBox.at(i).left + 80, vBox.at(i).top + 130));
	}

	//for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())
	//{
	//	int i = it.second->getID()->All_ID;

	//	//DRAW->render(it.second->cardNormalKey, DV2(200, 200), DV3(vBox.at(i).left, vBox.at(i).top, 0));
	//	//DRAW->render("small_bk", DV2(200, 200), DV3(vBox.at(i).left, vBox.at(i).top, 0));

	//	D2DX->renderRect(vBox.at(i).left, vBox.at(i).top, 
	//		vBox.at(i).right - vBox.at(i).left, vBox.at(i).bottom - vBox.at(i).top, ColorF(1, 0, 0));

	//	DRAW->render(it.second->cardNormalKey, DV2(80, 130), DV2(vBox.at(i).left + 80, vBox.at(i).top + 130));

	//	//DRAW->TextDraw_Center(it.second->getName(), L"�������", 20, vBox.at(i).left + 100, vBox.at(i).top + 155, { 255,255,255 });
	//}
}

void SquadEdditScene::KeyInputAction()
{
	//������ ��� Ȯ��
	if (KEYMANAGER->isKeyUp(VK_TAB))
		showEquip = showEquip ? false : true;
}

void SquadEdditScene::Allocate_Box()
{
	vBox.clear();

	////����� �ڽ�
	//for (int i = 0; i < PLAYER->getPlayerSquad(FocusSquad)->mSquad.size(); ++i)
	//{
	//	RECT rc;
	//	rc.left = 100 + (i * 170);
	//	rc.top = 180;
	//	rc.right = rc.left + 150;
	//	rc.bottom = rc.top + 450;

	//	vBox.push_back(rc);
	//}
}

void SquadEdditScene::Allocate_Box_All()
{
	vBox.clear();

	//	100
	//	180	
	//	6 per Line

	//����� �ڽ�
	for (int i = 0; i < PLAYER->getPlayerTaticDoll().getTotalTacDollNum() + 1; ++i)
	{
		RECT rc = RectMakeCenter(200 + ((i % 6) * 170), 150 + ((i / 6) * 270), 80, 130);
		vBox.push_back(rc);
	}


	//for (int i = 0; i < PLAYER->getPlayerTaticDoll().getTotalTacDollNum(); ++i)
	//{
	//	RECT rc;
	//	rc.left = 50 + ((i % 5) * 220);
	//	rc.top = 200 + ((i / 5) * 220);
	//	rc.right = rc.left + 200;
	//	rc.bottom = rc.top + 200;

	//	vBox.push_back(rc);
	//}
}

void SquadEdditScene::InsertSelect(void * obj)
{
	SquadEdditScene* object = (SquadEdditScene*)obj;

	for (auto& it : object->mButton)
	{
		if (it.first.find("CHARA") == string::npos) continue;

		if (ptInRect(it.second.box, g_ptMouse))
		{
			if (it.first.find("1") != string::npos)
				object->FocusBox = 1;

			else if (it.first.find("2") != string::npos)
				object->FocusBox = 2;

			else if (it.first.find("3") != string::npos)
				object->FocusBox = 3;

			else if (it.first.find("4") != string::npos)
				object->FocusBox = 4;

			else if (it.first.find("5") != string::npos)
				object->FocusBox = 5;

		}
	}

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

void SquadEdditScene::ChangeSquad_Select(void * obj)
{
	SquadEdditScene* object = (SquadEdditScene*)obj;

	for (auto& it : object->mButton)
	{
		if (it.first.find("SQUAD") == string::npos) continue;

		if (ptInRect(it.second.box, g_ptMouse))
		{
			if (it.first.find("1") != string::npos)
				object->FocusSquad = 1;

			else if (it.first.find("2") != string::npos)
				object->FocusSquad = 2;

			else if (it.first.find("3") != string::npos)
				object->FocusSquad = 3;

			for (int i = 0; i < object->vCharacter.size(); ++i)
			{
				if (i < PLAYER->getPlayerSquad(object->FocusSquad)->mSquad.size())
					object->vCharacter[i] = PLAYER->getPlayerSquad(object->FocusSquad)->mSquad[i]->cardNormalKey;

				else
					object->vCharacter[i] = "";
			}
			break;
		}
	}

	
}

void SquadEdditScene::ReturnBase_Select(void * obj)
{
	SquadEdditScene* object = (SquadEdditScene*)obj;

	object->changeScene = true;
}
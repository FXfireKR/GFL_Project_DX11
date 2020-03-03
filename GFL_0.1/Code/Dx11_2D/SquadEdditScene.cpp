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
	LOADMANAGER->Add_LoadTray("Squad_1_Already", "../../_Assets/Texture2D/Squad1Already.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 로비 메인화면
	LOADMANAGER->Add_LoadTray("Squad_2_Already", "../../_Assets/Texture2D/Squad2Already.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 로비 메인화면
	LOADMANAGER->Add_LoadTray("Squad_3_Already", "../../_Assets/Texture2D/Squad3Already.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 로비 메인화면

	LOADMANAGER->Add_LoadTray("Squad_1_Leader", "../../_Assets/Texture2D/Squad1Leader.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// 로비 메인화면
	LOADMANAGER->Add_LoadTray("Squad_2_Leader", "../../_Assets/Texture2D/Squad2Leader.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// 로비 메인화면
	LOADMANAGER->Add_LoadTray("Squad_3_Leader", "../../_Assets/Texture2D/Squad3Leader.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// 로비 메인화면

	LOADMANAGER->Add_LoadTray("SquadEmit", "../../_Assets/Texture2D/SquadEmit.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);					// 로비 메인화면

	LOADMANAGER->Add_LoadTray("SquadBar", "../../_Assets/Texture2D/SquadBar.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);					// 로비 메인화면
	LOADMANAGER->Add_LoadTray("SquadBar_s", "../../_Assets/Texture2D/SquadBar_select.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// 로비 메인화면

	LOADMANAGER->Add_LoadTray("editSceneBk", "../../_Assets/Texture2D/editSceneBk.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);				// 로비 메인화면

	LOADMANAGER->Add_LoadTray("gradiantBlack", "../../_Assets/Texture2D/gradiantBlack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// 로비 메인화면
	LOADMANAGER->Add_LoadTray("SlotSquad", "../../_Assets/Texture2D/SlotSquad.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);					// 로비 메인화면

	LOADMANAGER->Add_LoadTray("HomeButton", "../../_Assets/Texture2D/HomeButton.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);				// 로비 메인화면
	LOADMANAGER->Add_LoadTray("AllCard", "../../_Assets/Texture2D/AllCard.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);						// 로비 메인화면

	worldColor.a = 0.0f;

	mode = SQUAD;

	FocusSquad = 1;
	FocusBox = -1;

	showEquip = false;

	Allocate_Box();
	
	TEXT->Create_TextField("SQUAD", L"맑은고딕", "NULL", 36, DWRITE_FONT_WEIGHT_BOLD);
	TEXT->Create_TextField("DOLLNAME", L"바탕", "NULL", 30, DWRITE_FONT_WEIGHT_EXTRA_BOLD);

	mButton.insert(make_pair("RETURN", Button(15, 10, 153, 126, ReturnBase_Select)));		//	기지로 복귀 또는 편집창으로 복귀

	//mButton.insert(make_pair("SQUAD",   Button(400, 50, 110, 50, ReturnBase_Select)));		//	분대 편집
	//mButton.insert(make_pair("SQUADFS", Button(550, 50, 110, 50, ReturnBase_Select)));		//	화력지원 소대 편집

	//mButton.insert(make_pair("EQUIP", Button(550, 50, 110, 50, ReturnBase_Select)));		//	장비 확인 버튼

	mButton.insert(make_pair("SQUAD1", Button(0, 200, 150, 100, ChangeSquad_Select)));		//	1 제대
	mButton.insert(make_pair("SQUAD2", Button(0, 320, 150, 100, ChangeSquad_Select)));		//	2 제대
	mButton.insert(make_pair("SQUAD3", Button(0, 440, 150, 100, ChangeSquad_Select)));		//	3 제대

	mButton.insert(make_pair("CHARA1", Button(180,  200, SQUAD_BOX_WIDTH, SQUAD_BOX_HEIGHT, InsertSelect)));			//	1번 캐릭터
	mButton.insert(make_pair("CHARA2", Button(400,  200, SQUAD_BOX_WIDTH, SQUAD_BOX_HEIGHT, InsertSelect)));			//	2번 캐릭터
	mButton.insert(make_pair("CHARA3", Button(620,  200, SQUAD_BOX_WIDTH, SQUAD_BOX_HEIGHT, InsertSelect)));			//	3번 캐릭터
	mButton.insert(make_pair("CHARA4", Button(840,  200, SQUAD_BOX_WIDTH, SQUAD_BOX_HEIGHT, InsertSelect)));			//	4번 캐릭터
	mButton.insert(make_pair("CHARA5", Button(1060, 200, SQUAD_BOX_WIDTH, SQUAD_BOX_HEIGHT, InsertSelect)));			//	5번 캐릭터


	vCharacter.clear();
	vCharacter.resize(5, "");

	for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())		//	get List of Player's TacticalDoll
		it.second->LoadTray_ImageList();

	changeScene = false;

	//	UnLoad List
	vLoadList.clear();
	for (auto& it : LOADMANAGER->getLoadList())
		vLoadList.push_back(LoadResourceData(it->key, it->type));

	for (int i = 0; i < vCharacter.size(); ++i)
	{
		if (i < PLAYER->getPlayerSquad(FocusSquad)->mSquad.size())
			vCharacter[i] = PLAYER->getPlayerSquad(FocusSquad)->mSquad[i]->cardNormalKey;

		else
			vCharacter[i] = "";
	}

	//	Loading Setting
	LOADMANAGER->setAutoInit(false);
	LOADMANAGER->setNextScene("SQUAD");
	LOADMANAGER->setLoadImageKey("ShootRange");
	SCENE->Change_Scene("LOAD");

	SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);
	SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);
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
		SOUNDMANAGER->Play_Sound(SOUND_CHANNEL::CH_SOUND1, "FormationLoop", 0.25f);
		SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, worldColor.a < 0.15f ? worldColor.a : 0.15f);

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

			for (auto& it : vLoadList)
			{
				switch (it.type)
				{
				case RESOURCE_IMAGE:
					IMAGEMAP->ReleaseMemory(it.resourceKey);
					break;
				}
			}

			SOUNDMANAGER->Stop_Sound(SOUND_CHANNEL::CH_SOUND1, "FormationLoop");
			SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);

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
	DRAW->render("editSceneBk", DV2(WINSIZEX*0.5f, WINSIZEY*0.5f), DV2(WINSIZEX*0.5f, WINSIZEY*0.5f));
	DRAW->render("gradiantBlack", DV2(WINSIZEX*0.5f, 75), DV2(WINSIZEX*0.5f, 75));

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

		DRAW->render(key, DV2(SQUAD_BOX_H_WIDTH, SQUAD_BOX_H_HEIGHT), 
			DV2(180 + (i * (SQUAD_BOX_H_WIDTH + 130) + SQUAD_BOX_H_WIDTH), 200 + SQUAD_BOX_H_HEIGHT));
	}

	//for (auto& it : mButton)
	//	D2DX->renderRect(it.second.box.left, it.second.box.top, it.second.box.right - it.second.box.left,
	//		it.second.box.bottom - it.second.box.top, ColorF(0.0, 0.8f, 0.0));

	DRAW->render("HomeButton", DV2(76, 63), DV2(mButton["RETURN"].box.left + 76, mButton["RETURN"].box.top + 63));

	DRAW->render(FocusSquad == 1 ? "SquadBar_s" : "SquadBar", DV2(75, 50), DV2(mButton["SQUAD1"].box.left + 75, mButton["SQUAD1"].box.top + 50));
	DRAW->render(FocusSquad == 2 ? "SquadBar_s" : "SquadBar", DV2(75, 50), DV2(mButton["SQUAD2"].box.left + 75, mButton["SQUAD2"].box.top + 50));
	DRAW->render(FocusSquad == 3 ? "SquadBar_s" : "SquadBar", DV2(75, 50), DV2(mButton["SQUAD3"].box.left + 75, mButton["SQUAD3"].box.top + 50));

	TEXT->Change_Text("SQUAD", "1제대");
	TEXT->TextRender("SQUAD", mButton["SQUAD1"].box.left, mButton["SQUAD1"].box.top+25, 150, 100,ColorF(0, 0, 0), DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	
	TEXT->Change_Text("SQUAD", "2제대");
	TEXT->TextRender("SQUAD", mButton["SQUAD2"].box.left, mButton["SQUAD2"].box.top+25, 150, 100, ColorF(0, 0, 0), DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);

	TEXT->Change_Text("SQUAD", "3제대");
	TEXT->TextRender("SQUAD", mButton["SQUAD3"].box.left, mButton["SQUAD3"].box.top+25, 150, 100, ColorF(0, 0, 0), DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	
	Squad_render_Character();
}

void SquadEdditScene::Squad_render_Character()
{
	D2DX->renderRect(mButton["CHARA1"].box.left, mButton["CHARA1"].box.top, mButton["CHARA1"].box.right - mButton["CHARA1"].box.left,
		mButton["CHARA1"].box.bottom - mButton["CHARA1"].box.top, ColorF(0, 0, 0));

	D2DX->renderRect(mButton["CHARA2"].box.left, mButton["CHARA2"].box.top, mButton["CHARA2"].box.right - mButton["CHARA2"].box.left,
		mButton["CHARA2"].box.bottom - mButton["CHARA2"].box.top, ColorF(0, 0, 0));

	D2DX->renderRect(mButton["CHARA3"].box.left, mButton["CHARA3"].box.top, mButton["CHARA3"].box.right - mButton["CHARA3"].box.left,
		mButton["CHARA3"].box.bottom - mButton["CHARA3"].box.top, ColorF(0, 0, 0));

	D2DX->renderRect(mButton["CHARA4"].box.left, mButton["CHARA4"].box.top, mButton["CHARA4"].box.right - mButton["CHARA4"].box.left,
		mButton["CHARA4"].box.bottom - mButton["CHARA4"].box.top, ColorF(0, 0, 0));

	D2DX->renderRect(mButton["CHARA5"].box.left, mButton["CHARA5"].box.top, mButton["CHARA5"].box.right - mButton["CHARA5"].box.left,
		mButton["CHARA5"].box.bottom - mButton["CHARA5"].box.top, ColorF(0, 0, 0));

	uiAtlas atlas = PLAYER->getPlayerSquad(FocusSquad)->mSquad.count(0) ? IMAGEMAP->getUiAtlas("InstOvSlot") : IMAGEMAP->getUiAtlas("InstSlot");
	DRAW->render(atlas.textureKey, atlas.alphaTexKey, DV2(SQUAD_BOX_H_WIDTH, SQUAD_BOX_H_HEIGHT),
		DV2(mButton["CHARA1"].box.left + SQUAD_BOX_H_WIDTH, mButton["CHARA1"].box.top + SQUAD_BOX_H_HEIGHT), atlas.mixTexCoord, atlas.maxTexCoord);

	atlas = PLAYER->getPlayerSquad(FocusSquad)->mSquad.count(1) ? IMAGEMAP->getUiAtlas("InstOvSlot") : IMAGEMAP->getUiAtlas("InstSlot");
	DRAW->render(atlas.textureKey, atlas.alphaTexKey, DV2(SQUAD_BOX_H_WIDTH, SQUAD_BOX_H_HEIGHT),
		DV2(mButton["CHARA2"].box.left + SQUAD_BOX_H_WIDTH, mButton["CHARA2"].box.top + SQUAD_BOX_H_HEIGHT), atlas.mixTexCoord, atlas.maxTexCoord);

	atlas = PLAYER->getPlayerSquad(FocusSquad)->mSquad.count(2) ? IMAGEMAP->getUiAtlas("InstOvSlot") : IMAGEMAP->getUiAtlas("InstSlot");
	DRAW->render(atlas.textureKey, atlas.alphaTexKey, DV2(SQUAD_BOX_H_WIDTH, SQUAD_BOX_H_HEIGHT),
		DV2(mButton["CHARA3"].box.left + SQUAD_BOX_H_WIDTH, mButton["CHARA3"].box.top + SQUAD_BOX_H_HEIGHT), atlas.mixTexCoord, atlas.maxTexCoord);

	atlas = PLAYER->getPlayerSquad(FocusSquad)->mSquad.count(3) ? IMAGEMAP->getUiAtlas("InstOvSlot") : IMAGEMAP->getUiAtlas("InstSlot");
	DRAW->render(atlas.textureKey, atlas.alphaTexKey, DV2(SQUAD_BOX_H_WIDTH, SQUAD_BOX_H_HEIGHT),
		DV2(mButton["CHARA4"].box.left + SQUAD_BOX_H_WIDTH, mButton["CHARA4"].box.top + SQUAD_BOX_H_HEIGHT), atlas.mixTexCoord, atlas.maxTexCoord);

	atlas = PLAYER->getPlayerSquad(FocusSquad)->mSquad.count(4) ? IMAGEMAP->getUiAtlas("InstOvSlot") : IMAGEMAP->getUiAtlas("InstSlot");
	DRAW->render(atlas.textureKey, atlas.alphaTexKey, DV2(SQUAD_BOX_H_WIDTH, SQUAD_BOX_H_HEIGHT),
		DV2(mButton["CHARA5"].box.left + SQUAD_BOX_H_WIDTH, mButton["CHARA5"].box.top + SQUAD_BOX_H_HEIGHT), atlas.mixTexCoord, atlas.maxTexCoord);

	DRAW->render("SlotSquad", DV2(SQUAD_BOX_H_WIDTH, SQUAD_BOX_H_HEIGHT * 0.37f),
		DV2(mButton["CHARA1"].box.left + SQUAD_BOX_H_WIDTH, mButton["CHARA1"].box.bottom - (SQUAD_BOX_H_HEIGHT * 0.37f)));
	DRAW->render("SlotSquad", DV2(SQUAD_BOX_H_WIDTH, SQUAD_BOX_H_HEIGHT * 0.37f),
		DV2(mButton["CHARA2"].box.left + SQUAD_BOX_H_WIDTH, mButton["CHARA2"].box.bottom - (SQUAD_BOX_H_HEIGHT * 0.37f)));
	DRAW->render("SlotSquad", DV2(SQUAD_BOX_H_WIDTH, SQUAD_BOX_H_HEIGHT * 0.37f),
		DV2(mButton["CHARA3"].box.left + SQUAD_BOX_H_WIDTH, mButton["CHARA3"].box.bottom - (SQUAD_BOX_H_HEIGHT * 0.37f)));
	DRAW->render("SlotSquad", DV2(SQUAD_BOX_H_WIDTH, SQUAD_BOX_H_HEIGHT * 0.37f),
		DV2(mButton["CHARA4"].box.left + SQUAD_BOX_H_WIDTH, mButton["CHARA4"].box.bottom - (SQUAD_BOX_H_HEIGHT * 0.37f)));
	DRAW->render("SlotSquad", DV2(SQUAD_BOX_H_WIDTH, SQUAD_BOX_H_HEIGHT * 0.37f),
		DV2(mButton["CHARA5"].box.left + SQUAD_BOX_H_WIDTH, mButton["CHARA5"].box.bottom - (SQUAD_BOX_H_HEIGHT * 0.37f)));

	for (auto& it : PLAYER->getPlayerSquad(FocusSquad)->mSquad)
	{
		string chara = "CHARA";

		TEXT->Change_Text("DOLLNAME", it.second->getName());
		
		chara += to_string(it.second->getID()->SquadMem_ID + 1);

		//D2DX->renderRect(mButton[chara].box.left, mButton[chara].box.bottom - (SQUAD_BOX_H_HEIGHT * 0.74f) + 28, 180, 40, ColorF(1, 0, 0));

		TEXT->TextRender("DOLLNAME", mButton[chara].box.left, mButton[chara].box.bottom - (SQUAD_BOX_H_HEIGHT * 0.74f) + 28, 180, 40,
			ColorF(1, 1, 1), DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	}
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
					if (PLAYER->getPlayerSquad(FocusSquad)->mSquad.count(FocusBox)) {}
					else
					{
						if (PLAYER->getPlayerSquad(FocusSquad)->mSquad.size() < 5)
						{
							if (PLAYER->getPlayerTaticDoll().InsertSquad_IOPtacDoll(FocusSquad, i - 1) != E_FAIL)
								SOUNDMANAGER->Play_Effect(CH_VOICE, PLAYER->getPlayerTaticDoll().getAllTacDoll().at(i - 1)->SOUND_FORMATION, 0.15f);
						}
					}
				}

				else
				{
					if (PLAYER->getPlayerTaticDoll().getSquadMember(FocusSquad, FocusBox) != nullptr)
						PLAYER->getPlayerTaticDoll().ExitSquad_IOPtacDoll(FocusSquad, FocusBox);
				}

				for (int i = 0; i < vCharacter.size(); ++i)
				{
					if (i < PLAYER->getPlayerSquad(FocusSquad)->mSquad.size())
						vCharacter[i] = PLAYER->getPlayerSquad(FocusSquad)->mSquad[i]->cardNormalKey;

					else
						vCharacter[i] = "";
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

			if (it->getID()->Squad_ID == -1)
				DRAW->render(it->cardNormalKey, DV2(ALL_BOX_H_WIDTH, ALL_BOX_H_HEIGH), 
					DV2(vBox.at(i).left + ALL_BOX_H_WIDTH, vBox.at(i).top + ALL_BOX_H_HEIGH));

			else
			{
				DRAW->render(it->cardNormalKey, DV2(ALL_BOX_H_WIDTH, ALL_BOX_H_HEIGH), 
					DV2(vBox.at(i).left + ALL_BOX_H_WIDTH, vBox.at(i).top + ALL_BOX_H_HEIGH), DCR(1, 1, 1, 0.5f));

				switch (it->getID()->Squad_ID)
				{
				case 1:
					DRAW->render("Squad_1_Already", DV2(31, 35), DV2(vBox.at(i).left + 35, vBox.at(i).bottom - 40));
					break;

				case 2:
					DRAW->render("Squad_2_Already", DV2(31, 35), DV2(vBox.at(i).left + 35, vBox.at(i).bottom - 40));
					break;

				case 3:
					DRAW->render("Squad_3_Already", DV2(31, 35), DV2(vBox.at(i).left + 35, vBox.at(i).bottom - 40));
					break;
				}
				
			}

			
		}

		else
			DRAW->render("SquadEmit", DV2(80, 130), DV2(vBox.at(i).left + 80, vBox.at(i).top + 130));
	}
}

void SquadEdditScene::KeyInputAction()
{
	//장착된 장비 확인
	if (KEYMANAGER->isKeyUp(VK_TAB))
		showEquip = showEquip ? false : true;
}

void SquadEdditScene::Allocate_Box()
{
	vBox.clear();
}

void SquadEdditScene::Allocate_Box_All()
{
	vBox.clear();

	//	100
	//	180	
	//	6 per Line

	//드로잉 박스
	for (int i = 0; i < PLAYER->getPlayerTaticDoll().getTotalTacDollNum() + 1; ++i)
	{
		RECT rc = RectMakeCenter(100 + ((i % 6) * 170), 300 + ((i / 6) * 270), 
			ALL_BOX_H_WIDTH, ALL_BOX_H_HEIGH);
		vBox.push_back(rc);
	}
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
				object->FocusBox = 0;

			else if (it.first.find("2") != string::npos)
				object->FocusBox = 1;

			else if (it.first.find("3") != string::npos)
				object->FocusBox = 2;

			else if (it.first.find("4") != string::npos)
				object->FocusBox = 3;

			else if (it.first.find("5") != string::npos)
				object->FocusBox = 4;

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
#include "stdafx.h"
#include "EquipScene.h"

EquipScene::EquipScene()
{
}

EquipScene::~EquipScene()
{
}

void EquipScene::init()
{
	worldColor.a = 0.0f;

	CharaSlider.InfoDollID = -1;
	CharaSlider.isMoving = false;
	CharaSlider.isOpen = false;
	CharaSlider.Opacity = 1.0f;
	CharaSlider.axisPos.x = -300;
	CharaSlider.axisPos.y = 180;

	for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())		//	get List of Player's TacticalDoll
		it.second->LoadTray_ImageList();

	TEXT->Create_TextField("CHARA_NAME", L"¸¼Àº°íµñ", "NULL", 28, DWRITE_FONT_WEIGHT_BOLD);
	TEXT->Create_TextField("EQUIP_EXP", L"¸¼Àº°íµñ", "NULL", 12, DWRITE_FONT_WEIGHT_NORMAL);
	TEXT->Create_TextField("STATUS_IDX", L"¸¼Àº°íµñ", "NULL", 14, DWRITE_FONT_WEIGHT_NORMAL);
	TEXT->Create_TextField("STATUS_VAL", L"¸¼Àº°íµñ", "NULL", 10, DWRITE_FONT_WEIGHT_NORMAL);

	mButton.insert(make_pair("CHARA", Button(25, 50, 20, 50, CharacterSelect)));
	mButton.insert(make_pair("SLIDER", Button(25, 50, 20, 50, SlideOpener)));
	mButton.insert(make_pair("CANCLE", Button(25, 50, 20, 50, ReturnSelect)));

	mButton.insert(make_pair("EQUIP_1", Button(25, 50, 20, 50, EquipmentSelect)));
	mButton.insert(make_pair("EQUIP_2", Button(25, 50, 20, 50, EquipmentSelect)));
	mButton.insert(make_pair("EQUIP_3", Button(25, 50, 20, 50, EquipmentSelect)));

	CharaInfoBox = D2D_RectMake(CharaSlider.axisPos.x - 300, CharaSlider.axisPos.y - 180, CharaSlider.axisPos.x + 300, CharaSlider.axisPos.y + 180);

	mButton["CHARA"].box = D2D_RectMake(CharaSlider.axisPos.x - 290, CharaSlider.axisPos.y - 170,
		CharaSlider.axisPos.x - 140, CharaSlider.axisPos.y + 170);

	mButton["SLIDER"].box = D2D_RectMake(CharaSlider.axisPos.x + 300, CharaSlider.axisPos.y - 25,
		CharaSlider.axisPos.x + 320, CharaSlider.axisPos.y + 25);

	mButton["CANCLE"].box = D2D_RectMake(WINSIZEX - 200, 10, WINSIZEX - 100, 50);

	mButton["EQUIP_1"].box = D2D_RectMake(CharaSlider.axisPos.x - 128, CharaSlider.axisPos.y - 128,
		CharaSlider.axisPos.x - 0, CharaSlider.axisPos.y + 0);

	mButton["EQUIP_2"].box = D2D_RectMake(CharaSlider.axisPos.x - 128, CharaSlider.axisPos.y - 128,
		CharaSlider.axisPos.x - 0, CharaSlider.axisPos.y + 0);

	mButton["EQUIP_3"].box = D2D_RectMake(CharaSlider.axisPos.x - 128, CharaSlider.axisPos.y - 128,
		CharaSlider.axisPos.x - 0, CharaSlider.axisPos.y + 0);


	state = ES_MAIN;
	EquipNum = -1;

	//	Loading List Setting
	LOADMANAGER->Add_LoadTray("NameLabel", "../../_Assets/Texture2D/NameLabel.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	//EQUIP->AddTray_EquipImage();										//	get List of Equip

	//	Loading Setting
	LOADMANAGER->setAutoInit(false);
	LOADMANAGER->setNextScene("EQUIP");
	LOADMANAGER->setLoadImageKey("LoadBk_InEquip");
	SCENE->Change_Scene("LOAD");
}

void EquipScene::release()
{
}

void EquipScene::update()
{
	worldColor.a = worldColor.a < 1.0f ? worldColor.a + DELTA : 1.0f;

	CAMERA->setCameraFix(true);
	CAMERA->CameraReset();

	switch (state)
	{
	case EquipScene::ES_MAIN:
		State_MainUpdate();
		break;

	case EquipScene::ES_CHARA:
		State_CharacterUpdate();
		break;

	case EquipScene::ES_EQUIP:
		State_EquipUpdate();
		break;
	}

	//KEYMANAGER->Init();

	PLAYER->update();
	if (CharaSlider.InfoDollID != -1)
	{
		auto& tacdoll = PLAYER->getPlayerTaticDoll().getAllTacDoll().at(CharaSlider.InfoDollID);
		if (tacdoll->isFocus())
		{
			tacdoll->revFocus();
			PLAYER->setCurFocusDoll(nullptr);
		}
	}
}

void EquipScene::render()
{
	switch (state)
	{
	case EquipScene::ES_MAIN:
		State_MainRender();
		break;

	case EquipScene::ES_CHARA:
		State_CharacterRender();
		break;

	case EquipScene::ES_EQUIP:
		State_EquipRender();
		break;
	}

	PLAYER->render();
}

void EquipScene::State_MainUpdate()
{
	for (auto& it : mButton)
	{
		if (ptInRect(it.second.box, g_ptMouse))
		{
			if (KEYMANAGER->isKeyDown(VK_LBUTTON))
			{
				it.second.ClickAction(this);

				if (CharaSlider.InfoDollID != -1)
				{
					if (it.first.find("EQUIP") != string::npos)
					{
						auto& tacDoll = PLAYER->getPlayerTaticDoll().getAllTacDoll().at(CharaSlider.InfoDollID);

						if (it.first.find("_1") != string::npos)
							SelectedEQ_Type = tacDoll->p_getEquip().begin()->first;

						else if (it.first.find("_2") != string::npos)
							SelectedEQ_Type = (++tacDoll->p_getEquip().begin())->first;

						else
							SelectedEQ_Type = (++(++tacDoll->p_getEquip().begin()))->first;

					}
				}
				break;
			}
		}
	}

	//else if (KEYMANAGER->isKeyStayDown(VK_UP))
	//	CharaSlider.Opacity += DELTA;

	//else if (KEYMANAGER->isKeyStayDown(VK_DOWN))
	//	CharaSlider.Opacity -= DELTA;

	if (CharaSlider.isMoving)
	{
		if (CharaSlider.isOpen)
		{
			if (CharaSlider.axisPos.x < CHARA_SLIDER_LIMIT_MAX_X)
				CharaSlider.axisPos.x += DELTA * CHARA_SLIDER_SPEED;
			else
			{
				CharaSlider.axisPos.x = CHARA_SLIDER_LIMIT_MAX_X;
				CharaSlider.isMoving = false;
			}
		}

		else
		{
			if (CharaSlider.axisPos.x > CHARA_SLIDER_LIMIT_MIN_X)
				CharaSlider.axisPos.x -= DELTA * CHARA_SLIDER_SPEED;
			else
			{
				CharaSlider.axisPos.x = CHARA_SLIDER_LIMIT_MIN_X;
				CharaSlider.isMoving = false;
			}
		}

		//	BoxCollider Refresh
		{
			CharaInfoBox = D2D_RectMake(CharaSlider.axisPos.x - 300, CharaSlider.axisPos.y - 180,
				CharaSlider.axisPos.x + 300, CharaSlider.axisPos.y + 180);

			mButton["SLIDER"].box.left = CharaSlider.axisPos.x + 300;
			mButton["SLIDER"].box.right = CharaSlider.axisPos.x + 320;

			mButton["CHARA"].box.left = CharaSlider.axisPos.x - 290;
			mButton["CHARA"].box.right = CharaSlider.axisPos.x - 140;

			mButton["EQUIP_1"].box.left = CharaSlider.axisPos.x - 130;
			mButton["EQUIP_1"].box.right = mButton["EQUIP_1"].box.left + 128;
			mButton["EQUIP_1"].box.top = CharaSlider.axisPos.y - 170;
			mButton["EQUIP_1"].box.bottom = mButton["EQUIP_1"].box.top + 128;

			mButton["EQUIP_2"].box.left = CharaSlider.axisPos.x + 12;
			mButton["EQUIP_2"].box.right = mButton["EQUIP_2"].box.left + 128;
			mButton["EQUIP_2"].box.top = CharaSlider.axisPos.y - 170;
			mButton["EQUIP_2"].box.bottom = mButton["EQUIP_2"].box.top + 128;

			mButton["EQUIP_3"].box.left = CharaSlider.axisPos.x + 152;
			mButton["EQUIP_3"].box.right = mButton["EQUIP_3"].box.left + 128;
			mButton["EQUIP_3"].box.top = CharaSlider.axisPos.y - 170;
			mButton["EQUIP_3"].box.bottom = mButton["EQUIP_3"].box.top + 128;

		}


	}

	if (CharaSlider.InfoDollID != -1)
	{
		PLAYER->getPlayerTaticDoll().getAllTacDoll().at(CharaSlider.InfoDollID)->update();
	}
}

void EquipScene::State_MainRender()
{
	if (CharaSlider.InfoDollID != -1)
	{
		PLAYER->getPlayerTaticDoll().getAllTacDoll().at(CharaSlider.InfoDollID)->render_Ellipse();
		PLAYER->getPlayerTaticDoll().getAllTacDoll().at(CharaSlider.InfoDollID)->render_Motion();
	}

	D2DX->renderRect(mButton["SLIDER"].box, ColorF(0, 0.15, 0.85), true);

	if (CharaSlider.isMoving)
		D2DX->renderRect(CharaInfoBox, ColorF(0, 0.15, 0.85f, CharaSlider.Opacity), true);
	else
	{
		if (CharaSlider.isOpen)
			D2DX->renderRect(CharaInfoBox, ColorF(0, 0.15, 0.85f, CharaSlider.Opacity), true);
	}

	D2DX->renderRect(mButton["CHARA"].box, ColorF(0, 0.8, 0.0), true);

	// if the Character was already Selected in Box
	if (CharaSlider.InfoDollID != -1)
	{
		auto& tacDoll = PLAYER->getPlayerTaticDoll().getAllTacDoll().at(CharaSlider.InfoDollID);

		FLOAT wid = mButton["CHARA"].box.right - mButton["CHARA"].box.left;
		FLOAT hei = mButton["CHARA"].box.bottom - mButton["CHARA"].box.top;

		DRAW->render(tacDoll->cardNormalKey, DV2(wid*0.5f, hei*0.5f), DV2(mButton["CHARA"].box.left + (wid * 0.5f), mButton["CHARA"].box.top + (hei*0.5f)));

		// Equipment Rendering
		{
			D2DX->renderRect(mButton["EQUIP_1"].box, ColorF(1, 0, 0), true);
			D2DX->renderRect(mButton["EQUIP_2"].box, ColorF(0, 1, 0), true);
			D2DX->renderRect(mButton["EQUIP_3"].box, ColorF(0, 0, 0.5), true);

			auto it = tacDoll->p_getEquip().begin();

			if (it != tacDoll->p_getEquip().end())
			{
				wid = mButton["EQUIP_1"].box.right - mButton["EQUIP_1"].box.left;
				hei = mButton["EQUIP_1"].box.bottom - mButton["EQUIP_1"].box.top;
				if (it->second != nullptr)
					DRAW->render(it->second->getKey(), DV2(wid*0.5f, hei*0.5f), 
						DV2(mButton["EQUIP_1"].box.left + (wid * 0.5f), mButton["EQUIP_1"].box.top + (hei*0.5f)));
				++it;

				wid = mButton["EQUIP_2"].box.right - mButton["EQUIP_2"].box.left;
				hei = mButton["EQUIP_2"].box.bottom - mButton["EQUIP_2"].box.top;
				if (it->second != nullptr)
					DRAW->render(it->second->getKey(), DV2(wid*0.5f, hei*0.5f), 
						DV2(mButton["EQUIP_2"].box.left + (wid * 0.5f), mButton["EQUIP_2"].box.top + (hei*0.5f)));
				++it;

				wid = mButton["EQUIP_3"].box.right - mButton["EQUIP_3"].box.left;
				hei = mButton["EQUIP_3"].box.bottom - mButton["EQUIP_3"].box.top;
				if (it->second != nullptr)
					DRAW->render(it->second->getKey(), DV2(wid*0.5f, hei*0.5f), 
						DV2(mButton["EQUIP_3"].box.left + (wid * 0.5f), mButton["EQUIP_3"].box.top + (hei*0.5f)));
			}
		}


		// Status Rendering
		{
			if (tacDoll != nullptr)
			{
				TEXT->Change_Text("STATUS_IDX", "°ø°Ý·Â");
				TEXT->TextRender("STATUS_IDX", 150, 150, ColorF(0, 0, 0));
				TEXT->Change_Text("STATUS_VAL", ChangeToLPC(tacDoll->getStatus().AttackPoint));
				TEXT->TextRender("STATUS_VAL", 250, 150, ColorF(1, 1, 0));


				TEXT->Change_Text("STATUS_IDX", "Ä¡¸íÅ¸ È®·ü");
				TEXT->TextRender("STATUS_IDX", 150, 170, ColorF(0, 0, 0));
				TEXT->Change_Text("STATUS_VAL", ChangeToLPC(tacDoll->getStatus().CriticPoint));
				TEXT->TextRender("STATUS_VAL", 250, 170, ColorF(1, 1, 0));

				TEXT->Change_Text("STATUS_IDX", "Ä¡¸íÅ¸¹èÀ²");
				TEXT->TextRender("STATUS_IDX", 150, 190, ColorF(0, 0, 0));
				TEXT->Change_Text("STATUS_VAL", ChangeToLPC(tacDoll->getStatus().CriticAcl));
				TEXT->TextRender("STATUS_VAL", 250, 190, ColorF(1, 1, 0));

				TEXT->Change_Text("STATUS_IDX", "¸íÁß·ü");
				TEXT->TextRender("STATUS_IDX", 150, 210, ColorF(0, 0, 0));
				TEXT->Change_Text("STATUS_VAL", ChangeToLPC(tacDoll->getStatus().HitPoint));
				TEXT->TextRender("STATUS_VAL", 250, 210, ColorF(1, 1, 0));
			}
		}

	}

	D2DX->renderRect(mButton["CANCLE"].box, ColorF(0, 0.8, 0.0), true);
}

void EquipScene::State_EquipUpdate()
{
	auto& tacDoll = PLAYER->getPlayerTaticDoll().getAllTacDoll();

	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		int i = 0;
		for (auto& it : PLAYER->getPlayerEquip())
		{
			if (it.second.equipType == SelectedEQ_Type)
			{
				FLOAT x = 150 + ((i % 4) * 265);
				FLOAT y = 150 + ((i / 4) * 265);

				if (ptInRect(D2D_RectMakeCenter(x, y, 128, 128), g_ptMouse))
				{
					if (it.second.num > 0)
					{
						tacDoll.at(CharaSlider.InfoDollID)->p_getEquip().at(SelectedEQ_Type) = it.second.equip;
						tacDoll.at(CharaSlider.InfoDollID)->AttachEquipment();
						state = ES_MAIN;
					}

					break;
				}
				else
					++i;
			}
		}

		//¹öÆ°Ã¼Å©
		if (state == ES_EQUIP)
		{
			for (auto& btn : mButton)
			{
				if (ptInRect(btn.second.box, g_ptMouse))
				{
					btn.second.ClickAction(this);
					break;
				}
			}
		}

	}
}

void EquipScene::State_EquipRender()
{
	State_MainRender();
	D2DX->renderRect(D2D_RectMake(0, 0, WINSIZEX, WINSIZEY), ColorF(0, 0, 0, 0.85), true);

	int i = 0;
	for (auto& it : PLAYER->getPlayerEquip())
	{
		if (it.second.equipType == SelectedEQ_Type)
		{
			FLOAT x = 150 + ((i % 4) * 265);
			FLOAT y = 150 + ((i / 4) * 265);

			D2DX->renderRect(D2D_RectMakeCenter(x, y, 128, 128), ColorF(1, 0, 0));

			it.second.equip->render(x, y - 50, 0.5f);

			if (it.second.num < 1)
				D2DX->renderRect(D2D_RectMakeCenter(x, y, 64, 30), ColorF(1, 0, 0));
			
			DRAW->render("NameLabel", DV2(128, 60), DV2(x, y + 67), DCR(1, 1, 1, 1), DV3(0, 0, 180.0f));

			TEXT->Change_Text("CHARA_NAME", it.second.equip->getString().spec);
			TEXT->TextRender("CHARA_NAME", x - 123, y + 25, 245, 40, ColorF(1, 1, 0), DWRITE_TEXT_ALIGNMENT_CENTER);

			TEXT->Change_Text("EQUIP_EXP", it.second.equip->getString().explain);
			TEXT->TextRender("EQUIP_EXP", x - 122, y + 62, 245, 100, ColorF(1, 1, 1), DWRITE_TEXT_ALIGNMENT_CENTER);

			++i;
		}
	}

	D2DX->renderRect(mButton["CANCLE"].box, ColorF(0, 0.8, 0.0), true);
}

void EquipScene::State_CharacterUpdate()
{
	auto& it = PLAYER->getPlayerTaticDoll().getAllTacDoll();

	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < it.size(); ++i)
		{
			FLOAT x = 80 + ((i % 10) * 140);
			FLOAT y = 150 + ((int)(i * 0.1f) * 270.0f);

			if (ptInRect(D2D_RectMake(x - 60, y - 125, x + 60, y + 125), g_ptMouse))
			{
				if (CharaSlider.InfoDollID != -1)
					PLAYER->deleteTacDolToSquad(CharaSlider.InfoDollID, 1);

				CharaSlider.InfoDollID = i;
				PLAYER->insertTacDolToSquad(CharaSlider.InfoDollID, 1);

				it.at(i)->init();
				state = ES_MAIN;
				i = it.size();
				break;
			}

		}

		if (state == ES_CHARA)
			for (auto& btn : mButton)
			{
				if (ptInRect(btn.second.box, g_ptMouse))
				{
					btn.second.ClickAction(this);
					break;
				}
			}
	}
}

void EquipScene::State_CharacterRender()
{
	State_MainRender();
	D2DX->renderRect(D2D_RectMake(0, 0, WINSIZEX, WINSIZEY), ColorF(0, 0, 0, 0.85), true);

	auto& it = PLAYER->getPlayerTaticDoll().getAllTacDoll();

	for (int i = 0; i < it.size(); ++i)
	{
		auto& tacDoll = it.at(i);

		FLOAT x = 80 + ((i % 10) * 140);
		FLOAT y = 150 + ((int)(i * 0.1f) * 270.0f);

		DRAW->render(tacDoll->cardNormalKey, DV2(60, 125), DV2(x, y));
		DRAW->render("NameLabel", DV2(60, 30), DV2(x, y + 155.5));

		TEXT->Change_Text("CHARA_NAME", tacDoll->getName());
		TEXT->TextRender("CHARA_NAME", x - 54.0f, y + 125.0f, 110, 40, ColorF(1, 1, 0), DWRITE_TEXT_ALIGNMENT_CENTER);

		D2DX->renderRect(x - 60, y - 125, 120, 250, ColorF(1, 0, 0, 0.5));
	}

	D2DX->renderRect(mButton["CANCLE"].box, ColorF(0, 0.8, 0.0), true);
}

void EquipScene::SlideOpener(void* obj)
{
	EquipScene* object = (EquipScene*)obj;

	if (object->state == ES_MAIN)
	{
		//ÀÌ¹Ì ¿­·ÁÀÖ´Â°Å³ª ´ÝÇôÀÖ´Â »óÅÂ¶ó¸é
		if (!object->CharaSlider.isMoving)
		{
			object->CharaSlider.isOpen = !object->CharaSlider.isOpen;
			object->CharaSlider.isMoving = true;
		}
	}
}

void EquipScene::CharacterSelect(void * obj)
{
	EquipScene* object = (EquipScene*)obj;

	if (object->state == ES_MAIN)
	{
		if (!object->CharaSlider.isMoving)
		{
			object->state = ES_CHARA;

			auto& pTacDoll = PLAYER->getPlayerTaticDoll().getAllTacDoll();

			for (auto& it : pTacDoll)
				it.second->init();

			TEXT->Change_TextSize("CHARA_NAME", 28);

		}
	}
}

void EquipScene::EquipmentSelect(void * obj)
{
	EquipScene* object = (EquipScene*)obj;

	if (object->state == ES_MAIN && object->CharaSlider.InfoDollID != -1)
	{
		object->EquipNum = 0;

		object->state = ES_EQUIP;

		TEXT->Change_TextSize("CHARA_NAME", 12);

		for (auto& it : PLAYER->getPlayerEquip())
			if (it.second.equipType == object->SelectedEQ_Type)
				++object->EquipNum;
	}
}

void EquipScene::ReturnSelect(void * obj)
{
	EquipScene* object = (EquipScene*)obj;

	if (object->state == ES_MAIN)
	{
		//EQUIP->ExitTray_EquipImage();

		//for (auto& it : BDATA->getSquadSNV()->Call_Squad(1)->mSquad)
		//	it.second->LoadTray_List();

		//for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())		//	get List of Player's TacticalDoll
		//	it.second->LoadTray_List();										//	Add All Player's LoadTrayList

		if (object->CharaSlider.InfoDollID != -1)
			PLAYER->deleteTacDolToSquad(object->CharaSlider.InfoDollID, 1);

		LOADMANAGER->setAutoInit(true);
		LOADMANAGER->setNextScene("LOBBY");
		LOADMANAGER->setLoadImageKey("LoadBK_Test");
		SCENE->Change_Scene("LOAD");
		SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);
		worldColor.a = 0.0f;
	}

	else
		object->state = ES_MAIN;

}
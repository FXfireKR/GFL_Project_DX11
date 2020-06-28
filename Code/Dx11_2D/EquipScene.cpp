#include "stdafx.h"
#include "EquipScene.h"

EquipScene* EquipScene::objectPtr = nullptr;

/*
	static float debx = 800.0f;
	static float deby = 50.0f;
	static float debs = 28.0f

	ImGui::DragFloat("debx", &debx, 0.1f);
	ImGui::DragFloat("deby", &deby, 0.1f);
	ImGui::DragFloat("debs", &debs, 1.0f);
*/

EquipScene::EquipScene()
{
}

EquipScene::~EquipScene()
{
}

void EquipScene::init()
{
	worldColor.a = 0.0f;
	whlCount = 0;
	virtualHeight = 0.0f;
	virtualLimit = 0.0f;
	asixVirtual = 0;

	sceneChange = false;
	mouseDrag = false;

	selectedDoll = nullptr;
	selectedDollID = 0;
	objectPtr = nullptr;

	state = ES_MAIN;
	EquipNum = -1;

	for (auto& it : PLAYER->getPlayerTaticDoll().getAllDolls())		//	get List of Player's TacticalDoll
		it.second->LoadTray_ImageList();

	mButton.insert(make_pair(SBUTTONS::HOME_BACK, Button(10, 10, 100, 85, ReturnHome)));
	mButton.insert(make_pair(SBUTTONS::TURN_BACK, Button(10, 10, 150, 60, ReturnSelect)));
	mButton.insert(make_pair(SBUTTONS::SELECT_CHARA, Button(20, 160, SQUAD_BOX_WIDTH, SQUAD_BOX_HEIGHT, CharacterSelect)));
	mButton.insert(make_pair(SBUTTONS::SELECT_EQUIP_1, Button(0, 200, 150, 100, EquipmentSelect)));
	mButton.insert(make_pair(SBUTTONS::SELECT_EQUIP_2, Button(0, 200, 150, 100, EquipmentSelect)));
	mButton.insert(make_pair(SBUTTONS::SELECT_EQUIP_3, Button(0, 200, 150, 100, EquipmentSelect)));

	DWRITE->Create_TextField("CHARA_NAME", L"¸¼Àº°íµñ", "NULL", 28, DWRITE_FONT_WEIGHT_MEDIUM);
	DWRITE->Create_TextField("TITLE_NAME", L"¸¼Àº°íµñ", "NULL", 65, DWRITE_FONT_WEIGHT_BOLD);

	DWRITE->Create_TextField("EQUIP_NAME", L"¸¼Àº°íµñ", "NULL", 26, DWRITE_FONT_WEIGHT_BOLD);
	DWRITE->Create_TextField("EQUIP_EXP", L"¸¼Àº°íµñ", "NULL", 15, DWRITE_FONT_WEIGHT_BOLD);

	DWRITE->Create_TextField("STATUS_IDX", L"¸¼Àº°íµñ", "NULL", 14, DWRITE_FONT_WEIGHT_BOLD);
	DWRITE->Create_TextField("STATUS_VAL", L"¸¼Àº°íµñ", "NULL", 10, DWRITE_FONT_WEIGHT_BOLD);

	//	Loading List Setting
	LOAD->Add_LoadTray("NameLabel", "Texture2D/NameLabel.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("EquipBarBack", "Texture2D/EquipBarBack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("editSceneBk", "Texture2D/editSceneBk.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("gradiantBlack", "Texture2D/gradiantBlack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("HomeButton", "Texture2D/HomeButton.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("TurnBack", "Texture2D/TurnBack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("DollEmpty", "Texture2D/dollEmpty.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("AllCard", "Texture2D/AllCard.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOAD->Add_LoadTray("EquipCard", "Texture2D/EquipCard.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("EquipOut", "Texture2D/EquipOut.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("EquipCardBk", "Texture2D/EquipCardBk.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("SquadEmit", "Texture2D/SquadEmit.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	//EQUIP->AddTray_EquipImage();										//	get List of Equip

	//	Loading Setting
	LOAD->setAutoInit(false);
	LOAD->setNextScene("EQUIP");
	LOAD->setLoadImageKey("ShootRange");

	LOAD->mallocThread();
	SCENE->changeScene("LOAD");

	CAMERA->CameraReset();
}

void EquipScene::release()
{
}

void EquipScene::update()
{
	CAMERA->setCameraFix(true);

	if (sceneChange)
	{
		if (worldColor.a > 0.0f)
		{
			worldColor.a -= DELTA();
			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, worldColor.a > 0.15f ? 0.15f : worldColor.a);
		}

		else
		{
			SOUND->Stop_Sound(SOUND_CHANNEL::CH_SOUND1, "FormationLoop");
			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);

			LOAD->setAutoInit(true);
			LOAD->setNextScene("LOBBY");
			LOAD->setLoadImageKey("LoadBK_Test");
			SCENE->changeScene("LOAD");
			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);

			CAMERA->CameraReset();

			worldColor.a = 0.0f;
		}
	}

	else
	{
		if (!SOUND->isValidKey("FormationLoop"))
		{
			if (!SOUND->isPlay(SOUND_CHANNEL::CH_SOUND1, "FormationLoop"))
			{
				SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);
				SOUND->Play_Sound(SOUND_CHANNEL::CH_SOUND1, "FormationLoop", 0.0f);
				SOUND->setVolume("FormationLoop", SOUND_CHANNEL::CH_SOUND1, 0.0f);
			}
		}

		if (worldColor.a < 1.0f)
		{
			worldColor.a += DELTA();

			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, worldColor.a < 0.25f ? worldColor.a : 0.25f);
			SOUND->setVolume("FormationLoop", SOUND_CHANNEL::CH_SOUND1, worldColor.a < 0.25f ? worldColor.a : 0.25f);
		}

		else
		{
			worldColor.a = 1.0f;

			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.25f);
			SOUND->setVolume("FormationLoop", SOUND_CHANNEL::CH_SOUND1, 0.25f);
		}

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
}

void EquipScene::State_MainUpdate()
{
	for (auto& it : mButton) {

		if (ptInRect(it.second.box, g_ptMouse)) {

			if (KEYMANAGER->isKeyDown(VK_LBUTTON)) {

				if (selectedDoll != nullptr) {

					if ((int)it.first > 1) {
						switch (it.first)
						{
						case SBUTTONS::SELECT_EQUIP_1:
							SelectedEQ_Type = selectedDoll->p_getEquip().begin()->first;
							break;

						case SBUTTONS::SELECT_EQUIP_2:
							SelectedEQ_Type = (++selectedDoll->p_getEquip().begin())->first;
							break;

						case SBUTTONS::SELECT_EQUIP_3:
							SelectedEQ_Type = (++(++selectedDoll->p_getEquip().begin()))->first;
							break;

						default : break;
						}
					}
				}

				it.second.ClickAction(this);
				break;
			}
		}
	}
}

void EquipScene::State_MainRender()
{
	DRAW->render("editSceneBk", Vector2(WINSIZEX, WINSIZEY), Vector2(WINSIZEX*0.5f, WINSIZEY*0.5f));
	DRAW->render("gradiantBlack", Vector2(WINSIZEX, 100), Vector2(WINSIZEX*0.5f, 50));

	DWRITE->ChangeText("TITLE_NAME", "FACTORY");
	DWRITE->TextRender("TITLE_NAME", 995.0f, 0.0f, ColorF(0.8, 0.8, 0.8));

	D2D->renderRect(mButton[SBUTTONS::HOME_BACK].box, ColorF(1, 0, 0));
	DRAW->render("HomeButton", Vector2(100, 85), Vector2(mButton[SBUTTONS::HOME_BACK].box.left + 50,
		mButton[SBUTTONS::HOME_BACK].box.top + 40));

	D2D->renderRect(mButton[SBUTTONS::SELECT_CHARA].box, ColorF(0, 0, 0), true);

	// if the Character was already Selected in Box
	if (selectedDoll != nullptr)
	{
		FLOAT wid = mButton[SBUTTONS::SELECT_CHARA].box.right - mButton[SBUTTONS::SELECT_CHARA].box.left - 4;
		FLOAT hei = mButton[SBUTTONS::SELECT_CHARA].box.bottom - mButton[SBUTTONS::SELECT_CHARA].box.top - 4;

		DRAW->render(selectedDoll->keys.cardNormalKey, Vector2(wid, hei),
			Vector2(mButton[SBUTTONS::SELECT_CHARA].box.left + (wid * 0.5f) + 2,
				mButton[SBUTTONS::SELECT_CHARA].box.top + (hei*0.5f) + 2));

		//DWRITE->TextRender();
	}

	const uiAtlas* atlas = selectedDoll != nullptr ? IMAGEMAP->getUiAtlas("InstOvSlot") : IMAGEMAP->getUiAtlas("InstSlot");
	DRAW->render(atlas->textureKey, atlas->alphaTexKey, Vector2(SQUAD_BOX_H_WIDTH, SQUAD_BOX_H_HEIGHT),
		Vector2(mButton[SBUTTONS::SELECT_CHARA].box.left + SQUAD_BOX_H_WIDTH,
			mButton[SBUTTONS::SELECT_CHARA].box.top + SQUAD_BOX_H_HEIGHT), atlas->mixTexCoord, atlas->maxTexCoord);

	
		//// Equipment Rendering
		//{
		//	D2D->renderRect(mButton["EQUIP_1"].box, ColorF(1, 0, 0), true);
		//	D2D->renderRect(mButton["EQUIP_2"].box, ColorF(0, 1, 0), true);
		//	D2D->renderRect(mButton["EQUIP_3"].box, ColorF(0, 0, 0.5), true);
		//	
		//	auto iter = selectedDoll->p_getEquip().begin();
		//	for (size_t i = 0; i < selectedDoll->p_getEquip().size(); ++i)
		//	{
		//		string equipKey = ConvertFormat("EQUIP_%d", i + 1);

		//		wid = mButton[equipKey].box.right - mButton[equipKey].box.left;
		//		hei = mButton[equipKey].box.bottom - mButton[equipKey].box.top;
		//		if (iter->second != nullptr)
		//			DRAW->render(iter->second->getKey(), VEC2(wid, hei),
		//				VEC2(mButton[equipKey].box.left + (wid * 0.5f), mButton[equipKey].box.top + (hei*0.5f)));
		//		++iter;
		//	}
		//}
	


		// Status Rendering
		{
			//if (selectedDoll != nullptr)
			//{
			//	DWRITE->ChangeText("STATUS_IDX", "°ø°Ý·Â : %d", selectedDoll->getStatus().AttackPoint);
			//	DWRITE->TextRender("STATUS_IDX", 150, 150, ColorF(0, 0, 0));
			//
			//	DWRITE->ChangeText("STATUS_IDX", "Ä¡¸íÅ¸ È®·ü : %.1f %", selectedDoll->getStatus().CriticPoint);
			//	DWRITE->TextRender("STATUS_IDX", 150, 170, ColorF(0, 0, 0));
			//
			//	DWRITE->ChangeText("STATUS_IDX", "Ä¡¸íÅ¸¹èÀ² : %.1f %", selectedDoll->getStatus().CriticAcl * 100.0);
			//	DWRITE->TextRender("STATUS_IDX", 150, 190, ColorF(0, 0, 0));
			//
			//	DWRITE->ChangeText("STATUS_IDX", "¸íÁß·ü : %.2f", selectedDoll->getStatus().Accuracy * 100.0);
			//	DWRITE->TextRender("STATUS_IDX", 150, 210, ColorF(0, 0, 0));
			//}
		}

}

void EquipScene::State_EquipUpdate()
{
	if (whlCount < 0)
		whlCount = 0;

	else if (whlCount > ((EquipNum / 4) * 460))
		whlCount = ((EquipNum / 4) * 460);


	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		int i = 0;
		for (auto& it : PLAYER->getPlayerEquip())
		{
			//	Equip TypeÀÌ °°´Ù¸é
			if (it.second.equipType == SelectedEQ_Type)
			{
				if (!it.second.equip->isAttachAble(selectedDoll->getWeaponType()))
				{
					//++i;
					continue;
				}
				else
				{
					FLOAT x = 150 + ((i % 4) * 265);
					FLOAT y = 450 + ((i / 4) * 450);
					POINT vptMouse = g_ptMouse;
					vptMouse.y += whlCount;

					if (ptInRect(D2D_RectMakeCenter(x, y, 120, 211), vptMouse))
					{
						if (it.second.num > 0)
						{
							if (it.second.equip->isAttachAble(selectedDoll->getWeaponType()))
							{
								selectedDoll->p_getEquip().at(SelectedEQ_Type) = it.second.equip;
								selectedDoll->AttachEquipment();
								state = ES_MAIN;
							}
						}

						break;
					}
					else
						++i;
				}
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
	D2D->renderRect(D2D_RectMake(0, 0, WINSIZEX, WINSIZEY), ColorF(0, 0, 0, 0.85), true);

	int i = 0;
	for (auto& it : PLAYER->getPlayerEquip())
	{
		if (it.second.equipType == SelectedEQ_Type)
		{
			if (!it.second.equip->isAttachAble(selectedDoll->getWeaponType()))
				continue;

			else
			{
				FLOAT x = 150 + ((i % 4) * 265);
				FLOAT y = 450 + ((i / 4) * 450);

				D2D->renderRect(D2D_RectMakeCenter(x, y - whlCount, 120, 211), ColorF(1, 0, 0));

				if (it.second.num < 1)
					D2D->renderRect(D2D_RectMakeCenter(x, y - whlCount, 64, 30), ColorF(1, 0, 0));

				DRAW->render("EquipCardBk", Vector2(232, 180), Vector2(x, y - 81.5f - whlCount), COLR(1, 1, 1, 1));
				DRAW->render("EquipCard", Vector2(240, 422), Vector2(x, y - whlCount), COLR(1, 1, 1, 1));

				it.second.equip->render(x, y - 81.5f - whlCount, 0.65f);

				DWRITE->Change_Text("EQUIP_NAME", it.second.equip->getString().name);
				DWRITE->TextRender("EQUIP_NAME", x - 123, y + 35 - whlCount, 240, 40, ColorF(1, 1, 0), DWRITE_TEXT_ALIGNMENT_CENTER);

				DWRITE->Change_Text("EQUIP_EXP", it.second.equip->getString().native);
				DWRITE->TextRender("EQUIP_EXP", x - 122, y + 145 - whlCount, 240, 100, ColorF(1, 1, 1), DWRITE_TEXT_ALIGNMENT_CENTER);

				DWRITE->Change_Text("EQUIP_EXP", it.second.equip->getString().spec);
				DWRITE->TextRender("EQUIP_EXP", x - 122, y + 108 - whlCount, 240, 100, ColorF(1, 1, 1), DWRITE_TEXT_ALIGNMENT_CENTER);

				++i;
			}
		}
	}

	if (i == 0)
	{
		DWRITE->Change_Text("EQUIP_NAME", "Âø¿ë°¡´ÉÇÑ Àåºñ°¡ ¾ø½À´Ï´Ù");
		DWRITE->TextRender("EQUIP_NAME", 0, WINSIZEY * 0.5f, WINSIZEX, 50, ColorF(1, 1, 0), DWRITE_TEXT_ALIGNMENT_CENTER);
	}

	//D2D->renderRect(mButton["CANCLE"].box, ColorF(0, 0.8, 0.0), true);
}

void EquipScene::State_CharacterUpdate()
{
	//	Wheel Use Code
	if (whlCount > 0) {
		virtualHeight -= DELTA() * 1000.0f;
		whlCount = 0;
	}
	else if (whlCount < 0) {
		virtualHeight += DELTA() * 1000.0f;
		whlCount = 0;
	}

	//	Moude Use Code
	if (g_ptMouse.x > 1150) {
		if (KEYMANAGER->isKeyDown(VK_LBUTTON)) {
			asixVirtual = g_ptMouse.y;
			mouseDrag = true;
		}

		if (KEYMANAGER->isKeyUp(VK_LBUTTON)) 
			mouseDrag = false;

		if (mouseDrag) {
			if (asixVirtual > g_ptMouse.y)
				virtualHeight -= DELTA() * 700.0f;

			else if (asixVirtual < g_ptMouse.y)
				virtualHeight += DELTA() * 700.0f;

			asixVirtual = g_ptMouse.y;
		}
	}

	if (virtualHeight > 0)
		virtualHeight = 0.0f;
	else if (virtualHeight < virtualLimit) {
		virtualHeight = virtualLimit;
	}

	for (size_t i = 0; i < selBox.size(); ++i) {
		selBox[i].box = D2DRectMake(selBox[i].pos.x, selBox[i].pos.y + virtualHeight,
			CHARACTER_BOX_WID, CHARACTER_BOX_HEI);
	}

	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		for (size_t i = 0; i < selBox.size(); ++i) {

			if (ptInRect(selBox[i].box, g_ptMouse)) {

				selectedDoll = selBox[i].adress != nullptr ? (BaseTaticDoll*)selBox[i].adress : nullptr;
				selectedDollID = i;

				state = ES_MAIN;
				mouseDrag = false;
				break;
			}
		}

		if (state == ES_CHARA) {
			if (ptInRect(mButton[SBUTTONS::TURN_BACK].box, g_ptMouse))
				mButton[SBUTTONS::TURN_BACK].ClickAction(this);
		}
	}
}

void EquipScene::State_CharacterRender()
{
	DRAW->render("editSceneBk", Vector2(WINSIZEX, WINSIZEY), Vector2(WINSIZEX*0.5f, WINSIZEY*0.5f));

	for (size_t i = 0; i < selBox.size(); ++i) {
		FLOAT wid = selBox[i].box.right - selBox[i].box.left;
		FLOAT hei = selBox[i].box.bottom - selBox[i].box.top;
		FLOAT halfWid = wid * 0.5f;
		FLOAT halfHei = hei * 0.5f;
		Vector2 rendPos;

		if (selBox[i].adress != nullptr) {
			BaseTaticDoll* focusedTdoll = ((BaseTaticDoll*)selBox[i].adress);
			
			rendPos = Vector2(selBox[i].pos.x + halfWid, selBox[i].pos.y + halfHei + virtualHeight);

			D2D->renderRect(selBox[i].box, ColorF(1, 0, 0), true);
			DRAW->render(focusedTdoll->keys.cardNormalKey, Vector2(wid, hei), rendPos);

			DRAW->render("AllCard", Vector2(wid, hei), rendPos);

			DWRITE->ChangeText("CHARA_NAME", focusedTdoll->keys.name);
			DWRITE->TextRender("CHARA_NAME", rendPos.x - halfWid, rendPos.y + halfHei - 60, wid, 40, ColorF(1, 1, 1),
				DWRITE_TEXT_ALIGNMENT_CENTER);
		}
		else {
			D2D->renderRect(selBox[i].box, ColorF(1, 0, 0), true);
			DRAW->render("SquadEmit", Vector2(wid, hei),
				Vector2(selBox[i].pos.x + halfWid, selBox[i].pos.y + halfHei + virtualHeight));
		}
	}

	DRAW->render("gradiantBlack", Vector2(WINSIZEX, 100), Vector2(WINSIZEX*0.5f, 50));

	D2D->renderRect(mButton[SBUTTONS::TURN_BACK].box, ColorF(1, 0, 0));
	DRAW->render("TurnBack", Vector2(150, 75), Vector2(mButton[SBUTTONS::TURN_BACK].box.left + 75,
		mButton[SBUTTONS::TURN_BACK].box.top + 35));

	DWRITE->ChangeText("TITLE_NAME", "T-DOLL");
	DWRITE->TextRender("TITLE_NAME", 1045.0f, 0.0f, ColorF(0.8, 0.8, 0.8));
}

void EquipScene::CharacterSelect(void * obj)
{
	objectPtr = (EquipScene*)obj;

	if (objectPtr->state == ES_MAIN)
	{
		objectPtr->state = ES_CHARA;

		auto& pTacDoll = PLAYER->getPlayerTaticDoll().getAllDolls();
		size_t counter = 0;
		objectPtr->selBox.clear();
		objectPtr->selBox.reserve(pTacDoll.size() + 1);

		//	Insert Null Character For Out of Character
		selectBox _new;
		_new.pos = Vector2(20 + (counter % objectPtr->WIDTH_COUNT) * objectPtr->CHARACTER_BLANK_WID,
			120 + (counter / objectPtr->WIDTH_COUNT) * objectPtr->CHARACTER_BLANK_HEI);
		_new.box = D2DRectMake(_new.pos.x, _new.pos.y,
			objectPtr->CHARACTER_BOX_WID, objectPtr->CHARACTER_BOX_HEI);
		_new.adress = nullptr;
		objectPtr->selBox.push_back(_new);
		++counter;

		for (auto& iter : pTacDoll) {
			selectBox _new;

			_new.pos = Vector2(20 + (counter % objectPtr->WIDTH_COUNT) * objectPtr->CHARACTER_BLANK_WID,
				120 + (counter / objectPtr->WIDTH_COUNT) * objectPtr->CHARACTER_BLANK_HEI);

			_new.box = D2DRectMake(_new.pos.x, _new.pos.y, 
				objectPtr->CHARACTER_BOX_WID, objectPtr->CHARACTER_BOX_HEI);

			_new.adress = iter.second;

			objectPtr->selBox.push_back(_new);

			++counter;
		}

		objectPtr->virtualLimit =
			(float)(((int)objectPtr->selBox.size()) / objectPtr->WIDTH_COUNT) * objectPtr->CHARACTER_BLANK_HEI;
		objectPtr->virtualLimit *= -1.0f;

		DWRITE->Change_TextSize("CHARA_NAME", 28);
	}
}

void EquipScene::EquipmentSelect(void * obj)
{
	EquipScene* object = (EquipScene*)obj;

	if (object->state == ES_MAIN && object->selectedDoll != nullptr)
	{
		object->EquipNum = 0;

		whlCount = 0;

		object->state = ES_EQUIP;

		for (auto& it : PLAYER->getPlayerEquip())
		{
			if (it.second.equipType == object->SelectedEQ_Type)
			{
				if (it.second.equip->isAttachAble(object->selectedDoll->getWeaponType()))
					++object->EquipNum;
			}
		}
	}
}

void EquipScene::ReturnSelect(void * obj)
{
	objectPtr = (EquipScene*)obj;
	objectPtr->state = ES_MAIN;
}

void EquipScene::ReturnHome(void * obj)
{
	objectPtr = (EquipScene*)obj;

	if (objectPtr->state == ES_MAIN)
	{
		//EQUIP->ExitTray_EquipImage();

		//for (auto& it : BDATA->getSquadSNV()->Call_Squad(1)->mSquad)
		//	it.second->LoadTray_List();

		//for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())		//	get List of Player's TacticalDoll
		//	it.second->LoadTray_List();										//	Add All Player's LoadTrayList

		objectPtr->sceneChange = true;
	}
}
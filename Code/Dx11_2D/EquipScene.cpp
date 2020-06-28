#include "stdafx.h"
#include "EquipScene.h"
#include "EquipBase.h"

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
	mButton.insert(make_pair(SBUTTONS::TURN_BACK, Button(10, 0, 150, 90, ReturnSelect)));
	mButton.insert(make_pair(SBUTTONS::SELECT_CHARA, Button(20, 160, SQUAD_BOX_WIDTH, SQUAD_BOX_HEIGHT, CharacterSelect)));
	mButton.insert(make_pair(SBUTTONS::SELECT_EQUIP_1, Button(250, 213, CHARACTER_BOX_WID, CHARACTER_BOX_HEI, EquipmentSelect)));
	mButton.insert(make_pair(SBUTTONS::SELECT_EQUIP_2, Button(450, 213, CHARACTER_BOX_WID, CHARACTER_BOX_HEI, EquipmentSelect)));
	mButton.insert(make_pair(SBUTTONS::SELECT_EQUIP_3, Button(650, 213, CHARACTER_BOX_WID, CHARACTER_BOX_HEI, EquipmentSelect)));

	DWRITE->Create_TextField("CHARA_NAME", L"¸¼Àº°íµñ", "NULL", 28, DWRITE_FONT_WEIGHT_MEDIUM);
	DWRITE->Create_TextField("TITLE_NAME", L"¸¼Àº°íµñ", "NULL", 65, DWRITE_FONT_WEIGHT_BOLD);
	DWRITE->Create_TextField("NOTIFICATION", L"¸¼Àº°íµñ", "NULL", 80, DWRITE_FONT_WEIGHT_BOLD);

	DWRITE->Create_TextField("EQUIP_NAME", L"¸¼Àº°íµñ", "NULL", 20, DWRITE_FONT_WEIGHT_MEDIUM);
	DWRITE->Create_TextField("EQUIP_EXP", L"¸¼Àº°íµñ", "NULL", 12, DWRITE_FONT_WEIGHT_MEDIUM);

	DWRITE->Create_TextField("STATUS_IDX", L"¸¼Àº°íµñ", "NULL", 20, DWRITE_FONT_WEIGHT_MEDIUM);
	DWRITE->Create_TextField("STATUS_VAL", L"¸¼Àº°íµñ", "NULL", 20, DWRITE_FONT_WEIGHT_MEDIUM);

	//	Loading List Setting
	LOAD->Add_LoadTray("NameLabel", "Texture2D/NameLabel.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("EquipBarBack", "Texture2D/EquipBarBack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("editSceneBk", "Texture2D/editSceneBk.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("gradiantBlack", "Texture2D/gradiantBlack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("HomeButton", "Texture2D/HomeButton.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("TurnBack", "Texture2D/TurnBack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("DollEmpty", "Texture2D/dollEmpty.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("AllCard", "Texture2D/AllCard.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOAD->Add_LoadTray("EquipEmpty", "Texture2D/EquipEmpty.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
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

	//D2D->renderRect(mButton[SBUTTONS::HOME_BACK].box, ColorF(1, 0, 0));
	DRAW->render("HomeButton", Vector2(100, 85), Vector2(mButton[SBUTTONS::HOME_BACK].box.left + 50,
		mButton[SBUTTONS::HOME_BACK].box.top + 40));

	D2D->renderRect(mButton[SBUTTONS::SELECT_CHARA].box, ColorF(0, 0, 0), false);

	// if the Character was already Selected in Box
	if (selectedDoll != nullptr)
	{
		FLOAT wid = mButton[SBUTTONS::SELECT_CHARA].box.right - mButton[SBUTTONS::SELECT_CHARA].box.left - 4;
		FLOAT hei = mButton[SBUTTONS::SELECT_CHARA].box.bottom - mButton[SBUTTONS::SELECT_CHARA].box.top - 4;

		DRAW->render(selectedDoll->keys.cardNormalKey, Vector2(wid, hei),
			Vector2(mButton[SBUTTONS::SELECT_CHARA].box.left + (wid * 0.5f) + 2,
				mButton[SBUTTONS::SELECT_CHARA].box.top + (hei*0.5f) + 2));
	}

	const uiAtlas* atlas = selectedDoll != nullptr ? IMAGEMAP->getUiAtlas("InstOvSlot") : IMAGEMAP->getUiAtlas("InstSlot");
	DRAW->render(atlas->textureKey, atlas->alphaTexKey, Vector2(SQUAD_BOX_H_WIDTH, SQUAD_BOX_H_HEIGHT),
		Vector2(mButton[SBUTTONS::SELECT_CHARA].box.left + SQUAD_BOX_H_WIDTH,
			mButton[SBUTTONS::SELECT_CHARA].box.top + SQUAD_BOX_H_HEIGHT), atlas->mixTexCoord, atlas->maxTexCoord);

	if (selectedDoll != nullptr) {
		FLOAT wid = mButton[SBUTTONS::SELECT_CHARA].box.right - mButton[SBUTTONS::SELECT_CHARA].box.left - 4;
		FLOAT hei = mButton[SBUTTONS::SELECT_CHARA].box.bottom - mButton[SBUTTONS::SELECT_CHARA].box.top - 4;

		DWRITE->ChangeText("CHARA_NAME", selectedDoll->keys.name);
		DWRITE->TextRender("CHARA_NAME", mButton[SBUTTONS::SELECT_CHARA].box.left + 3,
			mButton[SBUTTONS::SELECT_CHARA].box.bottom - 165.0f, wid - 6, 40, ColorF(1, 1, 1), DWRITE_TEXT_ALIGNMENT_CENTER);
	}

	
	// Equipment Rendering
	if (selectedDoll != nullptr)
	{	
		auto iter = selectedDoll->p_getEquip().begin();
		for (int i = (int)SBUTTONS::SELECT_EQUIP_1; i < (int)SBUTTONS::SELECT_END; ++i) {
			FLOAT wid = mButton[(SBUTTONS)i].box.right - mButton[(SBUTTONS)i].box.left;
			FLOAT hei = mButton[(SBUTTONS)i].box.bottom - mButton[(SBUTTONS)i].box.top;
			FLOAT halfWid = wid * 0.5f;
			FLOAT halfHei = hei * 0.5f;
			Vector2 rendPos = Vector2(mButton[(SBUTTONS)i].box.left + halfWid, 
				mButton[(SBUTTONS)i].box.top + halfHei);

			if (iter->second != nullptr) {

				DRAW->render("EquipCardBk", Vector2(wid, halfHei), Vector2(rendPos.x, rendPos.y - 60));

				iter->second->render(Vector2(rendPos.x, rendPos.y - 60), Vector2(wid, halfHei), 1.0f);

				DRAW->render("EquipCard", Vector2(wid, hei), rendPos);

				DWRITE->Change_Text("EQUIP_NAME", iter->second->getString().name);
				DWRITE->TextRender("EQUIP_NAME", rendPos.x - halfWid, rendPos.y + 30, wid, 40, ColorF(1, 1, 1),
					DWRITE_TEXT_ALIGNMENT_CENTER);

				DWRITE->Change_Text("EQUIP_EXP", iter->second->getString().spec);
				DWRITE->TextRender("EQUIP_EXP", rendPos.x - halfWid + 2, rendPos.y + 95, wid - 4, 100, ColorF(1, 1, 1),
					DWRITE_TEXT_ALIGNMENT_CENTER);

				DWRITE->Change_Text("EQUIP_EXP", iter->second->getString().native);
				DWRITE->TextRender("EQUIP_EXP", rendPos.x - halfWid + 2, rendPos.y + 145, wid - 4, 100, ColorF(1, 1, 1),
					DWRITE_TEXT_ALIGNMENT_CENTER);
			
			}
			else
				DRAW->render("EquipEmpty", Vector2(wid, hei), rendPos);
			++iter;
		}
	}

	atlas = IMAGEMAP->getUiAtlas("EquipConv");
	DRAW->render(atlas->textureKey, atlas->alphaTexKey, Vector2(180, 225),
		Vector2(STATUS_RENDER_X, STATUS_RENDER_Y), atlas->mixTexCoord, atlas->maxTexCoord);

	// Status Rendering
	{
		if (selectedDoll != nullptr)
		{		
			string str = ConvertFormat("°ø°Ý·Â : %d >>", originStatus.AttackPoint);
			DWRITE->Change_Text("STATUS_IDX", str);
			DWRITE->TextRender("STATUS_IDX", STATUS_RENDER_X - 160, STATUS_RENDER_Y - 200, 340, 25, ColorF(1, 1, 1),
				DWRITE_TEXT_ALIGNMENT_LEADING);

			DWRITE->ChangeText("STATUS_VAL", "%d", selectedDoll->getStatus().AttackPoint);
			DWRITE->TextRender("STATUS_VAL", STATUS_RENDER_X - 160 + (str.size() * 10), STATUS_RENDER_Y - 200, 
				originStatus.AttackPoint == selectedDoll->getStatus().AttackPoint ?
				ColorF(1, 1, 1) : originStatus.AttackPoint > selectedDoll->getStatus().AttackPoint ? 
				ColorF(1, 0, 0) : ColorF(1, 1, 0));
			
		
			str = ConvertFormat("Ä¡¸íÅ¸ È®·ü : %.1f %% >>", originStatus.CriticPoint);
			DWRITE->Change_Text("STATUS_IDX", str);
			DWRITE->TextRender("STATUS_IDX", STATUS_RENDER_X - 160, STATUS_RENDER_Y - 170, 340, 25, ColorF(1, 1, 1),
				DWRITE_TEXT_ALIGNMENT_LEADING);
				
			DWRITE->ChangeText("STATUS_VAL", "%.1f %%", selectedDoll->getStatus().CriticPoint);
			DWRITE->TextRender("STATUS_VAL", STATUS_RENDER_X - 160 + (str.size() * 10), STATUS_RENDER_Y - 170,
				fabsf(originStatus.CriticPoint - selectedDoll->getStatus().CriticPoint) < fabsf(FLOAT_EPSILON) ?
				ColorF(1, 1, 1) : originStatus.CriticPoint > selectedDoll->getStatus().CriticPoint ?
				ColorF(1, 0, 0) : ColorF(1, 1, 0));


			str = ConvertFormat("Ä¡¸íÅ¸ ¹èÀ² : %.1f %% >>", originStatus.CriticAcl + 100.0);
			DWRITE->Change_Text("STATUS_IDX", str);
			DWRITE->TextRender("STATUS_IDX", STATUS_RENDER_X - 160, STATUS_RENDER_Y - 140, 340, 25, ColorF(1, 1, 1),
				DWRITE_TEXT_ALIGNMENT_LEADING);

			DWRITE->ChangeText("STATUS_VAL", "%.1f %%", selectedDoll->getStatus().CriticAcl + 100.0);
			DWRITE->TextRender("STATUS_VAL", STATUS_RENDER_X - 160 + (str.size() * 10), STATUS_RENDER_Y - 140,
				fabsf(originStatus.CriticAcl - selectedDoll->getStatus().CriticAcl) < fabsf(FLOAT_EPSILON) ?
				ColorF(1, 1, 1) : originStatus.CriticAcl > selectedDoll->getStatus().CriticAcl ?
				ColorF(1, 0, 0) : ColorF(1, 1, 0));


			str = ConvertFormat("¸íÁß·ü : %.2f %% >>", originStatus.Accuracy * 100.0);
			DWRITE->Change_Text("STATUS_IDX", str);
			DWRITE->TextRender("STATUS_IDX", STATUS_RENDER_X - 160, STATUS_RENDER_Y - 110, 340, 25, ColorF(1, 1, 1),
				DWRITE_TEXT_ALIGNMENT_LEADING);

			DWRITE->ChangeText("STATUS_VAL", "%.2f %%", selectedDoll->getStatus().Accuracy * 100.0);
			DWRITE->TextRender("STATUS_VAL", STATUS_RENDER_X - 160 + (str.size() * 10), STATUS_RENDER_Y - 110,
				fabsf(originStatus.Accuracy - selectedDoll->getStatus().Accuracy) < fabsf(FLOAT_EPSILON) ?
				ColorF(1, 1, 1) : originStatus.Accuracy > selectedDoll->getStatus().Accuracy ?
				ColorF(1, 0, 0) : ColorF(1, 1, 0));


			str = ConvertFormat("È¸ÇÇÀ² : %.2f %% >>", originStatus.Avoid * 100.0);
			DWRITE->Change_Text("STATUS_IDX", str);
			DWRITE->TextRender("STATUS_IDX", STATUS_RENDER_X - 160, STATUS_RENDER_Y - 80, 340, 25, ColorF(1, 1, 1),
				DWRITE_TEXT_ALIGNMENT_LEADING);

			DWRITE->ChangeText("STATUS_VAL", "%.2f %%", selectedDoll->getStatus().Avoid * 100.0);
			DWRITE->TextRender("STATUS_VAL", STATUS_RENDER_X - 160 + (str.size() * 10), STATUS_RENDER_Y - 80,
				fabsf(originStatus.Avoid - selectedDoll->getStatus().Avoid) < fabsf(FLOAT_EPSILON) ?
				ColorF(1, 1, 1) : originStatus.Avoid > selectedDoll->getStatus().Avoid ?
				ColorF(1, 0, 0) : ColorF(1, 1, 0));

			DWRITE->ChangeText("STATUS_IDX", "Ã¼·Â : %d", selectedDoll->getStatus().HitPoint.max);
			DWRITE->TextRender("STATUS_IDX", STATUS_RENDER_X - 160, STATUS_RENDER_Y - 50, 340, 25, ColorF(1, 1, 1),
				DWRITE_TEXT_ALIGNMENT_LEADING);

			DWRITE->ChangeText("STATUS_IDX", "¹æ¾î·Â : %d", selectedDoll->getStatus().Armor);
			DWRITE->TextRender("STATUS_IDX", STATUS_RENDER_X - 160, STATUS_RENDER_Y - 20, 340, 25, ColorF(1, 1, 1),
				DWRITE_TEXT_ALIGNMENT_LEADING);


			str = ConvertFormat("¹æ¾î ÆÄ¼â¼öÄ¡ : %d >>", originStatus.ArmorPierce);
			DWRITE->Change_Text("STATUS_IDX", str);
			DWRITE->TextRender("STATUS_IDX", STATUS_RENDER_X - 160, STATUS_RENDER_Y + 10, 340, 25, ColorF(1, 1, 1),
				DWRITE_TEXT_ALIGNMENT_LEADING);

			DWRITE->ChangeText("STATUS_VAL", "%d", selectedDoll->getStatus().ArmorPierce);
			DWRITE->TextRender("STATUS_VAL", STATUS_RENDER_X - 160 + (str.size() * 10), STATUS_RENDER_Y + 10,
				originStatus.ArmorPierce == selectedDoll->getStatus().ArmorPierce ?
				ColorF(1, 1, 1) : originStatus.ArmorPierce > selectedDoll->getStatus().ArmorPierce ?
				ColorF(1, 0, 0) : ColorF(1, 1, 0));

			str = ConvertFormat("¹æ¾î °üÅë·ü : %.2f %% >>", originStatus.ArmorIgnore * 100.0);
			DWRITE->Change_Text("STATUS_IDX", str);
			DWRITE->TextRender("STATUS_IDX", STATUS_RENDER_X - 160, STATUS_RENDER_Y + 40, 340, 25, ColorF(1, 1, 1),
				DWRITE_TEXT_ALIGNMENT_LEADING);

			DWRITE->ChangeText("STATUS_VAL", "%.2f %%", selectedDoll->getStatus().ArmorIgnore * 100.0);
			DWRITE->TextRender("STATUS_VAL", STATUS_RENDER_X - 160 + (str.size() * 10), STATUS_RENDER_Y + 40,
				fabsf(originStatus.ArmorIgnore - selectedDoll->getStatus().ArmorIgnore) < fabsf(FLOAT_EPSILON) ?
				ColorF(1, 1, 1) : originStatus.ArmorIgnore > selectedDoll->getStatus().ArmorIgnore ?
				ColorF(1, 0, 0) : ColorF(1, 1, 0));

			if (selectedDoll->getWeaponType() == TWT_SG) {
				DWRITE->ChangeText("STATUS_IDX", "¹æÆÐ ¼öÄ¡ : %d", selectedDoll->getStatus().ArmorPoint.max);
				DWRITE->TextRender("STATUS_IDX", STATUS_RENDER_X - 160, STATUS_RENDER_Y + 70, 340, 25, ColorF(1, 1, 1),
					DWRITE_TEXT_ALIGNMENT_LEADING);
			}
		}
	}
}

void EquipScene::State_EquipUpdate()
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
	else if (virtualHeight < virtualLimit)
		virtualHeight = virtualLimit;
	
	for (size_t i = 0; i < selBox.size(); ++i) {
		selBox[i].box = D2DRectMake(selBox[i].pos.x, selBox[i].pos.y + virtualHeight,
			CHARACTER_BOX_WID, CHARACTER_BOX_HEI);
	}

	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		for (size_t i = 0; i < selBox.size(); ++i) {

			if (ptInRect(selBox[i].box, g_ptMouse)) {

				EquipBase* selEquip = selBox[i].adress != nullptr ? (EquipBase*)selBox[i].adress : nullptr;

				if (selEquip == nullptr) {
					selectedDoll->p_getEquip().at(SelectedEQ_Type) = nullptr;
					selectedDoll->AttachEquipment();
					state = ES_MAIN;
					mouseDrag = false;
					break;
				}
				else {
					if (selEquip->isAttachAble(selectedDoll->getWeaponType()))
					{
						selectedDoll->p_getEquip().at(SelectedEQ_Type) = selEquip;
						selectedDoll->AttachEquipment();
						state = ES_MAIN;
						mouseDrag = false;
						break;
					}
				}
			}
		}

		if (state == ES_EQUIP) {
			if (ptInRect(mButton[SBUTTONS::TURN_BACK].box, g_ptMouse))
				mButton[SBUTTONS::TURN_BACK].ClickAction(this);
		}
	}


}

void EquipScene::State_EquipRender()
{
	DRAW->render("editSceneBk", Vector2(WINSIZEX, WINSIZEY), Vector2(WINSIZEX*0.5f, WINSIZEY*0.5f));

	if (selBox.size() == 1)	{
		DRAW->render("bkGuard", Vector2(WINSIZEX, WINSIZEY), Vector2(WINSIZEX*0.5f, WINSIZEY*0.5f), COLR(1, 1, 1, 0.4));
		DWRITE->Change_Text("NOTIFICATION", "Âø¿ë°¡´ÉÇÑ Àåºñ°¡ ¾ø½À´Ï´Ù");
		DWRITE->TextRender("NOTIFICATION", 0, WINSIZEY * 0.5f, WINSIZEX, 50, ColorF(1, 1, 0),
			DWRITE_TEXT_ALIGNMENT_CENTER);
	}
	else {

		for (size_t i = 0; i < selBox.size(); ++i) {
			FLOAT wid = selBox[i].box.right - selBox[i].box.left;
			FLOAT hei = selBox[i].box.bottom - selBox[i].box.top;
			FLOAT halfWid = wid * 0.5f;
			FLOAT halfHei = hei * 0.5f;
			Vector2 rendPos;

			if (selBox[i].adress != nullptr) {
				EquipBase* focusedEquip = ((EquipBase*)selBox[i].adress);

				rendPos = Vector2(selBox[i].pos.x + halfWid, selBox[i].pos.y + halfHei + virtualHeight);

				//D2D->renderRect(selBox[i].box, ColorF(1, 0, 0), true);

				DRAW->render("EquipCardBk", Vector2(wid, halfHei), Vector2(rendPos.x, rendPos.y - 60));
				focusedEquip->render(Vector2(rendPos.x, rendPos.y - 60), Vector2(wid, halfHei), 1.0f);
				DRAW->render("EquipCard", Vector2(wid, hei), rendPos);

				DWRITE->Change_Text("EQUIP_NAME", focusedEquip->getString().name);
				DWRITE->TextRender("EQUIP_NAME", rendPos.x - halfWid, rendPos.y + 30, wid, 40, ColorF(1, 1, 1),
					DWRITE_TEXT_ALIGNMENT_CENTER);

				DWRITE->Change_Text("EQUIP_EXP", focusedEquip->getString().spec);
				DWRITE->TextRender("EQUIP_EXP", rendPos.x - halfWid + 2, rendPos.y + 95, wid - 4, 100, ColorF(1, 1, 1),
					DWRITE_TEXT_ALIGNMENT_CENTER);

				DWRITE->Change_Text("EQUIP_EXP", focusedEquip->getString().native);
				DWRITE->TextRender("EQUIP_EXP", rendPos.x - halfWid + 2, rendPos.y + 145, wid - 4, 100, ColorF(1, 1, 1),
					DWRITE_TEXT_ALIGNMENT_CENTER);

			}

			else {
				//D2D->renderRect(selBox[i].box, ColorF(1, 0, 0), false);
				DRAW->render("SquadEmit", Vector2(wid, hei),
					Vector2(selBox[i].pos.x + halfWid, selBox[i].pos.y + halfHei + virtualHeight));
			}
		}
	}

	DRAW->render("gradiantBlack", Vector2(WINSIZEX, 100), Vector2(WINSIZEX*0.5f, 50));

	//D2D->renderRect(mButton[SBUTTONS::TURN_BACK].box, ColorF(1, 0, 0));
	DRAW->render("TurnBack", Vector2(150, 90), Vector2(mButton[SBUTTONS::TURN_BACK].box.left + 75,
		mButton[SBUTTONS::TURN_BACK].box.top + 45));

	DWRITE->ChangeText("TITLE_NAME", "EQUIP");
	DWRITE->TextRender("TITLE_NAME", 1055.0f, 0.0f, ColorF(0.8, 0.8, 0.8));
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

	else if (virtualHeight < virtualLimit)
		virtualHeight = virtualLimit;	

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

				if (selectedDoll != nullptr)
					originStatus = selectedDoll->getMaxStatus();

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

			//D2D->renderRect(selBox[i].box, ColorF(1, 0, 0), true);
			DRAW->render(focusedTdoll->keys.cardNormalKey, Vector2(wid, hei), rendPos);

			DRAW->render("AllCard", Vector2(wid, hei), rendPos);

			DWRITE->ChangeText("CHARA_NAME", focusedTdoll->keys.name);
			DWRITE->TextRender("CHARA_NAME", rendPos.x - halfWid, rendPos.y + halfHei - 60, wid, 40, ColorF(1, 1, 1),
				DWRITE_TEXT_ALIGNMENT_CENTER);
		}
		else {
			//D2D->renderRect(selBox[i].box, ColorF(1, 0, 0), true);
			DRAW->render("SquadEmit", Vector2(wid, hei),
				Vector2(selBox[i].pos.x + halfWid, selBox[i].pos.y + halfHei + virtualHeight));
		}
	}

	DRAW->render("gradiantBlack", Vector2(WINSIZEX, 100), Vector2(WINSIZEX*0.5f, 50));

	//D2D->renderRect(mButton[SBUTTONS::TURN_BACK].box, ColorF(1, 0, 0));
	DRAW->render("TurnBack", Vector2(150, 90), Vector2(mButton[SBUTTONS::TURN_BACK].box.left + 75,
		mButton[SBUTTONS::TURN_BACK].box.top + 45));

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
		selectBox outer;
		outer.pos = Vector2(20 + (counter % objectPtr->WIDTH_COUNT) * objectPtr->CHARACTER_BLANK_WID,
			120 + (counter / objectPtr->WIDTH_COUNT) * objectPtr->CHARACTER_BLANK_HEI);
		outer.box = D2DRectMake(outer.pos.x, outer.pos.y,
			objectPtr->CHARACTER_BOX_WID, objectPtr->CHARACTER_BOX_HEI);
		outer.adress = nullptr;
		objectPtr->selBox.push_back(outer);
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

		if (objectPtr->selBox.size() > objectPtr->WIDTH_COUNT)
			objectPtr->virtualLimit -= (objectPtr->CHARACTER_BLANK_HEI * 0.5f);

		objectPtr->virtualLimit *= -1.0f;

		objectPtr->mouseDrag = false;
	}
}

void EquipScene::EquipmentSelect(void * obj)
{
	objectPtr = (EquipScene*)obj;

	if (objectPtr->state == ES_MAIN && objectPtr->selectedDoll != nullptr)
	{
		objectPtr->EquipNum = 0;

		whlCount = 0;
		objectPtr->virtualHeight = 0.0f;

		size_t counter = 0;
		objectPtr->selBox.clear();
		objectPtr->selBox.reserve(PLAYER->getPlayerEquip().size() + 1);

		//	Insert Null EquipCard For Out of Equip
		selectBox outer;
		outer.pos = Vector2(20 + (counter % objectPtr->WIDTH_COUNT) * objectPtr->CHARACTER_BLANK_WID,
			120 + (counter / objectPtr->WIDTH_COUNT) * objectPtr->CHARACTER_BLANK_HEI);
		outer.box = D2DRectMake(outer.pos.x, outer.pos.y,
			objectPtr->CHARACTER_BOX_WID, objectPtr->CHARACTER_BOX_HEI);
		outer.adress = nullptr;
		objectPtr->selBox.push_back(outer);
		++counter;

		for (auto& it : PLAYER->getPlayerEquip())
		{
			if (it.second.equipType == objectPtr->SelectedEQ_Type)
			{
				if (it.second.equip->isAttachAble(objectPtr->selectedDoll->getWeaponType()))
				{
					selectBox _new;

					_new.pos = Vector2(20 + (counter % objectPtr->WIDTH_COUNT) * objectPtr->CHARACTER_BLANK_WID,
						120 + (counter / objectPtr->WIDTH_COUNT) * objectPtr->CHARACTER_BLANK_HEI);

					_new.box = D2DRectMake(_new.pos.x, _new.pos.y,
						objectPtr->CHARACTER_BOX_WID, objectPtr->CHARACTER_BOX_HEI);

					_new.adress = it.second.equip;

					objectPtr->selBox.push_back(_new);

					++counter;
				}
				++objectPtr->EquipNum;
			}
		}

		objectPtr->virtualLimit =
			(float)(((int)objectPtr->selBox.size()) / objectPtr->WIDTH_COUNT) * objectPtr->CHARACTER_BLANK_HEI;

		if (objectPtr->selBox.size() > objectPtr->WIDTH_COUNT)
			objectPtr->virtualLimit -= (objectPtr->CHARACTER_BLANK_HEI * 0.5f);

		objectPtr->virtualLimit *= -1.0f;

		objectPtr->state = ES_EQUIP;	
		objectPtr->mouseDrag = false;

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
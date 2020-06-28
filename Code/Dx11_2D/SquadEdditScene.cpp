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
	LOAD->Add_LoadTray("Squad_1_Already", "Texture2D/Squad1Already.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 1 Á¦´ë ¹èÄ¡Áß
	LOAD->Add_LoadTray("Squad_2_Already", "Texture2D/Squad2Already.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 2 Á¦´ë ¹èÄ¡Áß
	LOAD->Add_LoadTray("Squad_3_Already", "Texture2D/Squad3Already.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);		// 3 Á¦´ë ¹èÄ¡Áß
	
	LOAD->Add_LoadTray("Squad_1_Leader", "Texture2D/Squad1Leader.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// 
	LOAD->Add_LoadTray("Squad_2_Leader", "Texture2D/Squad2Leader.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// 
	LOAD->Add_LoadTray("Squad_3_Leader", "Texture2D/Squad3Leader.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// 
	
	LOAD->Add_LoadTray("SquadEmit", "Texture2D/SquadEmit.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);					// 
	
	LOAD->Add_LoadTray("SquadBar", "Texture2D/SquadBar.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);					// 
	LOAD->Add_LoadTray("SquadBar_s", "Texture2D/SquadBar_select.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// 
	
	LOAD->Add_LoadTray("editSceneBk", "Texture2D/editSceneBk.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);				// 
	
	LOAD->Add_LoadTray("gradiantBlack", "Texture2D/gradiantBlack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);			// 
	LOAD->Add_LoadTray("SlotSquad", "Texture2D/SlotSquad.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);					// 
	
	LOAD->Add_LoadTray("HomeButton", "Texture2D/HomeButton.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);				// 
	LOAD->Add_LoadTray("AllCard", "Texture2D/AllCard.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);						// 

	worldColor.a = 0.0f;

	mode = SQUAD;

	FocusSquad = 1;
	FocusBox = -1;

	showEquip = false;

	virtualHeight = 0.0f;
	virtualLimit = 0.0f;
	asixVirtual = 0;
	mouseDrag = false;

	DWRITE->Create_TextField("SQUAD", L"¸¼Àº°íµñ", "NULL", 36, DWRITE_FONT_WEIGHT_BOLD);
	DWRITE->Create_TextField("CHARA_NAME", L"¸¼Àº°íµñ", "NULL", 28, DWRITE_FONT_WEIGHT_MEDIUM);
	DWRITE->Create_TextField("TITLE_NAME", L"¸¼Àº°íµñ", "NULL", 65, DWRITE_FONT_WEIGHT_BOLD);

	mButton.insert(make_pair(SBUTTONS::HOME_BACK, Button(10, 10, 100, 85, ReturnBase_Select)));
	mButton.insert(make_pair(SBUTTONS::TURN_BACK, Button(10, 10, 100, 85, ReturnBase_Select)));

	mButton.insert(make_pair(SBUTTONS::SELECT_SQUAD_1, Button(0, 200, 150, 100, ChangeSquad_Select)));
	mButton.insert(make_pair(SBUTTONS::SELECT_SQUAD_2, Button(0, 320, 150, 100, ChangeSquad_Select)));
	mButton.insert(make_pair(SBUTTONS::SELECT_SQUAD_3, Button(0, 440, 150, 100, ChangeSquad_Select)));

	mButton.insert(make_pair(SBUTTONS::SELECT_CHARA_1, Button(180, 200, SQUAD_BOX_WIDTH, SQUAD_BOX_HEIGHT, InsertSelect)));
	mButton.insert(make_pair(SBUTTONS::SELECT_CHARA_2, Button(400, 200, SQUAD_BOX_WIDTH, SQUAD_BOX_HEIGHT, InsertSelect)));
	mButton.insert(make_pair(SBUTTONS::SELECT_CHARA_3, Button(620, 200, SQUAD_BOX_WIDTH, SQUAD_BOX_HEIGHT, InsertSelect)));
	mButton.insert(make_pair(SBUTTONS::SELECT_CHARA_4, Button(840, 200, SQUAD_BOX_WIDTH, SQUAD_BOX_HEIGHT, InsertSelect)));
	mButton.insert(make_pair(SBUTTONS::SELECT_CHARA_5, Button(1060, 200, SQUAD_BOX_WIDTH, SQUAD_BOX_HEIGHT, InsertSelect)));

	vBox.clear();
	vCharacter.clear();
	vCharacter.resize(5, "");

	for (auto& it : PLAYER->getPlayerTaticDoll().getAllDolls())		//	get List of Player's TacticalDoll
		it.second->LoadTray_ImageList();

	changeScene = false;

	//	UnLoad List
	vLoadList.clear();
	for (auto& it : LOAD->getLoadList())
		vLoadList.push_back(LoadResourceData(it->key, it->type));

	for (int i = 0; i < vCharacter.size(); ++i)
	{
		if (i < PLAYER->getPlayerSquad(FocusSquad)->squadMember.size())
			vCharacter[i] = PLAYER->getPlayerSquad(FocusSquad)->squadMember[i]->keys.cardNormalKey;

		else
			vCharacter[i] = "";
	}

	atlas = nullptr;

	//	Loading Setting
	LOAD->setAutoInit(false);
	LOAD->setNextScene("SQUAD");
	LOAD->setLoadImageKey("ShootRange");

	LOAD->mallocThread();
	SCENE->changeScene("LOAD");

	SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);
	CAMERA->CameraReset();
}

void SquadEdditScene::release()
{
}

void SquadEdditScene::update()
{
	CAMERA->setCameraFix(true);

	if (!changeScene)
	{
		SOUND->Play_Sound(SOUND_CHANNEL::CH_SOUND1, "FormationLoop", 0.25f);
		SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, worldColor.a < 0.15f ? worldColor.a : 0.15f);

		worldColor.a = worldColor.a < 1.0f ? worldColor.a + DELTA() : 1.0f;

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
			worldColor.a -= DELTA() * 2.0f;

		else
		{
			worldColor.a = 0.0f;

			LOAD->insertUnLoadList(vLoadList);

			SOUND->Stop_Sound(SOUND_CHANNEL::CH_SOUND1, "FormationLoop");
			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);

			LOAD->setAutoInit(true);
			LOAD->setNextScene("LOBBY");
			LOAD->setLoadImageKey("ShootRange");
			SCENE->changeScene("UNLOAD");
			SOUND->setVolume(SOUND_CHANNEL::CH_SOUND1, 0.0f);
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
		for (auto& it : mButton) {

			if (ptInRect(it.second.box, g_ptMouse))
				it.second.ClickAction(this);
		}
	}

	KeyInputAction();
}

void SquadEdditScene::Squad_Render()
{
	DRAW->render("editSceneBk", Vector2(WINSIZEX, WINSIZEY), Vector2(WINSIZEX*0.5f, WINSIZEY*0.5f));
	DRAW->render("gradiantBlack", Vector2(WINSIZEX, 100), Vector2(WINSIZEX*0.5f, 50));

	DWRITE->ChangeText("TITLE_NAME", "SQUAD");
	DWRITE->TextRender("TITLE_NAME", 1045.0f, 0.0f, ColorF(0.8, 0.8, 0.8));

	for (int i = 0; i < vCharacter.size(); ++i) {	
		auto key = vCharacter[i];
		if (key.size() < 2) continue;

		DRAW->render(PLAYER->getPlayerSquad(FocusSquad)->squadMember[i]->keys.cardNormalKey, 
			Vector2(SQUAD_BOX_H_WIDTH * 2, SQUAD_BOX_H_HEIGHT * 2),
			Vector2(180 + (i * (SQUAD_BOX_H_WIDTH + 130) + SQUAD_BOX_H_WIDTH), 200 + SQUAD_BOX_H_HEIGHT));
	}

	DRAW->render("HomeButton", Vector2(100, 85), 
		Vector2(mButton[SBUTTONS::HOME_BACK].box.left + 50, mButton[SBUTTONS::HOME_BACK].box.top + 40));

	for (int i = (int)SBUTTONS::SELECT_SQUAD_1; i < (int)SBUTTONS::SELECT_CHARA_1; ++i)	{

		DRAW->render(FocusSquad == i ? "SquadBar_s" : "SquadBar", 
			Vector2(150, 100), Vector2(mButton[(SBUTTONS)i].box.left + 75, mButton[(SBUTTONS)i].box.top + 50));

		DWRITE->ChangeText("SQUAD", "%dÁ¦´ë", (SBUTTONS)i);
		DWRITE->TextRender("SQUAD", mButton[(SBUTTONS)i].box.left, mButton[(SBUTTONS)i].box.top + 25, 150, 100,
			ColorF(0, 0, 0), DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	}

	Squad_render_Character();
}

void SquadEdditScene::Squad_render_Character()
{
	for (int i = (int)SBUTTONS::SELECT_CHARA_1; i < (int)SBUTTONS::END; ++i) {

		D2D->renderRect(mButton[(SBUTTONS)i].box.left, mButton[(SBUTTONS)i].box.top,
			mButton[(SBUTTONS)i].box.right - mButton[(SBUTTONS)i].box.left,
			mButton[(SBUTTONS)i].box.bottom - mButton[(SBUTTONS)i].box.top, ColorF(0, 0, 0));

		atlas = PLAYER->getPlayerSquad(FocusSquad)->squadMember.count(i - 4) ?
			IMAGEMAP->getUiAtlas("InstOvSlot") : IMAGEMAP->getUiAtlas("InstSlot");

		DRAW->render(atlas->textureKey, atlas->alphaTexKey, Vector2(SQUAD_BOX_H_WIDTH, SQUAD_BOX_H_HEIGHT),
			Vector2(mButton[(SBUTTONS)i].box.left + SQUAD_BOX_H_WIDTH, mButton[(SBUTTONS)i].box.top + SQUAD_BOX_H_HEIGHT),
			atlas->mixTexCoord, atlas->maxTexCoord);

		DRAW->render("SlotSquad", Vector2(SQUAD_BOX_WIDTH, SQUAD_BOX_HEIGHT * 0.37f),
			Vector2(mButton[(SBUTTONS)i].box.left + SQUAD_BOX_H_WIDTH, 
				mButton[(SBUTTONS)i].box.bottom - (SQUAD_BOX_H_HEIGHT * 0.37f)));
	}

	for (auto& it : PLAYER->getPlayerSquad(FocusSquad)->squadMember) {

		int chara = (int)SBUTTONS::SELECT_CHARA_1;

		DWRITE->Change_Text("DOLLNAME", it.second->getName());

		chara += it.second->getID()->SquadMem_ID;

		DWRITE->TextRender("DOLLNAME", mButton[(SBUTTONS)chara].box.left, mButton[(SBUTTONS)chara].box.bottom -
			(SQUAD_BOX_H_HEIGHT * 0.74f) + 28, 180, 40, ColorF(1, 1, 1), DWRITE_TEXT_ALIGNMENT_CENTER);
	}
}

void SquadEdditScene::All_Update()
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

	for (size_t i = 0; i < vBox.size(); ++i) {
		vBox[i].box = D2DRectMake(vBox[i].pos.x, vBox[i].pos.y + virtualHeight,
			CHARACTER_BOX_WID, CHARACTER_BOX_HEI);
	}

	if (KEYMANAGER->isKeyDown(VK_LBUTTON)) {

		for (size_t i = 0; i < vBox.size(); ++i) {

			if (ptInRect(vBox[i].box, g_ptMouse)) {

				if (vBox[i].adress != nullptr) {

					if (PLAYER->getPlayerSquad(FocusSquad)->squadMember.count(FocusBox)) {
						if (PLAYER->getPlayerTaticDoll().changeSquadTacDoll(FocusSquad, i - 1, FocusBox) != E_FAIL)
							SOUND->Play_Effect(CH_VOICE, 
								PLAYER->getPlayerTaticDoll().getAllDolls().at(i - 1)->keys.SOUND_FORMATION, 0.15f);
					}

					else {

						if (PLAYER->getPlayerSquad(FocusSquad)->squadMember.size() < 5)	{
							if (PLAYER->getPlayerTaticDoll().insertSquadTacDoll(FocusSquad, i - 1) != E_FAIL)
								SOUND->Play_Effect(CH_VOICE, 
									PLAYER->getPlayerTaticDoll().getAllDolls().at(i - 1)->keys.SOUND_FORMATION, 0.15f);
						}
					}
				}

				else {
					if (PLAYER->getPlayerTaticDoll().getSquadMember(FocusSquad, FocusBox) != nullptr)
						PLAYER->getPlayerTaticDoll().exitSquadTacDoll(FocusSquad, FocusBox);
				}

				for (int i = 0; i < vCharacter.size(); ++i)
				{
					if (i < PLAYER->getPlayerSquad(FocusSquad)->squadMember.size())
						vCharacter[i] = PLAYER->getPlayerSquad(FocusSquad)->squadMember[i]->keys.cardNormalKey;

					else
						vCharacter[i] = "";
				}

				mode = SHOWMODE::SQUAD;
				mouseDrag = false;
				break;
			}
		}
	}

}

void SquadEdditScene::All_Render()
{
	DRAW->render("editSceneBk", Vector2(WINSIZEX, WINSIZEY), Vector2(WINSIZEX*0.5f, WINSIZEY*0.5f));

	for (size_t i = 0; i < vBox.size(); ++i) {
		FLOAT wid = vBox[i].box.right - vBox[i].box.left;
		FLOAT hei = vBox[i].box.bottom - vBox[i].box.top;
		FLOAT halfWid = wid * 0.5f;
		FLOAT halfHei = hei * 0.5f;
		Vector2 rendPos;

		if (vBox[i].adress != nullptr) {
			BaseTaticDoll* focusedTdoll = ((BaseTaticDoll*)vBox[i].adress);

			rendPos = Vector2(vBox[i].pos.x + halfWid, vBox[i].pos.y + halfHei + virtualHeight);

			DRAW->render(focusedTdoll->keys.cardNormalKey, Vector2(wid, hei), rendPos);

			DRAW->render("AllCard", Vector2(wid, hei), rendPos);

			DWRITE->ChangeText("CHARA_NAME", focusedTdoll->keys.name);
			DWRITE->TextRender("CHARA_NAME", rendPos.x - halfWid, rendPos.y + halfHei - 60, wid, 40, ColorF(1, 1, 1),
				DWRITE_TEXT_ALIGNMENT_CENTER);

			if (focusedTdoll->getID()->Squad_ID != -1) {
				DRAW->render("bkGuard", Vector2(wid, hei), rendPos, COLR(1, 1, 1, 0.4f));

				string key = ConvertFormat("Squad_%d_Already", focusedTdoll->getID()->Squad_ID);
				DRAW->render(key, Vector2(65, 70), Vector2(rendPos.x + 35, rendPos.y - 40));
			}
		}
		else {

			DRAW->render("SquadEmit", Vector2(wid, hei),
				Vector2(vBox[i].pos.x + halfWid, vBox[i].pos.y + halfHei + virtualHeight));
		}
	}

	DRAW->render("gradiantBlack", Vector2(WINSIZEX, 100), Vector2(WINSIZEX*0.5f, 50));

	DWRITE->ChangeText("TITLE_NAME", "T-DOLL");
	DWRITE->TextRender("TITLE_NAME", 1045.0f, 0.0f, ColorF(0.8, 0.8, 0.8));

}

void SquadEdditScene::KeyInputAction()
{
	//	ÀåÂøµÈ Àåºñ È®ÀÎ
	if (KEYMANAGER->isKeyUp(VK_TAB))
		showEquip = showEquip ? false : true;
}

void SquadEdditScene::Allocate_Box_All()
{
	auto& pTacDoll = PLAYER->getPlayerTaticDoll().getAllDolls();
	size_t counter = 0;

	vBox.clear();
	vBox.reserve(pTacDoll.size() + 1);

	//	Insert Null Character For Out of Character
	selectBox outer;

	outer.pos = Vector2(20 + (counter % WIDTH_COUNT) * CHARACTER_BLANK_WID, 
		120 + (counter / WIDTH_COUNT) * CHARACTER_BLANK_HEI);
	outer.box = D2DRectMake(outer.pos.x, outer.pos.y, CHARACTER_BOX_WID, CHARACTER_BOX_HEI);
	outer.adress = nullptr;

	vBox.push_back(outer);

	++counter;

	for (auto& iter : pTacDoll) {
		selectBox _new;

		_new.pos = Vector2(20 + (counter % WIDTH_COUNT) * CHARACTER_BLANK_WID,
			120 + (counter / WIDTH_COUNT) * CHARACTER_BLANK_HEI);
		_new.box = D2DRectMake(_new.pos.x, _new.pos.y, CHARACTER_BOX_WID, CHARACTER_BOX_HEI);
		_new.adress = iter.second;

		vBox.push_back(_new);
		++counter;
	}

	virtualLimit = (float)(((int)vBox.size()) / WIDTH_COUNT) * CHARACTER_BLANK_HEI;
	if (vBox.size() > WIDTH_COUNT)	
		virtualLimit -= (CHARACTER_BLANK_HEI * 0.5f);
	virtualLimit *= -1.0f;
}

void SquadEdditScene::InsertSelect(void * obj)
{
	SquadEdditScene* object = (SquadEdditScene*)obj;

	for (auto& it : object->mButton) {

		if (it.first < SBUTTONS::SELECT_CHARA_1 || it.first > SBUTTONS::SELECT_CHARA_5) continue;

		if (ptInRect(it.second.box, g_ptMouse)) {

			object->FocusBox = (int)(it.first) - (int)(SBUTTONS::SELECT_CHARA_1);
			break;
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
			PLAYER->getPlayerTaticDoll().exitSquadTacDoll(object->FocusSquad, object->FocusBox);

			object->vBox.pop_back();
			object->FocusBox = -1;
		}
	}
}

void SquadEdditScene::ChangeSquad_Select(void * obj)
{
	SquadEdditScene* object = (SquadEdditScene*)obj;

	for (auto& it : object->mButton) {

		if (it.first < SBUTTONS::SELECT_SQUAD_1 || it.first > SBUTTONS::SELECT_SQUAD_3) continue;

		if (ptInRect(it.second.box, g_ptMouse)) {

			switch (it.first)
			{
			case SBUTTONS::SELECT_SQUAD_1:
			case SBUTTONS::SELECT_SQUAD_2:
			case SBUTTONS::SELECT_SQUAD_3:
				object->FocusSquad = (int)it.first;
				break;

			default:
				break;
			}
		
			for (int i = 0; i < object->vCharacter.size(); ++i)
			{
				if (i < PLAYER->getPlayerSquad(object->FocusSquad)->squadMember.size())
					object->vCharacter[i] = 
					PLAYER->getPlayerSquad(object->FocusSquad)->squadMember[i]->keys.cardNormalKey;

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
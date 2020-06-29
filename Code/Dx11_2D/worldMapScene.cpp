#include "stdafx.h"
#include "worldMapScene.h"

worldMapScene* worldMapScene::objectPtr = NULL;

worldMapScene::worldMapScene()
{
	worldmapInit = false;
	squadInit = false;
	isInsertSquad = false;
	isThermalSupport = false;

	mButton.insert(make_pair(BCODE_TURN, Button(1195, 670, 150, 80, TurnChange_Select)));					//	�� �ѱ�
	mButton.insert(make_pair(BCODE_BATCH, Button(1195, 540, 150, 60, SquadBatch_Select)));					//	��ġ������ ������ �������� Ȱ��ȭ
	mButton.insert(make_pair(BCODE_GIVEUP, Button(100, 10, 150, 80, GiveUpGame_Select)));					//	���� ����

	mButton.insert(make_pair(BCODE_INSERT, Button(1000, 670, 150, 80, InsertSquad_Select)));				//	�ش� ���� ��ġ
	mButton.insert(make_pair(BCODE_BACK, Button(10, 20, 80, 40, BackToNormal_Select)));						//	�ڷΰ���
	mButton.insert(make_pair(BCODE_NORMAL_SQUAD, Button(1000, 400, 150, 80, NormalSquad_Select)));			//	�Ϲ� �����Ҵ� ����
	mButton.insert(make_pair(BCODE_THERMAL_SQUAD, Button(1000, 500, 150, 80, ThermalSquad_Select)));		//	ȭ������ �Ҵ� ����

	for (size_t i = 0; i < PLAYER->getPlayerTaticDoll().getSquadSize(); ++i)
		squadButton.push_back(D2DRectMake(0, 250 + (i * 90), 150, 75));
}

worldMapScene::~worldMapScene()
{
}

void worldMapScene::init()
{
	if (!worldmapInit)
	{
		worldColor.a = 1.0f;
		Focus_Squad = -1;
		rendSquad = false;

		Turn = 0;
		turnAliance = ALIANCE_GRIFFON;

		LOAD->Add_LoadTray("Panel_Normal", "Texture2D/Panel.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
		LOAD->Add_LoadTray("Panel_Heli", "Texture2D/Heliport.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
		LOAD->Add_LoadTray("Panel_Hq", "Texture2D/HQ.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
		LOAD->Add_LoadTray("WAY", "Texture2D/waypoint.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
		LOAD->Add_LoadTray("SquadBar", "Texture2D/SquadBar.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);					 
		LOAD->Add_LoadTray("SquadBar_s", "Texture2D/SquadBar_select.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

		CAMERA->CameraReset();

		for (auto& it : PLAYER->getPlayerTaticDoll().getAllDolls())		//	get List of Player's TacticalDoll
		{
			it.second->LoadTray_SoundList();
			it.second->LoadTray_ImageList();
			//PLAYER->insertTacDolToSquad(it.first, 1);
		}

		mInstSquad.clear();
		for (int i = 0; i < 5; ++i)
		{
			vector<UINT> vint;
			mInstSquad.insert(make_pair((TATICDOLL_ALIANCE_TYPE)(i + 1), vint));
		}

		//	@ �ӽ÷� �� �д� ���� �ؼ�, �����ϱ�
		{
			BDATA->getSquadSNV()->insertSquadMember(1, new Strelet, false);
			BDATA->getSquadSNV()->insertSquadMember(1, new Strelet, false);
			BDATA->getSquadSNV()->insertSquadMember(1, new Strelet, false);

			BDATA->getSquadSNV()->callSquad(1)->squadLeader = BDATA->getSquadSNV()->callSquadMember(1, BDATA->getSquadSNV()->callSquad(1)->squadLeaderID);
			BDATA->getSquadSNV()->callSquad(1)->nowNodeID = 1;
			mInstSquad[ALIANCE_PEREDEUS].push_back(1);



			BDATA->getSquadSNV()->insertSquadMember(2, new Strelet, false);
			BDATA->getSquadSNV()->insertSquadMember(2, new Strelet, false);
			BDATA->getSquadSNV()->insertSquadMember(2, new Strelet, false);

			BDATA->getSquadSNV()->callSquad(2)->squadLeader = BDATA->getSquadSNV()->callSquadMember(2, BDATA->getSquadSNV()->callSquad(2)->squadLeaderID);
			BDATA->getSquadSNV()->callSquad(2)->nowNodeID = 4;
			mInstSquad[ALIANCE_PEREDEUS].push_back(2);

			for (auto& it : mInstSquad.find(ALIANCE_PEREDEUS)->second)
			{
				BaseTaticDoll* leader = BDATA->getSquadSNV()->callSquad(it)->squadLeader;
				SquadData* squad = BDATA->getSquadSNV()->callSquad(it);

				leader->p_getCharacterPos()->x = MAP->pManager->findPanel(squad->nowNodeID)->getPanelPos().x;
				leader->p_getCharacterPos()->y = MAP->pManager->findPanel(squad->nowNodeID)->getPanelPos().y;
			}

		}


		//	=============================================================

		//			@@@		UnLoad Test		@@@

		//	=============================================================

		LOAD->setAutoInit(false);
		LOAD->setNextScene("WORLD");

		vLoadList.clear();
		for (auto& it : LOAD->getLoadList())
			vLoadList.push_back(LoadResourceData(it->key, it->type));

		//Helicopter::init();

		LOAD->mallocThread();
		SCENE->changeScene("LOAD");

		MAP->Check_ActionPoint();
		MAP->setActionPoint(MAP->getMaxActionPoint());

		worldmapInit = true;
	}

	if (!squadInit)
	{
		for (auto& aliance : mInstSquad)
		{
			for (int i = aliance.second.size() - 1; i > -1; --i)
			{
				UINT squadID = aliance.second[i];

				switch (aliance.first)
				{
				case ALIANCE_GRIFFON:
				{
					//	�����̶��...
					if (PLAYER->getPlayerSquad(squadID)->AllDead)
						aliance.second.erase(aliance.second.begin() + i);

					//	�׷��� �ʴٸ�
					else
					{
						int panelID = PLAYER->getPlayerSquad(squadID)->nowNodeID;
						if (panelID != -1)
						{
							BaseTaticDoll* leader = PLAYER->getPlayerSquad(squadID)->squadLeader;
							D3DXVECTOR2 panelPos = MAP->pManager->findPanel(panelID)->getPanelPos();

							leader->init();

							leader->p_getCharacterPos()->x = panelPos.x;
							leader->p_getCharacterPos()->y = panelPos.y;
						}
					}
				}
				break;

				case ALIANCE_IRONBLOD:
				case ALIANCE_PEREDEUS:
				case ALIANCE_SCCOM:
				case ALIANCE_NONE:
				{
					SquadData* squad = BDATA->getSquadSNV()->callSquad(squadID);

					if (squad->AllDead)
						aliance.second.erase(aliance.second.begin() + i);

					else
					{
						BaseTaticDoll* leader = BDATA->getSquadSNV()->callSquad(squadID)->squadLeader;
						D3DXVECTOR2 panelPos = MAP->pManager->findPanel(squad->nowNodeID)->getPanelPos();

						leader->p_getCharacterPos()->x = panelPos.x;
						leader->p_getCharacterPos()->y = panelPos.y;
					}
				}
				break;
				}
			}
		}

		squadInit = true;
	}
}

void worldMapScene::release()
{
}

void worldMapScene::update()
{
	//	false
	CAMERA->setCameraFix(isInsertSquad);
	MAP->update();

	if (CameraPositionX < WINSIZEX * (-0.5f))
		CameraPositionX = WINSIZEX * (-0.5f);

	if (CameraPositionX > (WINSIZEX * 0.5f))
		CameraPositionX = (WINSIZEX * 0.5f);

	if (CameraPositionY < WINSIZEY * (-0.5f))
		CameraPositionY = WINSIZEY * (-0.5f);
					  
	if (CameraPositionY > WINSIZEY * 0.5f)
		CameraPositionY = WINSIZEY * 0.5f;

	//Helicopter::UpdateHelicopter();

	keyUpate();

	//	�д뺰 �д��� ������Ʈ
	for (auto& it : mInstSquad.find(ALIANCE_GRIFFON)->second)
	{
		//	������ �����ߴ��� ���θ� üũ
		for (auto& enemyID : mInstSquad.find(ALIANCE_PEREDEUS)->second)
		{
			if (BDATA->getSquadSNV()->callSquad(enemyID)->AllDead) continue;

			if (BDATA->getSquadSNV()->callSquad(enemyID)->nowNodeID == PLAYER->getPlayerSquad(it)->nowNodeID)
			{
				MAP->pGetMissionFlag() = MAP->pManager->findPanel(PLAYER->getPlayerSquad(it)->nowNodeID)->getPlagData();

				BDATA->insertObject_AtSquad(enemyID);

				//	�ش� �д� ID�� ���� ����
				BDATA->getEngageSquadID() = enemyID;
				PLAYER->getCurrentSquad() = it;

				SOUND->Play_Effect(SOUND_CHANNEL::CH_VOICE, MAP->pGetMissionFlag().battlePlag < 5 ? 
					PLAYER->getPlayerSquad(it)->squadLeader->keys.SOUND_MEET : 
					PLAYER->getPlayerSquad(it)->squadLeader->keys.SOUND_DEFENSE , 0.15f);

				squadInit = false;
				
				PLAYER->getPlayerSquad(it)->squadLeader->setInWorld(false);

				SCENE->changeScene("BATTLE", true);
			}
		}
		BaseTaticDoll* leader = PLAYER->getPlayerSquad(it)->squadLeader;
		leader->getMotion()->update(DELTA());
		leader->MoveClickPoint();
		leader->Update_DrawPos();

		/*if (leader->getMotion()->isCurrent("wait") && !(PLAYER->getPlayerSquad(it)->nextNodeID > 0))
			PLAYER->getPlayerSquad(it)->nowNodeID = PLAYER->getPlayerSquad(it)->nextNodeID;*/
	}

	for (auto& it : mInstSquad.find(ALIANCE_PEREDEUS)->second)
	{
		BaseTaticDoll* leader = BDATA->getSquadSNV()->callSquad(it)->squadLeader;

		leader->getMotion()->update(DELTA());
		leader->Update_DrawPos();
	}
}

void worldMapScene::keyUpate()
{
	//ImGui::Checkbox("RendSquad", &rendSquad);

	//	��Ŭ���� �ൿ
	if (KEYMANAGER->isKeyUp(VK_LBUTTON))
	{
		//	@ Button Action
		if (!isInsertSquad) {

			bool buttonSel = false;

			for (int i = (int)BCODE_TURN; i < (int)BCODE_INSERT; ++i) {
				if (ptInRect(mButton[(BUTTON_CODE)i].box, g_ptMouse)) {
					mButton[(BUTTON_CODE)i].ClickAction(this);
					buttonSel = true;
					break;
				}
			}

			if (buttonSel == false) {

				int curSelectID;
				MAP->pManager->Search_SelectPanel();

				if ((curSelectID = MAP->pManager->getSelectPanelId()) != -1) {

					if (Focus_Squad == -1) {

						bool onSpawnSite = false;

						for (auto& it : mInstSquad.find(ALIANCE_GRIFFON)->second) {

							//	Ŭ���� ��忡 �Ʊ��д밡 �ִٸ�...
							if (PLAYER->getPlayerSquad(it)->nowNodeID == static_cast<UINT>(curSelectID))
							{
								Focus_Squad = it;	//	�ش� �д뿡 ��Ŀ���� �Ѱ��ش�
								onSpawnSite = true;
								break;
							}
						}

						//	Ŭ���� �г����� �ƹ��� �д뵵 �������
						if (!onSpawnSite) {
							//	Ŭ���� �г��� �Ʊ��Ҽ� ���ֺ� / �︮��Ʈ�ϰ��
							if (MAP->pManager->findPanel(curSelectID)->getPanelEnum() != PANEL_CLASS_NONE &&
								MAP->pManager->findPanel(curSelectID)->getPanelAlience() == ALIANCE_GRIFFON)
							{
								rendSquad = true;
								Spawn_Squad = 1;
								Spawn_PanelID = curSelectID;
							}
							else
								rendSquad = false;
						}
					}
				}
			}
		}

		else {
			for (int i = (int)BCODE_INSERT; i < (int)BCODE_END; ++i) {
				if (ptInRect(mButton[(BUTTON_CODE)i].box, g_ptMouse)) {
					mButton[(BUTTON_CODE)i].ClickAction(this);
					break;
				}
			}

			for (size_t i = 0; i < squadButton.size(); ++i) {
				if (ptInRect(squadButton[i], g_ptMouse)) {
					Spawn_Squad = i + 1;
					break;
				}
			}
		}
	}

	if (rendSquad == false)
	{
		//	��Ŭ���� �ൿ
		if (KEYMANAGER->isKeyUp(VK_RBUTTON))
		{
			if (Focus_Squad != -1)
			{
				int id;
				MAP->pManager->Search_SelectPanel();
				if ((id = MAP->pManager->getSelectPanelId()) != -1)
				{
					auto Leader = PLAYER->getPlayerSquad(Focus_Squad)->squadLeader;

					bool onSpawnSite = false;		//��ȯ�������� �δ밡 �����ϴ°�?

					for (auto& it : mInstSquad.find(ALIANCE_GRIFFON)->second)
					{
						//Ŭ���� ��忡 �Ʊ��д밡 �ִٸ�...
						if (PLAYER->getPlayerSquad(it)->nowNodeID == id)
						{
							Focus_Squad = it;	//�ش� �д뿡 ��Ŀ���� �Ѱ��ش�
							onSpawnSite = true;
							break;
						}
					}

					if (!onSpawnSite)
					{
						//	������������쿡�� �����ϼ��ִ�.
						if (Leader->getMotion()->isCurrent("wait"))
						{
							//	�ڽ��� ���� ���ִ� ��尡 �ƴϴ�.
							if (PLAYER->getPlayerSquad(Focus_Squad)->nowNodeID != id)
							{
								//	���� ���ִ� ��尡, ���õ� ���� ������ �Ǿ��ִ°�?
								if (MAP->pManager->findPanel(PLAYER->getPlayerSquad(Focus_Squad)->nowNodeID)->FindLinkedPanel(id))
								{
									Leader->MovePoint(MAP->pManager->findPanel(id)->getPanelPos().x, MAP->pManager->findPanel(id)->getPanelPos().y, true);
									PLAYER->getPlayerSquad(Focus_Squad)->nowNodeID = id;
								}

								//	������ ��� ��带 ���ý�, ��Ŀ���� �����Ѵ�.
								else
									Focus_Squad = -1;
							}
						}
					}
				}
			}
		}
	}
}

bool worldMapScene::Find_SquadInWorld(TATICDOLL_ALIANCE_TYPE ac, UINT id)
{
	if (mInstSquad.count(ac))
	{
		for (auto& it : mInstSquad.find(ac)->second)
		{
			if (it == id)
				return true;
		}
	}
	return false;
}

void worldMapScene::render()
{
	//	�� �̹��� ������
	MAP->render();

	for (auto& aliance : mInstSquad) 
	{
		for (auto& squadID : aliance.second) {
			switch (aliance.first)
			{
			case ALIANCE_GRIFFON:
			{
				int nowNode = PLAYER->getPlayerSquad(squadID)->nowNodeID;
				auto panel = MAP->pManager->findPanel(nowNode);
				auto Leader = PLAYER->getPlayerSquad(squadID)->squadLeader;

				Leader->render_Motion();
			}
			break;

			case ALIANCE_IRONBLOD:
			case ALIANCE_PEREDEUS:
			case ALIANCE_SCCOM:
			case ALIANCE_NONE:
			{
				auto Leader = BDATA->getSquadSNV()->callSquad(squadID)->squadLeader;
				Leader->render_Motion();
			}
			break;
			}
		}
	}

	//Helicopter::RenderHelicopter();

	if (isInsertSquad == false)
		render_NormalWorld();
	else
		render_SelectWorld();
}

void worldMapScene::render_NormalWorld()
{
	for (int i = (int)BCODE_TURN; i < (int)BCODE_INSERT; ++i)
		D2D->renderRect(mButton[(BUTTON_CODE)i].box.left, mButton[(BUTTON_CODE)i].box.top, mButton[(BUTTON_CODE)i].box.right - mButton[(BUTTON_CODE)i].box.left,
			mButton[(BUTTON_CODE)i].box.bottom - mButton[(BUTTON_CODE)i].box.top, ColorF(1, 0, 0));
}

void worldMapScene::render_SelectWorld()
{
	guardPos = VEC2(CameraPositionX + (WINSIZEX * 0.5f), (WINSIZEY * 0.5f) - CameraPositionY);
	DRAW->render("bkGuard", VEC2(WINSIZEX, WINSIZEY), guardPos, COLR(1.0f, 1.0f, 1.0f, 0.5f));

	for (auto& iter : PLAYER->getPlayerSquad(Spawn_Squad)->squadMember) {
		DRAW->render(iter.second->keys.cardNormalKey,
			VEC2(140.0f, 260.0f), VEC2(guardPos.x + ((iter.second->getID()->SquadMem_ID - 3) * 150), guardPos.y));
	}

	for (size_t i = 0; i < squadButton.size(); ++i) {

		D2D->renderRect(squadButton[i], ColorF(1.0, 0, 0));

		DRAW->render(Spawn_Squad == i + 1 ? "SquadBar_s" : "SquadBar", VEC2(150, 75),
			VEC2(squadButton[i].left + 75.0f + CameraPositionX, squadButton[i].top + 37.5f - CameraPositionY));

		DWRITE->ChangeText("SQUAD", "%d ����", (i + 1));
		DWRITE->TextRender("SQUAD", squadButton[i].left, squadButton[i].top + 25, 150, 75,
			ColorF(0, 0, 0), DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	}

	for (int i = (int)BCODE_INSERT; i <= (int)BCODE_THERMAL_SQUAD; ++i) {
		D2D->renderRect(mButton[(BUTTON_CODE)i].box.left, mButton[(BUTTON_CODE)i].box.top, mButton[(BUTTON_CODE)i].box.right - mButton[(BUTTON_CODE)i].box.left,
			mButton[(BUTTON_CODE)i].box.bottom - mButton[(BUTTON_CODE)i].box.top, ColorF(1, 0, 0));
	}
}

void worldMapScene::TurnChange_Select(void * obj)
{
	objectPtr = static_cast<worldMapScene*>(obj);

	//	GRF�� ���� �ֺ� �г� Ž��
	MAP->pManager->Check_AroundPanel(ALIANCE_GRIFFON);

	//	Ž�� ������, �ൿ ����Ʈ ����� �ִ� �ൿ��ġ�� ����
	MAP->Check_ActionPoint();
	MAP->plsMaxActionPoint(objectPtr->mInstSquad[ALIANCE_GRIFFON].size());
	MAP->setActionPoint(MAP->getMaxActionPoint());

	//	������ �г� ID�� -1�� �ʱ�ȭ
	MAP->pManager->resetSelectPID();

	for (auto& squadID : objectPtr->mInstSquad[ALIANCE_GRIFFON])
	{
		SquadData* squad = PLAYER->getPlayerSquad(squadID);

		if (squad->nowNodeID != -1) {

			if (MAP->pManager->findPanel(squad->nowNodeID)->getPanelAlience() != ALIANCE_GRIFFON)
				MAP->pManager->findPanel(squad->nowNodeID)->setPanelAlience(ALIANCE_GRIFFON);
		}

	}

	//	IRB�� ���� �ֺ� �г� Ž��
	MAP->pManager->Check_AroundPanel(ALIANCE_IRONBLOD);
}

void worldMapScene::SquadBatch_Select(void * obj)
{
	//	@ HQ�Ǵ� PORT�� Ȱ��ȭ�� ���� ��ġ
	objectPtr = static_cast<worldMapScene*>(obj);

	if (objectPtr->rendSquad) {
		if (objectPtr->Spawn_Squad != 0 && objectPtr->Spawn_PanelID != -1) {
			if (MAP->pManager->findPanel(objectPtr->Spawn_PanelID)->getPanelEnum() != PANEL_CLASS_NONE &&
				MAP->pManager->findPanel(objectPtr->Spawn_PanelID)->getPanelAlience() == ALIANCE_GRIFFON)
			{
				objectPtr->isInsertSquad = true;
				objectPtr->isThermalSupport = false;
			}
		}
	}
}

void worldMapScene::GiveUpGame_Select(void * obj)
{
	//	@ �������� ��ư Action Function (worldMapScene memroy Adress - void*)
	
	objectPtr = static_cast<worldMapScene*>(obj);

	objectPtr->worldmapInit = false;
	objectPtr->squadInit = false;

	//Helicopter::release();

	LOAD->insertUnLoadList(objectPtr->vLoadList);
	LOAD->setAutoInit(true);
	LOAD->setNextScene("CHAPTER");
	LOAD->setLoadImageKey("ShootRange");
	SCENE->changeScene("UNLOAD");
}

void worldMapScene::InsertSquad_Select(void * obj)
{
	//	@ �δ� ��ġ��ư
	objectPtr = static_cast<worldMapScene*>(obj);

	//	�̹� ��ġ�� �д밡 �ƴ϶�� ���� ��ȯ�Ѵ�.
	if (objectPtr->Find_SquadInWorld(ALIANCE_GRIFFON, objectPtr->Spawn_Squad) == false &&
		PLAYER->getPlayerSquad(objectPtr->Spawn_Squad)->squadMember.size() != 0) {

		objectPtr->mInstSquad.find(ALIANCE_GRIFFON)->second.push_back(objectPtr->Spawn_Squad);

		PLAYER->getPlayerSquad(objectPtr->Spawn_Squad)->nowNodeID = objectPtr->Spawn_PanelID;

		BaseTaticDoll* squadLeader = PLAYER->getPlayerSquad(objectPtr->mInstSquad.find(ALIANCE_GRIFFON)->second.back())->squadLeader;
		Panel* curPanel = MAP->pManager->findPanel(objectPtr->Spawn_PanelID);

		squadLeader->p_getCharacterPos()->x = curPanel->getPanelPos().x;
		squadLeader->p_getCharacterPos()->y = curPanel->getPanelPos().y;

		SOUND->Play_Effect(SOUND_CHANNEL::CH_VOICE, squadLeader->keys.SOUND_GOATTACK, 0.15f);

		//Helicopter::CallHelicopter(curPanel->getPanelPos(), objectPtr->isThermalSupport);

		objectPtr->rendSquad = false;
		objectPtr->isInsertSquad = false;
	}
}

void worldMapScene::BackToNormal_Select(void * obj)
{
	objectPtr = static_cast<worldMapScene*>(obj);
	objectPtr->isInsertSquad = false;
}

void worldMapScene::NormalSquad_Select(void * obj)
{
	objectPtr = static_cast<worldMapScene*>(obj);
	objectPtr->isThermalSupport = false;
}

void worldMapScene::ThermalSquad_Select(void * obj)
{
	objectPtr = static_cast<worldMapScene*>(obj);
	objectPtr->isThermalSupport = true;
}
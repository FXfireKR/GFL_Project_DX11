#include "stdafx.h"
#include "worldMapScene.h"

worldMapScene* worldMapScene::objectPtr = NULL;

worldMapScene::worldMapScene()
{
	worldmapInit = false;
	squadInit = false;
	isInsertSquad = false;
	isThermalSupport = false;

	mButton.insert(make_pair(BCODE_TURN, Button(1195, 670, 150, 80, TurnChange_Select)));					//	턴 넘김
	mButton.insert(make_pair(BCODE_BATCH, Button(1195, 540, 150, 60, SquadBatch_Select)));					//	배치가능한 발판을 눌렀을때 활성화
	mButton.insert(make_pair(BCODE_GIVEUP, Button(100, 10, 150, 80, GiveUpGame_Select)));					//	작전 포기

	mButton.insert(make_pair(BCODE_INSERT, Button(1000, 670, 150, 80, InsertSquad_Select)));				//	해당 유닛 배치
	mButton.insert(make_pair(BCODE_BACK, Button(10, 20, 80, 40, BackToNormal_Select)));						//	뒤로가기
	mButton.insert(make_pair(BCODE_NORMAL_SQUAD, Button(1000, 400, 150, 80, NormalSquad_Select)));			//	일반 보병소대 선택
	mButton.insert(make_pair(BCODE_THERMAL_SQUAD, Button(1000, 500, 150, 80, ThermalSquad_Select)));		//	화력지원 소대 선택

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

		//	@ 임시로 적 분대 생성 해서, 전투하기
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
					//	전멸이라면...
					if (PLAYER->getPlayerSquad(squadID)->AllDead)
						aliance.second.erase(aliance.second.begin() + i);

					//	그렇지 않다면
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

	//	분대별 분대장 업데이트
	for (auto& it : mInstSquad.find(ALIANCE_GRIFFON)->second)
	{
		//	적군과 조우했는지 여부를 체크
		for (auto& enemyID : mInstSquad.find(ALIANCE_PEREDEUS)->second)
		{
			if (BDATA->getSquadSNV()->callSquad(enemyID)->AllDead) continue;

			if (BDATA->getSquadSNV()->callSquad(enemyID)->nowNodeID == PLAYER->getPlayerSquad(it)->nowNodeID)
			{
				MAP->pGetMissionFlag() = MAP->pManager->findPanel(PLAYER->getPlayerSquad(it)->nowNodeID)->getPlagData();

				BDATA->insertObject_AtSquad(enemyID);

				//	해당 분대 ID로 전투 설정
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

	//	좌클릭시 행동
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

							//	클릭한 노드에 아군분대가 있다면...
							if (PLAYER->getPlayerSquad(it)->nowNodeID == static_cast<UINT>(curSelectID))
							{
								Focus_Squad = it;	//	해당 분대에 포커싱을 넘겨준다
								onSpawnSite = true;
								break;
							}
						}

						//	클릭된 패널위에 아무런 분대도 없을경우
						if (!onSpawnSite) {
							//	클릭된 패널이 아군소속 지휘부 / 헬리포트일경우
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
		//	우클릭시 행동
		if (KEYMANAGER->isKeyUp(VK_RBUTTON))
		{
			if (Focus_Squad != -1)
			{
				int id;
				MAP->pManager->Search_SelectPanel();
				if ((id = MAP->pManager->getSelectPanelId()) != -1)
				{
					auto Leader = PLAYER->getPlayerSquad(Focus_Squad)->squadLeader;

					bool onSpawnSite = false;		//소환발판위에 부대가 존재하는가?

					for (auto& it : mInstSquad.find(ALIANCE_GRIFFON)->second)
					{
						//클릭한 노드에 아군분대가 있다면...
						if (PLAYER->getPlayerSquad(it)->nowNodeID == id)
						{
							Focus_Squad = it;	//해당 분대에 포커싱을 넘겨준다
							onSpawnSite = true;
							break;
						}
					}

					if (!onSpawnSite)
					{
						//	정지해있을경우에만 움직일수있다.
						if (Leader->getMotion()->isCurrent("wait"))
						{
							//	자신이 현재 서있는 노드가 아니다.
							if (PLAYER->getPlayerSquad(Focus_Squad)->nowNodeID != id)
							{
								//	현재 서있는 노드가, 선택된 노드와 연결이 되어있는가?
								if (MAP->pManager->findPanel(PLAYER->getPlayerSquad(Focus_Squad)->nowNodeID)->FindLinkedPanel(id))
								{
									Leader->MovePoint(MAP->pManager->findPanel(id)->getPanelPos().x, MAP->pManager->findPanel(id)->getPanelPos().y, true);
									PLAYER->getPlayerSquad(Focus_Squad)->nowNodeID = id;
								}

								//	범위를 벗어난 노드를 선택시, 포커싱을 해제한다.
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
	//	맵 이미지 랜더링
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

		DWRITE->ChangeText("SQUAD", "%d 제대", (i + 1));
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

	//	GRF에 대한 주변 패널 탐색
	MAP->pManager->Check_AroundPanel(ALIANCE_GRIFFON);

	//	탐색 종료후, 행동 포인트 계산후 최대 행동수치를 개선
	MAP->Check_ActionPoint();
	MAP->plsMaxActionPoint(objectPtr->mInstSquad[ALIANCE_GRIFFON].size());
	MAP->setActionPoint(MAP->getMaxActionPoint());

	//	선택한 패널 ID를 -1로 초기화
	MAP->pManager->resetSelectPID();

	for (auto& squadID : objectPtr->mInstSquad[ALIANCE_GRIFFON])
	{
		SquadData* squad = PLAYER->getPlayerSquad(squadID);

		if (squad->nowNodeID != -1) {

			if (MAP->pManager->findPanel(squad->nowNodeID)->getPanelAlience() != ALIANCE_GRIFFON)
				MAP->pManager->findPanel(squad->nowNodeID)->setPanelAlience(ALIANCE_GRIFFON);
		}

	}

	//	IRB에 대한 주변 패널 탐색
	MAP->pManager->Check_AroundPanel(ALIANCE_IRONBLOD);
}

void worldMapScene::SquadBatch_Select(void * obj)
{
	//	@ HQ또는 PORT에 활성화된 유닛 배치
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
	//	@ 작전포기 버튼 Action Function (worldMapScene memroy Adress - void*)
	
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
	//	@ 부대 배치버튼
	objectPtr = static_cast<worldMapScene*>(obj);

	//	이미 배치된 분대가 아니라면 노드로 소환한다.
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
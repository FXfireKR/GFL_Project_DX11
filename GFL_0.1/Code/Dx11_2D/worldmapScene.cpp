#include "stdafx.h"
#include "worldmapScene.h"

worldmapScene::worldmapScene()
{
	worldmapInit = false;
	squadInit = false;
}

worldmapScene::~worldmapScene()
{
}

void worldmapScene::init()
{
	if (!worldmapInit)
	{
		worldColor.a = 1.0f;
		Focus_Squad = -1;
		rendSquad = false;

		Turn = 0;
		turnAliance = ALIANCE_GRIFFON;

		LOADMANAGER->Add_LoadTray("Panel_Normal", "../../_Assets/Texture2D/Panel.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
		LOADMANAGER->Add_LoadTray("Panel_Heli", "../../_Assets/Texture2D/Heliport.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
		LOADMANAGER->Add_LoadTray("Panel_Hq", "../../_Assets/Texture2D/HQ.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
		LOADMANAGER->Add_LoadTray("WAY", "../../_Assets/Texture2D/waypoint.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

		LOADMANAGER->setAutoInit(false);
		LOADMANAGER->setNextScene("WORLD");
		SCENE->Change_Scene("LOAD");

		CAMERA->CameraReset();

		for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())		//	get List of Player's TacticalDoll
		{
			it.second->LoadTray_SoundList();
			it.second->LoadTray_ImageList();
			PLAYER->insertTacDolToSquad(it.first, 1);
		}

		mInstSquad.clear();
		for (int i = 0; i < 5; ++i)
		{
			vector<UINT> vint;
			mInstSquad.insert(make_pair((TATICDOLL_ALIANCE_TYPE)(i + 1), vint));
		}

		//	임시로 적 분대 생성 해서, 전투하기
		BDATA->getSquadSNV()->Insert_SquadMember(1, new Strelet);
		BDATA->getSquadSNV()->Call_Squad(1)->squadLeader = BDATA->getSquadSNV()->Call_SquadMember(1, BDATA->getSquadSNV()->Call_Squad(1)->squadLeaderID);
		BDATA->getSquadSNV()->Call_Squad(1)->nowNodeID = 11;
		mInstSquad[ALIANCE_PEREDEUS].push_back(1);

		BDATA->getSquadSNV()->Insert_SquadMember(2, new Strelet);
		BDATA->getSquadSNV()->Call_Squad(2)->squadLeader = BDATA->getSquadSNV()->Call_SquadMember(2, BDATA->getSquadSNV()->Call_Squad(2)->squadLeaderID);
		BDATA->getSquadSNV()->Call_Squad(2)->nowNodeID = 5;
		mInstSquad[ALIANCE_PEREDEUS].push_back(2);

		for (auto& it : mInstSquad.find(ALIANCE_PEREDEUS)->second)
		{
			TaticDoll* leader = BDATA->getSquadSNV()->Call_Squad(it)->squadLeader;
			tagSquad<TaticDoll*>* squad = BDATA->getSquadSNV()->Call_Squad(it);

			leader->p_getCharacterPos()->x = MAP->pManager->findPanel(squad->nowNodeID)->getPanelPos().x;
			leader->p_getCharacterPos()->y = MAP->pManager->findPanel(squad->nowNodeID)->getPanelPos().y;
		}

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
							TaticDoll* leader = PLAYER->getPlayerSquad(squadID)->squadLeader;
							POINT panelPos = MAP->pManager->findPanel(panelID)->getPanelPos();

							leader->init();

							leader->p_getCharacterPos()->x = static_cast<float>(panelPos.x);
							leader->p_getCharacterPos()->y = static_cast<float>(panelPos.y);						
						}
					}
				}
				break;

				case ALIANCE_IRONBLOD:
				case ALIANCE_PEREDEUS:
				case ALIANCE_SCCOM:
				case ALIANCE_NONE:
				{
					tagSquad<TaticDoll*>* squad = BDATA->getSquadSNV()->Call_Squad(squadID);

					if (squad->AllDead)
						aliance.second.erase(aliance.second.begin() + i);

					else
					{
						TaticDoll* leader = BDATA->getSquadSNV()->Call_Squad(squadID)->squadLeader;						
						POINT panelPos = MAP->pManager->findPanel(squad->nowNodeID)->getPanelPos();

						leader->p_getCharacterPos()->x = static_cast<float>(panelPos.x);
						leader->p_getCharacterPos()->y = static_cast<float>(panelPos.y);
					}
				}
				break;
				}
			}
		}

		squadInit = true;
	}
}

void worldmapScene::release()
{
}

void worldmapScene::update()
{
	CAMERA->setCameraFix(false);
	MAP->update();

	keyUpate();
		
	//	분대별 분대장 업데이트
	for (auto& it : mInstSquad.find(ALIANCE_GRIFFON)->second)
	{
		//	적군과 조우했는지 여부를 체크
		for (auto& enemyID : mInstSquad.find(ALIANCE_PEREDEUS)->second)
		{
			if (BDATA->getSquadSNV()->Call_Squad(enemyID)->AllDead) continue;

			if (BDATA->getSquadSNV()->Call_Squad(enemyID)->nowNodeID == PLAYER->getPlayerSquad(it)->nowNodeID)
			{
				MAP->pGetMissionFlag() = MAP->pManager->findPanel(PLAYER->getPlayerSquad(it)->nowNodeID)->getPlagData();

				BDATA->insertObject_AtSquad(enemyID);
				BDATA->getEngageSquadID() = enemyID;

				squadInit = false;

				SCENE->Change_Scene("BATTLE");
				SCENE->Init_Scene();
			}
		}
		TaticDoll* leader = PLAYER->getPlayerSquad(it)->squadLeader;
		leader->getMotion()->update(DELTA);
		leader->MoveClickPoint();
		leader->Update_DrawPos();

		/*if (leader->getMotion()->isCurrent("wait") && !(PLAYER->getPlayerSquad(it)->nextNodeID > 0))
			PLAYER->getPlayerSquad(it)->nowNodeID = PLAYER->getPlayerSquad(it)->nextNodeID;*/
	}

	for (auto& it : mInstSquad.find(ALIANCE_PEREDEUS)->second)
	{
		TaticDoll* leader = BDATA->getSquadSNV()->Call_Squad(it)->squadLeader;

		leader->getMotion()->update(DELTA);
		leader->Update_DrawPos();
	}
}

void worldmapScene::keyUpate()
{
	if (!rendSquad)
	{
		//	좌클릭시 행동
		if (KEYMANAGER->isKeyUp(VK_LBUTTON))
		{
			int id;
			MAP->pManager->Search_SelectPanel();
			if ((id = MAP->pManager->getSelectPanelId()) != -1)
			{
				if (Focus_Squad == -1)
				{
					bool onSpawnSite = false;

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

					//클릭된 패널위에 아무런 분대도 없을경우
					if (!onSpawnSite)
					{
						//클릭된 패널이 아군소속 지휘부 / 헬리포트일경우
						if (MAP->pManager->findPanel(id)->getPanelEnum() != PANEL_CLASS_NONE &&
							MAP->pManager->findPanel(id)->getPanelAlience() == ALIANCE_GRIFFON)
						{
							rendSquad = true;
							Spawn_Squad = 1;
							Spawn_PanelID = id;
						}
					}
				}
			}
		}

		//ImGui::DragInt("FocusID", &Focus_Squad);

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
									Leader->MovePoint(MAP->pManager->findPanel(id)->getPanelPos().x, MAP->pManager->findPanel(id)->getPanelPos().y);
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

		//	엔터시 행동 [ 임시 ]
		if (KEYMANAGER->isKeyUp(VK_RETURN))
		{ 
			MAP->pManager->Check_AroundPanel(ALIANCE_GRIFFON);

			MAP->Check_ActionPoint();
			MAP->plsMaxActionPoint(mInstSquad[ALIANCE_GRIFFON].size());
			MAP->setActionPoint(MAP->getMaxActionPoint());

			MAP->pManager->setSelectPID();

			for (auto& squadID : mInstSquad[ALIANCE_GRIFFON])
			{
				auto* squad = PLAYER->getPlayerSquad(squadID);

				if (squad->nowNodeID != -1)
				{
					if (MAP->pManager->findPanel(squad->nowNodeID)->getPanelAlience() != ALIANCE_GRIFFON)
						MAP->pManager->findPanel(squad->nowNodeID)->setPanelAlience(ALIANCE_GRIFFON);
				}

			}

			MAP->pManager->Check_AroundPanel(ALIANCE_IRONBLOD);
		}

	}

	else
	{
		//	스페이스바 [ 임시 ]
		if (KEYMANAGER->isKeyUp(VK_SPACE))
		{
			//	이미 배치된 분대가 아니라면 포트 노드로 배속시킨다.
			if (!Find_SquadInWorld(ALIANCE_GRIFFON, Spawn_Squad))
			{
				mInstSquad.find(ALIANCE_GRIFFON)->second.push_back(Spawn_Squad);

				PLAYER->getPlayerSquad(Spawn_Squad)->nowNodeID = Spawn_PanelID;

				PLAYER->getPlayerSquad(mInstSquad.find(ALIANCE_GRIFFON)->second.back())->squadLeader->p_getCharacterPos()->x =
					MAP->pManager->findPanel(Spawn_PanelID)->getPanelPos().x;

				PLAYER->getPlayerSquad(mInstSquad.find(ALIANCE_GRIFFON)->second.back())->squadLeader->p_getCharacterPos()->y =
					MAP->pManager->findPanel(Spawn_PanelID)->getPanelPos().y;

				rendSquad = false;
			}
		}
	}
}

bool worldmapScene::Find_SquadInWorld(TATICDOLL_ALIANCE_TYPE ac, UINT id)
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

void worldmapScene::render()
{
	MAP->render();

	for (auto& aliance : mInstSquad)
	{
		for (auto& squadID : aliance.second)
		{

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
				auto Leader = BDATA->getSquadSNV()->Call_Squad(squadID)->squadLeader;
				Leader->render_Motion();
			}
				break;
			}
		}
	}

	//if (rendSquad)
	//{

	//}

}
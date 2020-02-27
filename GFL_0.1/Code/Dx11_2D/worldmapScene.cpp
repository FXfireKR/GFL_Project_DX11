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

		//	�ӽ÷� �� �д� ���� �ؼ�, �����ϱ�
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
					//	�����̶��...
					if (PLAYER->getPlayerSquad(squadID)->AllDead)
						aliance.second.erase(aliance.second.begin() + i);

					//	�׷��� �ʴٸ�
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
		
	//	�д뺰 �д��� ������Ʈ
	for (auto& it : mInstSquad.find(ALIANCE_GRIFFON)->second)
	{
		//	������ �����ߴ��� ���θ� üũ
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
		//	��Ŭ���� �ൿ
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
						//Ŭ���� ��忡 �Ʊ��д밡 �ִٸ�...
						if (PLAYER->getPlayerSquad(it)->nowNodeID == id)
						{
							Focus_Squad = it;	//�ش� �д뿡 ��Ŀ���� �Ѱ��ش�
							onSpawnSite = true;
							break;
						}
					}

					//Ŭ���� �г����� �ƹ��� �д뵵 �������
					if (!onSpawnSite)
					{
						//Ŭ���� �г��� �Ʊ��Ҽ� ���ֺ� / �︮��Ʈ�ϰ��
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
									Leader->MovePoint(MAP->pManager->findPanel(id)->getPanelPos().x, MAP->pManager->findPanel(id)->getPanelPos().y);
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

		//	���ͽ� �ൿ [ �ӽ� ]
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
		//	�����̽��� [ �ӽ� ]
		if (KEYMANAGER->isKeyUp(VK_SPACE))
		{
			//	�̹� ��ġ�� �д밡 �ƴ϶�� ��Ʈ ���� ��ӽ�Ų��.
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
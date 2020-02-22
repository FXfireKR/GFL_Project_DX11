#include "stdafx.h"
#include "worldmapScene.h"

worldmapScene::worldmapScene()
{
}

worldmapScene::~worldmapScene()
{
}

void worldmapScene::init()
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
	for (int i = 0; i < 2; ++i)
	{
		vector<UINT> vint;
		mInstSquad.insert(make_pair((TATICDOLL_ALIANCE_TYPE)(i + 1), vint));
	}

	MAP->Check_ActionPoint();
	MAP->setActionPoint(MAP->getMaxActionPoint());
}

void worldmapScene::release()
{
}

void worldmapScene::update()
{
	CAMERA->setCameraFix(false);
	MAP->update();

	keyUpate();
	
	ImGui::Checkbox("renderSquad", &rendSquad);
		
	//	�д뺰 �д��� ������Ʈ
	for (auto& it : mInstSquad.find(ALIANCE_GRIFFON)->second)
	{
		PLAYER->getPlayerSquad(it)->squadLeader->getMotion()->update(DELTA);
		PLAYER->getPlayerSquad(it)->squadLeader->MoveClickPoint();
		PLAYER->getPlayerSquad(it)->squadLeader->Update_DrawPos();
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

		ImGui::DragInt("FocusID", &Focus_Squad);

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


		ImGui::Text("Max Act Point %d", MAP->getMaxActionPoint());
		ImGui::Text("Act Point %d", MAP->getActionPoint());

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

	for (auto& it : mInstSquad.find(ALIANCE_GRIFFON)->second)
	{
		int nowNode = PLAYER->getPlayerSquad(it)->nowNodeID;
		auto panel = MAP->pManager->findPanel(nowNode);
		auto Leader = PLAYER->getPlayerSquad(it)->squadLeader;

		//ImGui::DragFloat("Leader_X", &Leader->p_getCharacterPos()->x);
		//ImGui::DragFloat("Leader_Y", &Leader->p_getCharacterPos()->y);

		Leader->render_Motion();
	}

	//if (rendSquad)
	//{

	//}

}
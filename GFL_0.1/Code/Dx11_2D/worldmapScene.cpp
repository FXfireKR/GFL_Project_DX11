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
}

void worldmapScene::release()
{
}

void worldmapScene::update()
{
	CAMERA->setCameraFix(false);
	MAP->update();
	
	ImGui::Checkbox("renderSquad", &rendSquad);

	if (!rendSquad)
	{
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
						//클릭된 패널이 아군소속 지휘부 / 헬리포트일경우  [ 임시 전부 허용 ]
						if (MAP->pManager->findPanel(id)->getPanelEnum() != PANEL_CLASS_NONE && 
								MAP->pManager->findPanel(id)->getPanelAlience() == ALIANCE_GRIFFON)
						{
							rendSquad = true;
							Spawn_Squad = 1;
							Spawn_PanelID = id;
							//Focus_Squad = 
						}
					}
				}
			}
		}

		ImGui::DragInt("FocusID", &Focus_Squad);

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
		
	}
	else
	{
		//	분대 배치 확정
		if (KEYMANAGER->isKeyUp(VK_SPACE))
		{
			//이미 배치된 분대가 아니라면 포트 노드로 배속시킨다.
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

	//분대별 분대장 업데이트
	for (auto& it : mInstSquad.find(ALIANCE_GRIFFON)->second)
	{
		PLAYER->getPlayerSquad(it)->squadLeader->getMotion()->update(DELTA);
		PLAYER->getPlayerSquad(it)->squadLeader->MoveClickPoint();
		PLAYER->getPlayerSquad(it)->squadLeader->Update_DrawPos();
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

		ImGui::DragFloat("Leader_X", &Leader->p_getCharacterPos()->x);
		ImGui::DragFloat("Leader_Y", &Leader->p_getCharacterPos()->y);

		Leader->render_Motion();

		//Leader->getMotion().render(Leader->getDrawPos().x, Leader->getDrawPos().y, 0.5F, 1.0F, DgreeAngle(Leader->getViewAngle()));
	}

	//if (rendSquad)
	//{

	//}

}
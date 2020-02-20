#pragma once
#include "Scene.h"

class worldmapScene : public Scene
{
private:
	bool rendSquad;

	int Focus_Squad;		//현재 선택된 분대 (없을경우 -1)
	int Spawn_Squad;		//배치할 분대
	int Spawn_PanelID;		//배치할 패널위치

	map<TATICDOLL_ALIANCE_TYPE, vector<UINT>> mInstSquad;

public:
	worldmapScene();
	~worldmapScene();

	// Scene을(를) 통해 상속됨
	virtual void init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

	bool Find_SquadInWorld(TATICDOLL_ALIANCE_TYPE ac, UINT id);
};
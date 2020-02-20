#pragma once
#include "Scene.h"

class worldmapScene : public Scene
{
private:
	bool rendSquad;

	int Focus_Squad;		//���� ���õ� �д� (������� -1)
	int Spawn_Squad;		//��ġ�� �д�
	int Spawn_PanelID;		//��ġ�� �г���ġ

	map<TATICDOLL_ALIANCE_TYPE, vector<UINT>> mInstSquad;

public:
	worldmapScene();
	~worldmapScene();

	// Scene��(��) ���� ��ӵ�
	virtual void init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

	bool Find_SquadInWorld(TATICDOLL_ALIANCE_TYPE ac, UINT id);
};
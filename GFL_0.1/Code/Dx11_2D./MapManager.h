#pragma once
#include "PanelManager.h"
#include "BTM_Loader.h"
#include "BattleMap.h"
#include "singleton.h"

class MapManager : public singleton<MapManager>
{
public:
	BattleMap* battleMap;
	PanelManager* pManager;
	UINT MaxActionPoint;
	UINT ActionPoint;

public:
	MapManager();
	~MapManager();

	HRESULT init();
	void release();
	void update();
	void render();

public:
	void CreatePanel_();
	void Check_ActionPoint();
	void Load_MapFile(string path);

public:
	inline void decreaseActPoint(int dcp = 1) { ActionPoint -= dcp; }
	inline int getActionPoint() { return ActionPoint; }
	inline void setActionPoint(int a) { ActionPoint = a; }

	inline int getMaxActionPoint() { return MaxActionPoint; }
	inline void plsMaxActionPoint(int pls) { MaxActionPoint += pls; }

	inline BattleMap* getBattleMap() { return battleMap; }
};
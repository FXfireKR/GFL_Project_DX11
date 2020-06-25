#pragma once
#include "PanelManager.h"
#include "BTM_Loader.h"
#include "BattleMap.h"
#include "singleton.hpp"

class MapManager : public singleton<MapManager>
{
public:
	MapManager();
	~MapManager();

	HRESULT init();
	void update();
	void render();

public:
	void CreatePanel_();
	void Check_ActionPoint();
	void Load_MapFile(string path);

private:
	void release();

	string eraseEscape(string _str);
	int readInteger(string _str);
	size_t readUnsignedInteger(string _str);
	POINT readDoubleInteger(string _str);
	tagArea readArea(string _str);
	PANEL_CLASSIFY readPanelClass(string _str);
	TATICDOLL_ALIANCE_TYPE readAlianceClass(string _str);

public:
	inline void decreaseActPoint(int dcp = 1) { ActionPoint -= dcp; }
	inline int getActionPoint() { return ActionPoint; }
	inline void setActionPoint(int a) { ActionPoint = a; }

	inline int getMaxActionPoint() { return MaxActionPoint; }
	inline void plsMaxActionPoint(int pls) { MaxActionPoint += pls; }

	inline BattleMap* getBattleMap() { 
		return battleMap;
	}

	inline const BattlePlagData& getMissionFlag() { return missionFlag; }
	inline BattlePlagData& pGetMissionFlag() { return missionFlag; }


public:
	BattleMap* battleMap;
	PanelManager* pManager;
	UINT MaxActionPoint;
	UINT ActionPoint;
	string mapImageFileName;


	BattlePlagData missionFlag;				//	전투 승리 조건 플래그
};
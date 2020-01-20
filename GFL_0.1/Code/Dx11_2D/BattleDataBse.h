#pragma once

#include "IRB_DollType.h"
#include "SCM_DollType.h"
#include "PRD_DollType.h"

#include "SquadManager.h"
#include "PlayerData.h"

#include "singleton.h"

class BattleDataBse : public singleton<BattleDataBse>
{
private:
	//	현재 전투씬에 그려지고있는 녀석들
	vector<TaticDoll*>				vecBattleObjects;		


	//	적군의 분대를 관리한다.
	SquadManager<TaticDoll*>*		squadSNV;

	
	//	플레이어 전체관리
	PlayerData* player;

	SINT EngageEnemySquad;

public:
	BattleDataBse();
	~BattleDataBse();

	HRESULT init();
	void release();

public:
	//inline Squad* getEnemySquad(UINT id) { return mSNVsquad.find(id)->second; }
	//inline TaticDoll* getEnemyMember(UINT sq_id, UINT id) { return (TaticDoll*)getEnemySquad(sq_id)->getSquadMember(id); }
	//inline SINT& getEngageSquadID() { return EngageEnemySquad; }

	//inline tagSquad<TaticDoll*>* getEnemySquad(UINT squadID) { return squadSNV->Call_Squad(squadID); }
	//inline TaticDoll* getEnemyMember(UINT squadID, UINT memID) { return squadSNV->Call_SquadMember(squadID, memID); }
	//inline TaticDoll* getCur_EnemyMember(UINT memID) { return squadSNV->Call_SquadMember(EngageEnemySquad, memID); }

	inline SquadManager<TaticDoll*>* getSquadSNV() { return squadSNV; }

	inline vector<TaticDoll*>& getCurrUnits() { return vecBattleObjects; }

	inline PlayerData* getPlayerData() { return player; }

	inline TaticDoll* getObject(size_t _vecAt)
	{
		return (vecBattleObjects.size() - 1 < _vecAt) ? nullptr : vecBattleObjects[_vecAt];
	}
};
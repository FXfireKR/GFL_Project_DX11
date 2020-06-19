#pragma once

#include "IRB_Dolls.h"
#include "SCM_Dolls.h"
#include "PRD_Dolls.h"

#include "SquadManager.h"
#include "PlayerData.h"

#include "singleton.hpp"

class BattleDataBase : public singleton<BattleDataBase>
{
public:
	BattleDataBase();
	~BattleDataBase();

	HRESULT init();
	void release();

	void insertObject_AtSquad(SINT id);
	void insertObject(BaseTaticDoll* _pointer);
	void deleteObject(const BaseTaticDoll* _pointer);
	void deleteObject(size_t at);
	void deleteAll();

public:
	//inline Squad* getEnemySquad(UINT id) { return mSNVsquad.find(id)->second; }
	//inline TaticDoll* getEnemyMember(UINT sq_id, UINT id) { return (TaticDoll*)getEnemySquad(sq_id)->getSquadMember(id); }
	inline SINT& getEngageSquadID() { return EngageEnemySquad; }

	//inline tagSquad<TaticDoll*>* getEnemySquad(UINT squadID) { return squadSNV->Call_Squad(squadID); }
	//inline TaticDoll* getEnemyMember(UINT squadID, UINT memID) { return squadSNV->Call_SquadMember(squadID, memID); }
	//inline TaticDoll* getCur_EnemyMember(UINT memID) { return squadSNV->Call_SquadMember(EngageEnemySquad, memID); }

	inline SquadManager* getSquadSNV() { return squadSNV; }

	inline vector<BaseTaticDoll*>& getCurrUnits() { return vecBattleObjects; }

	inline PlayerData* getPlayerData() { return player; }

	inline const vector<BaseTaticDoll*>& getObjectVector() { return vecBattleObjects; }

	inline BaseTaticDoll* getObject(size_t _vecAt) {
		return (vecBattleObjects.size() - 1 < _vecAt) ? nullptr : vecBattleObjects[_vecAt];
	}

private:

	vector<BaseTaticDoll*>				vecBattleObjects;		//	���� �������� �׷������ִ� �༮��
	SquadManager*						squadSNV;				//	������ �д븦 �����Ѵ�.
	PlayerData*							player;					//	�÷��̾� ��ü����

	SINT								EngageEnemySquad;		//	������ �� �д� ��
};
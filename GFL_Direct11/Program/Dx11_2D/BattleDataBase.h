#pragma once

#include "singleton.h"
#include "SNVtacDollType.h"
#include "SquadManager.h"

class BattleDataBase : public singleton<BattleDataBase>
{
private:
	FPNT vPos;			//가상카메라 좌표

	//적군의 분대를 관리한다.
	SquadManager<SNVtacDoll*>* squadSNV;

	UINT EngageEnemySquad;

public:
	BattleDataBase();
	~BattleDataBase();

	HRESULT init();
	void release();
	void VirtualPos_Limit(int y);		//가상좌표의 제한을 걸어준다. Y만ㅋㅋ (SCene의 업데이트 최 하단부)

public:
	inline FPNT getVirtualPos() { return vPos; }
	inline FPNT* p_getVirtualPos() { return &vPos; }
	//inline Squad* getEnemySquad(UINT id) { return mSNVsquad.find(id)->second; }
	//inline SNVtacDoll* getEnemyMember(UINT sq_id, UINT id) { return (SNVtacDoll*)getEnemySquad(sq_id)->getSquadMember(id); }
	inline UINT& getEngageSquadID() { return EngageEnemySquad; }

	inline tagSquad<SNVtacDoll*>* getEnemySquad(UINT squadID) { return squadSNV->Call_Squad(squadID); }
	inline SNVtacDoll* getEnemyMember(UINT squadID, UINT memID) { return squadSNV->Call_SquadMember(squadID, memID); }
	inline SNVtacDoll* getCur_EnemyMember(UINT memID) { return squadSNV->Call_SquadMember(EngageEnemySquad, memID); }

	inline SquadManager<SNVtacDoll*>* getSquadSNV() { return squadSNV; }
};
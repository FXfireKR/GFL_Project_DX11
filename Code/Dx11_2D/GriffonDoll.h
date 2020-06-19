#pragma once

/*
	�÷��̾��� �÷��̾�� ���������� ���������� �����մϴ�.

	�д������ ����/��ġ/�̵� ���� ����ϸ�, �޸𸮸� �Ҵ����ݴϴ�.

	���������� '�Ʊ� ���������� �޸��� ������ �Ҵ�' �� �̰����� �̷�����ϴ�.

	Player�� singleton�������� ���������� ������ �����ϸ�, �����ÿ��� BattleData���� �����Ͽ� ����ϵ����մϴ�.
*/

#include "GRF_Dolls.h"
#include "BaseTaticDoll.h"
#include "SquadManager.h"

class GriffonDoll
{
private:
	void release();

public:
	GriffonDoll();
	~GriffonDoll();

	HRESULT init();

	//	Battle T-Doll Function
	HRESULT createIOPtacDoll(GRFDOLL_NAME name);				//	�������� �����Ѵ�.
	HRESULT insertSquadTacDoll(SINT squadID, SINT allID);		//	���������� �ش� �д뿡 �߰��Ѵ�.
	HRESULT exitSquadTacDoll(SINT squadID, SINT memID);			//	���������� �ش� �д뿡�� �����Ѵ�
	HRESULT appointLeaderTacDoll(SINT squadID, SINT memID);		//	�д볻���� ���������� �д������� �Ӹ��Ѵ�.

	//Support T-Doll Function
	HRESULT createIOPtherTeam(GRFDOLL_NAME name);				//	ȭ�¼Ҵ븦 �����Ѵ�.

public:
	inline const size_t getTotalTacDollNum() { 
		return mAllDoll.size();
	}

	inline const size_t getSquadSize() {
		return static_cast<size_t>(IOPsquad->getTotalSquadSize());
	}

	inline SquadData* getSquad(SINT id) { 
		SquadData* returns = IOPsquad->callSquad(id);
		return returns != nullptr ? returns : nullptr;
	}

	inline BaseTaticDoll* getSquadMember(SINT squadID, SINT memID) {
		BaseTaticDoll* returns = IOPsquad->callSquadMember(squadID, memID);
		return returns != nullptr ? returns : nullptr;
	}

	inline const unordered_map<SINT, BaseTaticDoll*>& getAllDolls() {
		return mAllDoll;
	}

	inline const size_t getTotalThermalDolNum() {
		return mAllThermalTeam.size();
	}

	inline BaseThermalTeam* getSupportDoll(SINT id) {
		assert(mAllThermalTeam.count(id) != 0);
		return mAllThermalTeam.at(id);
	}

	inline unordered_map<SINT, BaseThermalTeam*>& getAllThermals() {
		return mAllThermalTeam;
	}

private:

	SquadManager* IOPsquad;

	unordered_map<SINT, BaseTaticDoll*>				mAllDoll;			//	��� �������� [ȹ������� �з���]
	unordered_map<SINT, BaseTaticDoll*>::iterator	dollIter;

	unordered_map<SINT, BaseThermalTeam*>			mAllThermalTeam;	//	��� ȭ���������� [ȹ������� �з���]
	unordered_map<SINT, BaseThermalTeam*>::iterator	thermIter;

};
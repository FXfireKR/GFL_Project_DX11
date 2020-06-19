#pragma once

/*
	플레이어의 플레이어블 전술인형을 종합적으로 관리합니다.

	분대생성및 관리/배치/이동 등을 담당하며, 메모리를 할당해줍니다.

	최종적으로 '아군 전술인형의 메모리의 생성과 할당' 은 이곳에서 이루어집니다.

	Player는 singleton패턴으로 전반적으로 접근이 가능하며, 전투시에는 BattleData에서 참조하여 사용하도록합니다.
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
	HRESULT createIOPtacDoll(GRFDOLL_NAME name);				//	전술인형 생산한다.
	HRESULT insertSquadTacDoll(SINT squadID, SINT allID);		//	전술인형을 해당 분대에 추가한다.
	HRESULT exitSquadTacDoll(SINT squadID, SINT memID);			//	전술인형을 해당 분대에서 퇴출한다
	HRESULT appointLeaderTacDoll(SINT squadID, SINT memID);		//	분대내부의 전술인형을 분대장으로 임명한다.

	//Support T-Doll Function
	HRESULT createIOPtherTeam(GRFDOLL_NAME name);				//	화력소대를 생산한다.

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

	unordered_map<SINT, BaseTaticDoll*>				mAllDoll;			//	모든 전술인형 [획득순서로 분류됨]
	unordered_map<SINT, BaseTaticDoll*>::iterator	dollIter;

	unordered_map<SINT, BaseThermalTeam*>			mAllThermalTeam;	//	모든 화력지원인형 [획득순서로 분류됨]
	unordered_map<SINT, BaseThermalTeam*>::iterator	thermIter;

};
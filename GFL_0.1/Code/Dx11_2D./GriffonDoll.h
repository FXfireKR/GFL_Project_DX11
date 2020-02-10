#pragma once

/*
	플레이어의 플레이어블 전술인형을 종합적으로 관리합니다.

	분대생성및 관리/배치/이동 등을 담당하며, 메모리를 할당해줍니다.

	최종적으로 '아군 전술인형의 메모리의 생성과 할당' 은 이곳에서 이루어집니다.

	Player는 singleton패턴으로 전반적으로 접근이 가능하며,
	전투시에는 BattleData에서 참조하여 사용하도록합니다.
*/


#include "GRF_DollType.h"
#include "TaticDoll.h"
#include "SquadManager.h"

class GriffonDoll
{
private:
	map<SINT, TaticDoll*> mAlltacDoll;		//모든 전술인형 [획득순서로 분류됨]

	SquadManager<TaticDoll*> IOPsquad;



	//map<IOP_SUPPORTDOLL_TYPE, IOPsuportDoll*> mAllsuportDoll;	//모든 화력 지원 소대

public:
	GriffonDoll();
	~GriffonDoll();

	HRESULT init();
	void release();

	//Battle TaticDoll
	HRESULT Create_IOPtacDoll(GRFDOLL_NAME name);				//전술인형을 생산한다

	HRESULT InsertSquad_IOPtacDoll(SINT squadId, SINT allid);	//전술인형을 분대에 넣는다.
	HRESULT ExitSquad_IOPtacDoll(SINT squadId, SINT memID);		//전술인형을 분대에서 퇴출한다
	HRESULT AppointLeader_IOPsquad(SINT sid, SINT mid);			//분대내의 전술인형을 분대장으로 임명한다.


	//Support TaticDoll
	//HRESULT Create_IOPsptDoll(IOP_SUPPORTDOLL_TYPE type);		//화력소대를 생산한다.



public:
	inline int getTotalTacDollNum() { return mAlltacDoll.size(); }

	inline tagSquad<TaticDoll*>* getSquad(SINT id) { return (IOPsquad.Call_Squad(id) != nullptr) ? IOPsquad.Call_Squad(id) : nullptr; }
	inline TaticDoll* getSquadMember(SINT sid, SINT mid) { return (IOPsquad.Call_SquadMember(sid, mid) != nullptr) ? IOPsquad.Call_SquadMember(sid, mid) : nullptr; }

	//inline IOPsuportDoll* getSupportDoll(IOP_SUPPORTDOLL_TYPE type) { return mAllsuportDoll.count(type) ? mAllsuportDoll.find(type)->second : nullptr; }

	inline const map<SINT, TaticDoll*>& getAllTacDoll() { return mAlltacDoll; }
};
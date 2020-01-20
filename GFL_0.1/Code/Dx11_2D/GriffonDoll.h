#pragma once

/*
	�÷��̾��� �÷��̾�� ���������� ���������� �����մϴ�.

	�д������ ����/��ġ/�̵� ���� ����ϸ�, �޸𸮸� �Ҵ����ݴϴ�.

	���������� '�Ʊ� ���������� �޸��� ������ �Ҵ�' �� �̰����� �̷�����ϴ�.

	Player�� singleton�������� ���������� ������ �����ϸ�,
	�����ÿ��� BattleData���� �����Ͽ� ����ϵ����մϴ�.
*/


#include "GRF_DollType.h"
#include "TaticDoll.h"
#include "SquadManager.h"

class GriffonDoll
{
private:
	map<SINT, TaticDoll*> mAlltacDoll;		//��� �������� [ȹ������� �з���]

	SquadManager<TaticDoll*> IOPsquad;



	//map<IOP_SUPPORTDOLL_TYPE, IOPsuportDoll*> mAllsuportDoll;	//��� ȭ�� ���� �Ҵ�

public:
	GriffonDoll();
	~GriffonDoll();

	HRESULT init();
	void release();

	//Battle TaticDoll
	HRESULT Create_IOPtacDoll(GRFDOLL_NAME name);				//���������� �����Ѵ�

	HRESULT InsertSquad_IOPtacDoll(SINT squadId, SINT allid);	//���������� �д뿡 �ִ´�.
	HRESULT ExitSquad_IOPtacDoll(SINT squadId, SINT memID);		//���������� �д뿡�� �����Ѵ�
	HRESULT AppointLeader_IOPsquad(SINT sid, SINT mid);			//�д볻�� ���������� �д������� �Ӹ��Ѵ�.


	//Support TaticDoll
	//HRESULT Create_IOPsptDoll(IOP_SUPPORTDOLL_TYPE type);		//ȭ�¼Ҵ븦 �����Ѵ�.



public:
	inline int getTotalTacDollNum() { return mAlltacDoll.size(); }

	inline tagSquad<TaticDoll*>* getSquad(SINT id) { return (IOPsquad.Call_Squad(id) != nullptr) ? IOPsquad.Call_Squad(id) : nullptr; }
	inline TaticDoll* getSquadMember(SINT sid, SINT mid) { return (IOPsquad.Call_SquadMember(sid, mid) != nullptr) ? IOPsquad.Call_SquadMember(sid, mid) : nullptr; }

	//inline IOPsuportDoll* getSupportDoll(IOP_SUPPORTDOLL_TYPE type) { return mAllsuportDoll.count(type) ? mAllsuportDoll.find(type)->second : nullptr; }

	inline const map<SINT, TaticDoll*>& getAllTacDoll() { return mAlltacDoll; }
};
#pragma once

/*
	�÷��̾��� �÷��̾�� ���������� ���������� �����մϴ�.

	�д������ ����/��ġ/�̵� ���� ����ϸ�, �޸𸮸� �Ҵ����ݴϴ�.

	���������� '�Ʊ� ���������� �޸��� ������ �Ҵ�' �� �̰����� �̷�����ϴ�.

	Player�� singleton�������� ���������� ������ �����ϸ�,
	�����ÿ��� BattleData���� �����Ͽ� ����ϵ����մϴ�.
*/


#include "IOPtacDollType.h"
#include "IOPtaticDoll.h"
#include "SquadManager.h"

class GriffonDoll
{
private:
	map<UINT, IOPtaticDoll*> mAlltacDoll;		//��� �������� [ȹ������� �з���]

	SquadManager<IOPtaticDoll*> IOPsquad;

	//map<IOP_SUPPORTDOLL_TYPE, IOPsuportDoll*> mAllsuportDoll;	//��� ȭ�� ���� �Ҵ�

public:
	GriffonDoll();
	~GriffonDoll();

	HRESULT init();
	void release();

	//Battle TaticDoll
	HRESULT Create_IOPtacDoll(IOPDOLL_NAME name);				//���������� �����Ѵ�

	HRESULT InsertSquad_IOPtacDoll(UINT squadId, UINT allid);	//���������� �д뿡 �ִ´�.
	HRESULT ExitSquad_IOPtacDoll(UINT squadId, UINT memID);		//���������� �д뿡�� �����Ѵ�
	HRESULT AppointLeader_IOPsquad(UINT sid, UINT mid);			//�д볻�� ���������� �д������� �Ӹ��Ѵ�.


	//Support TaticDoll
	//HRESULT Create_IOPsptDoll(IOP_SUPPORTDOLL_TYPE type);		//ȭ�¼Ҵ븦 �����Ѵ�.



public:
	inline int getTotalTacDollNum() { return mAlltacDoll.size(); }

	inline tagSquad<IOPtaticDoll*>* getSquad(UINT id) { return (IOPsquad.Call_Squad(id) != nullptr) ? IOPsquad.Call_Squad(id) : nullptr; }
	inline IOPtaticDoll* getSquadMember(UINT sid, UINT mid) { return (IOPsquad.Call_SquadMember(sid, mid) != nullptr) ? IOPsquad.Call_SquadMember(sid, mid) : nullptr; }

	//inline IOPsuportDoll* getSupportDoll(IOP_SUPPORTDOLL_TYPE type) { return mAllsuportDoll.count(type) ? mAllsuportDoll.find(type)->second : nullptr; }

	inline const map<UINT, IOPtaticDoll*>& getAllTacDoll() { return mAlltacDoll; }
};
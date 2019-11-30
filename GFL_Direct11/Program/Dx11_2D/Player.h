#pragma once

#include "singleton.h"
#include "GriffonDoll.h"

class Player : public singleton<Player>
{
private:
	GriffonDoll* tacDoll;

	UINT currentSquad;		//���� ������ ���ϰ��ִ� �д�
	UINT focusTacdolID;		//��Ŀ���� �������� �д��� ID
	UINT cFocusTacDollID;	//ī�޶� ��Ŀ���ϰ��ִ� �д��� ID	[ 0�ϰ�� �ƹ��͵� ���� ]

	//IOP_SUPPORTDOLL_TYPE curntSuportDoll;	//���� ������ �����ϰ��ִ� ȭ�¼Ҵ� �̸�


	POINT sPos, ePos;
	FPNT mPos, trgPos;
	bool multiSelectMod;

	vector<UINT> vMultiSelect;		//���� ������ �������
	map<int, vector<UINT>> mRemUnit;

	//map<UINT, EquipBase> mPlayerEquip;		//�÷��̾ �����ϰ��ִ� ������
	//map<UINT, EquipBase>::iterator EquipIter;
	int OwnEquipNumber;

	bool RemUnit_Toggle;

public:
	Player();
	~Player();

	HRESULT init();
	void release();
	void update();
	void render();


	void Change_FocusTacDoll();			//�ѹ��е带 ���� �����Ѵ�.
	void Change_FocusTacDoll_Func(int memID);
	void SaveRemUnit(int padID);
	void LoadRemUnit(int padID);

	void AutoSetting_CameraFocus();		//�������� ���ý�, �ڵ����� ī�޶� �̵���Ų��.

	bool Targetting_Other();			//���콺 ������ ��󿡰� Ÿ����

	void testFuc();
	void testinst();

public:
	//���� �������� �д��� �д���� �ҷ��ɴϴ�
	inline IOPtaticDoll* getIOPdoll_crntSquad(UINT squadID)
	{
		return (IOPtaticDoll*)(tacDoll->getSquadMember(currentSquad, squadID));
	}

	//���� �������� �д��� ��Ŀ�̵� �д���� �ҷ��ɴϴ�
	inline IOPtaticDoll* getIOPdoll_crntSquadFocus() { return focusTacdolID > 0 ? (IOPtaticDoll*)(tacDoll->getSquadMember(currentSquad, focusTacdolID)) : nullptr; }

	//������ �д븦 �ҷ��ɴϴ�.
	inline tagSquad<IOPtaticDoll*>* getPlayerSquad(UINT squadID) { return tacDoll->getSquad(squadID) != nullptr ? tacDoll->getSquad(squadID) : nullptr; }

	//��Ŀ�̵� ���������� �ҷ��ɴϴ�.
	//inline IOPsuportDoll* getIOPsptDoll_crntFocus() { return tacDoll->getSupportDoll(curntSuportDoll); }

	inline GriffonDoll& getPlayerTaticDoll() { return *tacDoll; }

	inline UINT getCameraFocusTacDollID() { return cFocusTacDollID; }
	inline void setCameraFocusTacDollID(UINT id) { cFocusTacDollID = id > 0 && id < tacDoll->getSquad(currentSquad)->mSquad.size() + 1 ? id : 0; }

	inline UINT& getCurrentSquad() { return currentSquad; }

	//inline map<UINT, EquipBase>& getPlayerEquip() { return mPlayerEquip; }
	//inline map<UINT, EquipBase>::iterator& getEquipIterator() { return EquipIter; }

	inline int getOwnEquipNumber() const { return OwnEquipNumber; }

};
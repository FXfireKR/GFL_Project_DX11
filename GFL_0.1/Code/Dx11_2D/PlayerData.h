#pragma once
#include "GriffonDoll.h"

class PlayerData
{
private:

	int							OwnEquipNumber;

	bool						multiSelectMod;
	bool						cameraFocus;			//	ī�޶� ��Ŀ���ΰ�?
	bool						RemUnit_Toggle;

	SINT						currentSquad;			//	���� ������ ���ϰ��ִ� �д�

	POINT						sPos, ePos;
	D3DXVECTOR2					mPos, trgPos;

		
	TaticDoll*					curFocusDol;			//	���� ��Ŀ���ϰ��ִ� ����
	GriffonDoll*				tacDoll;

	vector<UINT>				vMultiSelect;			//	���� ������ �������
	map<int, vector<UINT>>		mRemUnit;


	//IOP_SUPPORTDOLL_TYPE curntSuportDoll;	//���� ������ �����ϰ��ִ� ȭ�¼Ҵ� �̸�
	//map<UINT, EquipBase> mPlayerEquip;		//�÷��̾ �����ϰ��ִ� ������
	//map<UINT, EquipBase>::iterator EquipIter;

public:
	FLOAT						playerHighLight;
	FLOAT						maxPlayerHighLight;
	bool						PlayerSlow;

public:
	PlayerData();
	~PlayerData();

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

	void test_create();
	void test_setting();

public:
	//���� �������� �д��� �д���� �ҷ��ɴϴ�
	inline TaticDoll* getIOPdoll_crntSquad(UINT squadID) { return (TaticDoll*)(tacDoll->getSquadMember(currentSquad, squadID)); }

	//���� �������� �д��� ��Ŀ�̵� �д���� �ҷ��ɴϴ�
	inline TaticDoll* getIOPdoll_crntSquadFocus() { return curFocusDol; }

	//������ �д븦 �ҷ��ɴϴ�.
	inline tagSquad<TaticDoll*>* getPlayerSquad(UINT squadID) { return tacDoll->getSquad(squadID) != nullptr ? tacDoll->getSquad(squadID) : nullptr; }

	//��Ŀ�̵� ���������� �ҷ��ɴϴ�.
	//inline IOPsuportDoll* getIOPsptDoll_crntFocus() { return tacDoll->getSupportDoll(curntSuportDoll); }

	inline GriffonDoll& getPlayerTaticDoll() { return *tacDoll; }

	inline SINT& getCurrentSquad() { return currentSquad; }

	//inline map<UINT, EquipBase>& getPlayerEquip() { return mPlayerEquip; }
	//inline map<UINT, EquipBase>::iterator& getEquipIterator() { return EquipIter; }

	inline int getOwnEquipNumber() const { return OwnEquipNumber; }

};
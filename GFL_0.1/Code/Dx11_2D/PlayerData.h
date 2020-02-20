#pragma once
#include "GriffonDoll.h"

class PlayerData
{
private:
	struct EquipContainer
	{
		BYTE				num;			//	����
		EquipBase*			equip;			//	���
		EQUIPTYPE			equipType;		//	
		EQUIPTOTAL_TYPE		allEquipType;	//	

		EquipContainer(EquipBase* _equip, BYTE _num = 1) : equip(_equip) , num(_num)
		{
			allEquipType = equip->getItemType();
			equipType = equip->getEquipType();
		}
	};

private:

	int							OwnEquipNumber;			//	������ ��� ����

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

	vector<TATICDOLL_ALIANCE_TYPE> vAlianceList;		//	���� �������� �Ҽ� ���ֵ�

	//IOP_SUPPORTDOLL_TYPE curntSuportDoll;				//	���� ������ �����ϰ��ִ� ȭ�¼Ҵ� �̸�

	map<string, EquipContainer> mPlayerEquip;			//	�÷��̾ �����ϰ��ִ� ������
	map<string, EquipContainer>::iterator EquipIter;		//	���� ���� Iterator

public:
	FLOAT						playerHighLight;
	FLOAT						maxPlayerHighLight;
	bool						PlayerSlow;

public:
	PlayerData();
	~PlayerData();

	HRESULT init();
	void reset();
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

	bool isAliance(TATICDOLL_ALIANCE_TYPE _type);

	void insertTacDolToSquad(SINT _allId, SINT _squadId);
	void deleteTacDolToSquad(SINT _squadMemId, SINT _squadId);

public:
	inline void setAliance(TATICDOLL_ALIANCE_TYPE _type) { if (!isAliance(_type))vAlianceList.push_back(_type); }

	inline const vector<TATICDOLL_ALIANCE_TYPE>& getAlianceList() { return vAlianceList; }

	inline void clearAlianceList() { vAlianceList.clear(); }
	//inline void eraseAlianceList(TATICDOLL_ALIANCE_TYPE _type) {vAlianceList}

	//���� �������� �д��� �д���� �ҷ��ɴϴ�
	inline TaticDoll* getIOPdoll_crntSquad(UINT squadID) { return (TaticDoll*)(tacDoll->getSquadMember(currentSquad, squadID)); }

	//���� �������� �д��� ��Ŀ�̵� �д���� �ҷ��ɴϴ�
	inline TaticDoll* getIOPdoll_crntSquadFocus() { return curFocusDol; }

	inline const TaticDoll* getCurFocusDoll() { return curFocusDol; }
	inline void setCurFocusDoll(TaticDoll* _focus) { curFocusDol = _focus; if (_focus == nullptr) cameraFocus = false; }

	//������ �д븦 �ҷ��ɴϴ�.
	inline tagSquad<TaticDoll*>* getPlayerSquad(UINT squadID) { return tacDoll->getSquad(squadID) != nullptr ? tacDoll->getSquad(squadID) : nullptr; }

	//��Ŀ�̵� ���������� �ҷ��ɴϴ�.
	//inline IOPsuportDoll* getIOPsptDoll_crntFocus() { return tacDoll->getSupportDoll(curntSuportDoll); }

	inline GriffonDoll& getPlayerTaticDoll() { return *tacDoll; }

	inline SINT& getCurrentSquad() { return currentSquad; }

	inline const map<string, EquipContainer>& getPlayerEquip() { return mPlayerEquip; }
	inline map<string, EquipContainer>& pGetPlayerEquip() { return mPlayerEquip; }

	inline int getOwnEquipNumber() const { return OwnEquipNumber; }

};
#pragma once
#include "GriffonDoll.h"

struct EquipContainer
{
	BYTE				num;			//	����
	EquipBase*			equip;			//	���
	EQUIPTYPE			equipType;		//	Ÿ��
	EQUIPTOTAL_TYPE		allEquipType;	//	���� �з�

	EquipContainer(EquipBase* _equip, BYTE _num = 1) : equip(_equip), num(_num)
	{
		allEquipType = equip->getItemType();
		equipType = equip->getEquipType();
	}
};

class PlayerData
{
private:
	void release();

public:
	PlayerData();
	~PlayerData();

	void init();
	void reset();
	void update();
	void render();

	void changeFocusDoll();
	void changeFocusDoll(SINT memID);

	void saveRemUnit(int padID);
	void loadRemUnit(int padID);

	void autoSettingCameraFocus();

	bool targettingOther();

	bool isAliance(TATICDOLL_ALIANCE_TYPE _type);

	void insertDollToSquad(SINT allID, SINT squadID);
	void deleteDollToSquad(SINT squadID, SINT memID);

	void testCreate();
	void testSetting();

public:
	inline void setAliance(TATICDOLL_ALIANCE_TYPE _type) { if (!isAliance(_type))vAlianceList.push_back(_type); }

	inline const vector<TATICDOLL_ALIANCE_TYPE>& getAlianceList() { return vAlianceList; }

	inline void clearAlianceList() { vAlianceList.clear(); }
	//inline void eraseAlianceList(TATICDOLL_ALIANCE_TYPE _type) {vAlianceList}

	//���� �������� �д��� �д���� �ҷ��ɴϴ�
	inline BaseTaticDoll* getIOPdoll_crntSquad(UINT squadID) { return tacDoll->getSquadMember(currentSquad, squadID); }

	//���� �������� �д��� ��Ŀ�̵� �д���� �ҷ��ɴϴ�
	inline BaseTaticDoll* getIOPdoll_crntSquadFocus() { return curFocusDol; }

	inline const BaseTaticDoll* getCurFocusDoll() { return curFocusDol; }
	inline void setCurFocusDoll(BaseTaticDoll* _focus) { curFocusDol = _focus; if (_focus == nullptr) cameraFocus = false; }

	//������ �д븦 �ҷ��ɴϴ�.
	inline SquadData* getPlayerSquad(UINT squadID) {
		return tacDoll->getSquad(squadID) != nullptr ? tacDoll->getSquad(squadID) : nullptr; 
	}

	//��Ŀ�̵� ���������� �ҷ��ɴϴ�.
	//inline IOPsuportDoll* getIOPsptDoll_crntFocus() { return tacDoll->getSupportDoll(curntSuportDoll); }

	inline GriffonDoll& getPlayerTaticDoll() { return *tacDoll; }

	inline SINT& getCurrentSquad() { return currentSquad; }

	inline const unordered_map<string, EquipContainer>& getPlayerEquip() { return mPlayerEquip; }
	inline unordered_map<string, EquipContainer>& pGetPlayerEquip() { return mPlayerEquip; }

	inline int getOwnEquipNumber() const { return OwnEquipNumber; }

	inline void setTechKeyLock(const bool _lock) { lockTechKey = _lock; }

private:
	int												OwnEquipNumber;			//	������ ��� ����

	bool											multiSelectMod;			//	is been MultiSelected
	bool											cameraFocus;			//	ī�޶� ��Ŀ���ΰ�?
	bool											RemUnit_Toggle;			//	Remember Unit Hotkey Toggle Boolean
	bool											lockTechKey;			//	��Ű ���

	SINT											currentSquad;			//	���� ������ ���ϰ��ִ� �д�

	POINT											sPos, ePos;				//
	D3DXVECTOR2										mPos, trgPos;			//

	BaseTaticDoll*									curFocusDol;			//	���� ��Ŀ���ϰ��ִ� ����
	GriffonDoll*									tacDoll;

	vector<UINT>									vMultiSelect;			//	���� ������ �������
	unordered_map<int, vector<UINT>>				mRemUnit;				//	�δ�����
	unordered_map<int, vector<UINT>>::iterator		miRemUnit;

	vector<TATICDOLL_ALIANCE_TYPE>					vAlianceList;			//	���� �������� �Ҽ� ���ֵ�

	unordered_map<string, EquipContainer>			mPlayerEquip;			//	�÷��̾ �����ϰ��ִ� ������
	unordered_map<string, EquipContainer>::iterator EquipIter;				//	���� ���� Iterator

	unordered_map<SINT, BaseThermalTeam*>			curSupportList;			//	���� ������ �����ϰ� �ִ� ȭ�¼Ҵ� ����Ʈ

public:
	FLOAT						playerHighLight;
	FLOAT						maxPlayerHighLight;
	bool						PlayerSlow;
};
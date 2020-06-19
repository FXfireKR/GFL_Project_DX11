#pragma once
#include "GriffonDoll.h"

struct EquipContainer
{
	BYTE				num;			//	갯수
	EquipBase*			equip;			//	장비
	EQUIPTYPE			equipType;		//	타입
	EQUIPTOTAL_TYPE		allEquipType;	//	세부 분류

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

	//현재 전투중인 분대의 분대원을 불러옵니다
	inline BaseTaticDoll* getIOPdoll_crntSquad(UINT squadID) { return tacDoll->getSquadMember(currentSquad, squadID); }

	//현재 전투중인 분대의 포커싱된 분대원을 불러옵니다
	inline BaseTaticDoll* getIOPdoll_crntSquadFocus() { return curFocusDol; }

	inline const BaseTaticDoll* getCurFocusDoll() { return curFocusDol; }
	inline void setCurFocusDoll(BaseTaticDoll* _focus) { curFocusDol = _focus; if (_focus == nullptr) cameraFocus = false; }

	//지정된 분대를 불러옵니다.
	inline SquadData* getPlayerSquad(UINT squadID) {
		return tacDoll->getSquad(squadID) != nullptr ? tacDoll->getSquad(squadID) : nullptr; 
	}

	//포커싱된 전술인형을 불러옵니다.
	//inline IOPsuportDoll* getIOPsptDoll_crntFocus() { return tacDoll->getSupportDoll(curntSuportDoll); }

	inline GriffonDoll& getPlayerTaticDoll() { return *tacDoll; }

	inline SINT& getCurrentSquad() { return currentSquad; }

	inline const unordered_map<string, EquipContainer>& getPlayerEquip() { return mPlayerEquip; }
	inline unordered_map<string, EquipContainer>& pGetPlayerEquip() { return mPlayerEquip; }

	inline int getOwnEquipNumber() const { return OwnEquipNumber; }

	inline void setTechKeyLock(const bool _lock) { lockTechKey = _lock; }

private:
	int												OwnEquipNumber;			//	보유한 장비 갯수

	bool											multiSelectMod;			//	is been MultiSelected
	bool											cameraFocus;			//	카메라가 포커싱인가?
	bool											RemUnit_Toggle;			//	Remember Unit Hotkey Toggle Boolean
	bool											lockTechKey;			//	핫키 잠금

	SINT											currentSquad;			//	현재 전투에 임하고있는 분대

	POINT											sPos, ePos;				//
	D3DXVECTOR2										mPos, trgPos;			//

	BaseTaticDoll*									curFocusDol;			//	현재 포커싱하고있는 유닛
	GriffonDoll*									tacDoll;

	vector<UINT>									vMultiSelect;			//	다중 선택을 했을경우
	unordered_map<int, vector<UINT>>				mRemUnit;				//	부대지정
	unordered_map<int, vector<UINT>>::iterator		miRemUnit;

	vector<TATICDOLL_ALIANCE_TYPE>					vAlianceList;			//	현재 동맹중인 소속 유닛들

	unordered_map<string, EquipContainer>			mPlayerEquip;			//	플레이어가 보유하고있는 부착물
	unordered_map<string, EquipContainer>::iterator EquipIter;				//	상위 값의 Iterator

	unordered_map<SINT, BaseThermalTeam*>			curSupportList;			//	현재 전투에 보조하고 있는 화력소대 리스트

public:
	FLOAT						playerHighLight;
	FLOAT						maxPlayerHighLight;
	bool						PlayerSlow;
};
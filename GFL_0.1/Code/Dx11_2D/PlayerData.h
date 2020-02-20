#pragma once
#include "GriffonDoll.h"

class PlayerData
{
private:
	struct EquipContainer
	{
		BYTE				num;			//	갯수
		EquipBase*			equip;			//	장비
		EQUIPTYPE			equipType;		//	
		EQUIPTOTAL_TYPE		allEquipType;	//	

		EquipContainer(EquipBase* _equip, BYTE _num = 1) : equip(_equip) , num(_num)
		{
			allEquipType = equip->getItemType();
			equipType = equip->getEquipType();
		}
	};

private:

	int							OwnEquipNumber;			//	보유한 장비 갯수

	bool						multiSelectMod;
	bool						cameraFocus;			//	카메라가 포커싱인가?
	bool						RemUnit_Toggle;

	SINT						currentSquad;			//	현재 전투에 임하고있는 분대

	POINT						sPos, ePos;
	D3DXVECTOR2					mPos, trgPos;

		
	TaticDoll*					curFocusDol;			//	현재 포커싱하고있는 유닛
	GriffonDoll*				tacDoll;

	vector<UINT>				vMultiSelect;			//	다중 선택을 했을경우
	map<int, vector<UINT>>		mRemUnit;

	vector<TATICDOLL_ALIANCE_TYPE> vAlianceList;		//	현재 동맹중인 소속 유닛들

	//IOP_SUPPORTDOLL_TYPE curntSuportDoll;				//	현재 전투에 보조하고있는 화력소대 이름

	map<string, EquipContainer> mPlayerEquip;			//	플레이어가 보유하고있는 부착물
	map<string, EquipContainer>::iterator EquipIter;		//	상위 값의 Iterator

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

	void Change_FocusTacDoll();			//넘버패드를 눌러 변경한다.
	void Change_FocusTacDoll_Func(int memID);
	void SaveRemUnit(int padID);
	void LoadRemUnit(int padID);

	void AutoSetting_CameraFocus();		//전술인형 선택시, 자동으로 카메라를 이동시킨다.

	bool Targetting_Other();			//마우스 내부의 대상에게 타겟팅

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

	//현재 전투중인 분대의 분대원을 불러옵니다
	inline TaticDoll* getIOPdoll_crntSquad(UINT squadID) { return (TaticDoll*)(tacDoll->getSquadMember(currentSquad, squadID)); }

	//현재 전투중인 분대의 포커싱된 분대원을 불러옵니다
	inline TaticDoll* getIOPdoll_crntSquadFocus() { return curFocusDol; }

	inline const TaticDoll* getCurFocusDoll() { return curFocusDol; }
	inline void setCurFocusDoll(TaticDoll* _focus) { curFocusDol = _focus; if (_focus == nullptr) cameraFocus = false; }

	//지정된 분대를 불러옵니다.
	inline tagSquad<TaticDoll*>* getPlayerSquad(UINT squadID) { return tacDoll->getSquad(squadID) != nullptr ? tacDoll->getSquad(squadID) : nullptr; }

	//포커싱된 전술인형을 불러옵니다.
	//inline IOPsuportDoll* getIOPsptDoll_crntFocus() { return tacDoll->getSupportDoll(curntSuportDoll); }

	inline GriffonDoll& getPlayerTaticDoll() { return *tacDoll; }

	inline SINT& getCurrentSquad() { return currentSquad; }

	inline const map<string, EquipContainer>& getPlayerEquip() { return mPlayerEquip; }
	inline map<string, EquipContainer>& pGetPlayerEquip() { return mPlayerEquip; }

	inline int getOwnEquipNumber() const { return OwnEquipNumber; }

};
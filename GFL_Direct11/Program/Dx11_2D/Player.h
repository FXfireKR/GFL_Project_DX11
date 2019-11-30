#pragma once

#include "singleton.h"
#include "GriffonDoll.h"

class Player : public singleton<Player>
{
private:
	GriffonDoll* tacDoll;

	UINT currentSquad;		//현재 전투에 임하고있는 분대
	UINT focusTacdolID;		//포커싱한 전술인형 분대멤버 ID
	UINT cFocusTacDollID;	//카메라가 포커싱하고있는 분대멤버 ID	[ 0일경우 아무것도 안함 ]

	//IOP_SUPPORTDOLL_TYPE curntSuportDoll;	//현재 전투에 보조하고있는 화력소대 이름


	POINT sPos, ePos;
	FPNT mPos, trgPos;
	bool multiSelectMod;

	vector<UINT> vMultiSelect;		//다중 선택을 했을경우
	map<int, vector<UINT>> mRemUnit;

	//map<UINT, EquipBase> mPlayerEquip;		//플레이어가 보유하고있는 부착물
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


	void Change_FocusTacDoll();			//넘버패드를 눌러 변경한다.
	void Change_FocusTacDoll_Func(int memID);
	void SaveRemUnit(int padID);
	void LoadRemUnit(int padID);

	void AutoSetting_CameraFocus();		//전술인형 선택시, 자동으로 카메라를 이동시킨다.

	bool Targetting_Other();			//마우스 내부의 대상에게 타겟팅

	void testFuc();
	void testinst();

public:
	//현재 전투중인 분대의 분대원을 불러옵니다
	inline IOPtaticDoll* getIOPdoll_crntSquad(UINT squadID)
	{
		return (IOPtaticDoll*)(tacDoll->getSquadMember(currentSquad, squadID));
	}

	//현재 전투중인 분대의 포커싱된 분대원을 불러옵니다
	inline IOPtaticDoll* getIOPdoll_crntSquadFocus() { return focusTacdolID > 0 ? (IOPtaticDoll*)(tacDoll->getSquadMember(currentSquad, focusTacdolID)) : nullptr; }

	//지정된 분대를 불러옵니다.
	inline tagSquad<IOPtaticDoll*>* getPlayerSquad(UINT squadID) { return tacDoll->getSquad(squadID) != nullptr ? tacDoll->getSquad(squadID) : nullptr; }

	//포커싱된 전술인형을 불러옵니다.
	//inline IOPsuportDoll* getIOPsptDoll_crntFocus() { return tacDoll->getSupportDoll(curntSuportDoll); }

	inline GriffonDoll& getPlayerTaticDoll() { return *tacDoll; }

	inline UINT getCameraFocusTacDollID() { return cFocusTacDollID; }
	inline void setCameraFocusTacDollID(UINT id) { cFocusTacDollID = id > 0 && id < tacDoll->getSquad(currentSquad)->mSquad.size() + 1 ? id : 0; }

	inline UINT& getCurrentSquad() { return currentSquad; }

	//inline map<UINT, EquipBase>& getPlayerEquip() { return mPlayerEquip; }
	//inline map<UINT, EquipBase>::iterator& getEquipIterator() { return EquipIter; }

	inline int getOwnEquipNumber() const { return OwnEquipNumber; }

};
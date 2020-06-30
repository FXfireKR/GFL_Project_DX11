#pragma once
#include "Scene.hpp"
#include "Helicopter.h"

class worldMapScene : public Scene
{
public:
	worldMapScene();
	~worldMapScene();

	// Scene을(를) 통해 상속됨
	virtual void init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

	static worldMapScene* objectPtr;	//	Button Pointer

private:

	void updateSquadLeader();
	void keyUpate();

	void render_NormalWorld();	//	일반
	void render_SelectWorld();	//	분대 선택중

	bool Find_SquadInWorld(TATICDOLL_ALIANCE_TYPE ac, UINT id);


	//	@ when Button was selected action fnc
	static void TurnChange_Select(void* obj);
	static void SquadBatch_Select(void* obj);
	static void GiveUpGame_Select(void* obj);
	static void InsertSquad_Select(void* obj);
	static void BackToNormal_Select(void* obj);
	static void NormalSquad_Select(void* obj);
	static void ThermalSquad_Select(void* obj);

private:
	enum BUTTON_CODE {
		BCODE_TURN = 1,
		BCODE_BATCH,
		BCODE_GIVEUP,

		BCODE_INSERT,
		BCODE_BACK,
		BCODE_NORMAL_SQUAD,
		BCODE_THERMAL_SQUAD,
		BCODE_END
	};

private:
	bool rendSquad;

	int Focus_Squad;		//	현재 선택된 분대 (없을경우 -1)
	UINT Spawn_Squad;		//	배치할 분대
	int Spawn_PanelID;		//	배치할 패널위치

	unordered_map<TATICDOLL_ALIANCE_TYPE, vector<UINT>> mInstSquad;
	vector<LoadResourceData>							vLoadList;
	vector<D2D_RECT_F>									squadButton;

	int Turn;				//	N번째 턴
	bool worldmapInit;		//	
	bool squadInit;			//	
	bool isInsertSquad;		//	분대 배치 선택중인가
	bool isThermalSupport;	//	화력 분대 배치인가

	TATICDOLL_ALIANCE_TYPE turnAliance;

	Vector2 guardPos;

	unordered_map<BUTTON_CODE, Button>		mButton;
};

/* 
	===========================================================

	@ 버튼 종류

	[ 일반 월드 ]
	 - 다음 턴
	 - 배치
	 - 작전 포기

	[ 월드 배치 ]
	 - 투입
	 - 일반 소대
	 - 화력 보조 소대
	 - 뒤로

	=========================================================== 
 */
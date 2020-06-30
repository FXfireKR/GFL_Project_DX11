#pragma once
#include "Scene.hpp"
#include "Helicopter.h"

class worldMapScene : public Scene
{
public:
	worldMapScene();
	~worldMapScene();

	// Scene��(��) ���� ��ӵ�
	virtual void init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

	static worldMapScene* objectPtr;	//	Button Pointer

private:

	void updateSquadLeader();
	void keyUpate();

	void render_NormalWorld();	//	�Ϲ�
	void render_SelectWorld();	//	�д� ������

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

	int Focus_Squad;		//	���� ���õ� �д� (������� -1)
	UINT Spawn_Squad;		//	��ġ�� �д�
	int Spawn_PanelID;		//	��ġ�� �г���ġ

	unordered_map<TATICDOLL_ALIANCE_TYPE, vector<UINT>> mInstSquad;
	vector<LoadResourceData>							vLoadList;
	vector<D2D_RECT_F>									squadButton;

	int Turn;				//	N��° ��
	bool worldmapInit;		//	
	bool squadInit;			//	
	bool isInsertSquad;		//	�д� ��ġ �������ΰ�
	bool isThermalSupport;	//	ȭ�� �д� ��ġ�ΰ�

	TATICDOLL_ALIANCE_TYPE turnAliance;

	Vector2 guardPos;

	unordered_map<BUTTON_CODE, Button>		mButton;
};

/* 
	===========================================================

	@ ��ư ����

	[ �Ϲ� ���� ]
	 - ���� ��
	 - ��ġ
	 - ���� ����

	[ ���� ��ġ ]
	 - ����
	 - �Ϲ� �Ҵ�
	 - ȭ�� ���� �Ҵ�
	 - �ڷ�

	=========================================================== 
 */
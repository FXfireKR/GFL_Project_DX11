#pragma once

#include "TaticDollBase.h"

//공격할 대상 판별
enum TARGETTING_ENEMY
{
	GRIFFON_TACDOLL = 0,
	SANGVIS_TACDOLL
};

class SNVtacDoll : public TaticDollBase
{
protected:
	TARGETTING_ENEMY targTeam;		//
	string original_path;	//캐릭터의 메인 일러스트 path입니다.

	double mvPointX;		//움직여야하는 좌표 X
	double mvPointY;		//움직여야하는 좌표 Y

	int smartTarget;
	float smartCounter;

	bool moveAble;

	vector<UINT> vRange;	//사거리내의 적군ID

public:
	SNVtacDoll();
	virtual ~SNVtacDoll();

	virtual void LoadTray_List()override;
	virtual HRESULT init()override;
	virtual void release()override;
	virtual void update()override;
	virtual void render()override;

	virtual void render_VisualBar()override;
	virtual void render_Motion()override;
	virtual void render_Ellipse()override;

	virtual void MotionUpdate();

	void IsEnemy_at();
	bool FindEnemy_ID(UINT id);
	int getEnemy_ID_Pos(UINT id);

	void Set_Targetting();			//사거리 내의 적을 포커싱합니다.
	void Set_Targetting_Angle();	//포착시에, 앵글을 최신화 해줍니다.


	void SetMovePoint(int x, int y);
	void MoveToSetPoint();

public:
	//Static형식의 함수를 만드는데 사용되는것들...
	inline MotionManager& getMotionManager() { return *motion; }
	//inline SoundManager& getSoundManager() { return *sound; }

	inline float* getAttackColTime() { return &AtkColTime; }
	inline float* getSkillColTime() { return &SklColTime; }

	inline const Status& getStatus() { return state; }

	inline void setTargetOS(TARGETTING_ENEMY os) { targTeam = os; vRange.clear(); if (isAlive) { Target_ID = -1; smartTarget = -1; motion->Change_Motion("IDLE"); } }
	inline const TARGETTING_ENEMY& getTargetOS() { return targTeam; }
};
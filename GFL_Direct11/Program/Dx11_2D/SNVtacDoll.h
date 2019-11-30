#pragma once

#include "TaticDollBase.h"

//������ ��� �Ǻ�
enum TARGETTING_ENEMY
{
	GRIFFON_TACDOLL = 0,
	SANGVIS_TACDOLL
};

class SNVtacDoll : public TaticDollBase
{
protected:
	TARGETTING_ENEMY targTeam;		//
	string original_path;	//ĳ������ ���� �Ϸ���Ʈ path�Դϴ�.

	double mvPointX;		//���������ϴ� ��ǥ X
	double mvPointY;		//���������ϴ� ��ǥ Y

	int smartTarget;
	float smartCounter;

	bool moveAble;

	vector<UINT> vRange;	//��Ÿ����� ����ID

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

	void Set_Targetting();			//��Ÿ� ���� ���� ��Ŀ���մϴ�.
	void Set_Targetting_Angle();	//�����ÿ�, �ޱ��� �ֽ�ȭ ���ݴϴ�.


	void SetMovePoint(int x, int y);
	void MoveToSetPoint();

public:
	//Static������ �Լ��� ����µ� ���Ǵ°͵�...
	inline MotionManager& getMotionManager() { return *motion; }
	//inline SoundManager& getSoundManager() { return *sound; }

	inline float* getAttackColTime() { return &AtkColTime; }
	inline float* getSkillColTime() { return &SklColTime; }

	inline const Status& getStatus() { return state; }

	inline void setTargetOS(TARGETTING_ENEMY os) { targTeam = os; vRange.clear(); if (isAlive) { Target_ID = -1; smartTarget = -1; motion->Change_Motion("IDLE"); } }
	inline const TARGETTING_ENEMY& getTargetOS() { return targTeam; }
};
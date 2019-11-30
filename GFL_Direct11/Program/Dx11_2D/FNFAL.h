#pragma once
#include "IOPtaticDoll.h"
#include "SmokeBase.h"

#define FNFAL_SPEED			125.0f
#define FNFAL_ACLRENDER		1.0F

#define FNFAL_SKILL_LRAD		200.0F
#define FNFAL_SKILL_SRAD		60.0F

//서서 대기/이동시
#define FNFAL_DRAWPOS_X		144.0f * FNFAL_ACLRENDER
#define FNFAL_DRAWPOS_Y		205.0f * FNFAL_ACLRENDER

#define FNFAL_YUTAN_MAX		3

class FNFAL : public IOPtaticDoll
{
private:
	struct UtanInfo
	{
		EllipseBase*	u;
		vector<UINT>	vTanRange;		//유탄 범위내 적군 ID
		SmokeBase*		utan;			
		FPNT			sklPos;
	};

private:
	vector<UtanInfo*>	vUtan;

	int FiredYutan;

public:
	FNFAL();
	~FNFAL();

	void LoadTray_List()override;
	HRESULT init()override;
	void release()override;
	void update()override;
	void render()override;
	void Use_ActiveSkill()override;
	void MotionUpdate()override;
	void Update_DrawPos()override;

	void render_VisualBar()override;
	void render_Motion()override;
	void render_Ellipse()override;

public:
	static void FNFAL_AttackAction(void * ob);
	static void FNFAL_SkillAction(void * ob);
	static void FNFAL_DeathAction(void * ob);
	static void FNFAL_VictoryAction(void * ob);

	inline int& getFiredYutan() { return FiredYutan; }

	inline SmokeBase* getYutan(int idx) { return vUtan[idx]->utan; }
	inline FPNT getSklPos(int idx) { return vUtan[idx]->sklPos; }

	inline float getLookAngle() { return Angle; }
};


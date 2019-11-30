#pragma once
#include "IOPtaticDoll.h"
#include "SmokeBase.h"

#define ARCHITECT_SPEED					105.0f
#define ARCHITECT_ACLRENDER				1.0F

#define ARCHITECT_SATTACK_LRAD			150.0F
#define ARCHITECT_SATTACK_SRAD			50.0F

#define ARCHITECT_YUTAN_MAX				2	

class ARCHITECT : public IOPtaticDoll
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
	ARCHITECT();
	~ARCHITECT();

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
	static void ARCHITECT_AttackAction(void * ob);
	static void ARCHITECT_sAttackAction(void * ob);

	static void ARCHITECT_Skill1_Action(void * ob);
	static void ARCHITECT_Skill2_Action(void * ob);
	static void ARCHITECT_Skill3_Action(void * ob);
	static void ARCHITECT_Skill4_Action(void * ob);

	inline int& getFiredYutan() { return FiredYutan; }

	inline SmokeBase* getYutan(int idx) { return vUtan[idx]->utan; }
	inline UtanInfo* getYutanInfo(int idx) { return vUtan[idx]; }
	inline FPNT getSklPos(int idx) { return vUtan[idx]->sklPos; }

	inline float getLookAngle() { return Angle; }

	inline void setMoveAble(bool able) { moveAble = able; }

	//static void FNFAL_SkillAction(void * ob);
	//static void FNFAL_DeathAction(void * ob);
	//static void FNFAL_VictoryAction(void * ob);

};


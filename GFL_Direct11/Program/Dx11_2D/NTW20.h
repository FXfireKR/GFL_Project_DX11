#pragma once
#include "IOPtaticDoll.h"

#define NTW20_SPEED			95.0f
#define NTW20_ACLRENDER		1.0F

//서서 대기/이동시
#define NTW20_DRAWPOS_X		144.0f * NTW20_ACLRENDER
#define NTW20_DRAWPOS_Y		205.0f * NTW20_ACLRENDER

//누워서 사격중일때
#define NTW20_DRAWPOS_X2	175.0f * NTW20_ACLRENDER
#define NTW20_DRAWPOS_Y2	206.0f * NTW20_ACLRENDER

#define NTW20_FOCUS			10.5f

class NTW20 : public IOPtaticDoll
{
private:


public:
	NTW20();
	~NTW20();

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
	static void NTW20_AttackAction(void * ob);
	static void NTW20_SkillAction(void * ob);
	static void NTW20_DeathAction(void * ob);
	static void NTW20_VictoryAction(void * ob);

	//inline int getSkillTarget() { return sklTarget_ID; }
	//inline Bullet& getBullet() { return *skl; }

};
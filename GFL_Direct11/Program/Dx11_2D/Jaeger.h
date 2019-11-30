#pragma once
#include "SNVtacDoll.h"

#define JAEGER_SPEED			1.25F
#define JAEGER_ACLRENDER		1.0F
#define JAEGER_DRAWPOS_X		149.0f * JAEGER_ACLRENDER
#define JAEGER_DRAWPOS_Y		202.0f * JAEGER_ACLRENDER


#define HYDRA_SMART_AI_FINDER_TIME	0.65f

class Jaeger : public SNVtacDoll
{
private:


public:
	Jaeger();
	~Jaeger();

	static void LoadTray();
	void LoadTray_List()override;
	HRESULT init()override;
	void release()override;
	void update()override;
	void MotionUpdate()override;

	void render_VisualBar()override;
	void render_Motion()override;
	void render_Ellipse()override;

	void UseSkill();
	void Action_Algorydm();

public:
	static void Jaeger_AttackAction(void * ob);
	static void Jaeger_SkillAction(void * ob);
	static void Jaeger_DeathAction(void * ob);
};
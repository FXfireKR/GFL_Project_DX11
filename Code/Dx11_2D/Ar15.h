#pragma once
#include "BaseTaticDoll.h"

class Ar15 : public BaseTaticDoll
{
protected:
	const double	AR15_SPEED = 200.0;
	const float		AR15_ACL_X = 0.8f;
	const float		AR15_ACL_Y = 0.8f;
	const double	AR15_SKILL_COLTIME = 8.0;
	const double	AR15_SKILL_MAINTAIN = 10.0;

	BYTE safeTrigger;

	double skillTimer;

	bool waitAfter;
	bool isActive;

public:
	Ar15();
	~Ar15();

	virtual void LoadTray_SoundList()override;
	virtual void LoadTray_ImageList()override;
	virtual HRESULT init()override;
	virtual void release()override;
	virtual void update()override;
	virtual void render()override;
	virtual void Use_ActiveSkill()override;
	virtual void MotionUpdate()override;
	virtual void Update_DrawPos()override;

	virtual void render_VisualBar()override;
	virtual void render_Motion()override;
	virtual void render_Ellipse()override;

public:
	static void AR15_Attack_Action(void* _this);
	//static void AR15_Reload_Action(void* _this);
};
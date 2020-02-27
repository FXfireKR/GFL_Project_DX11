#pragma once
#include "TaticDoll.h"

class Ksg : public TaticDoll
{
protected:
	const double	KSG_SPEED = 200.0;
	const float		KSG_ACL_X = 0.8f;
	const float		KSG_ACL_Y = 0.8f;
	const double	KSG_SKILL_COLTIME = 8.0;
	const double	KSG_SKILL_MAINTAIN = 10.0;

	const BYTE		KSG_MAX_BULLET = 5;
	
	BYTE safeTrigger;
	BYTE leftBullet;

	double skillTimer;

	bool isNeedReload;
	bool waitAfter;
	bool isActive;

public:
	Ksg();
	~Ksg();

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
	static void KSG_Attack_Action(void* _this);
	static void KSG_Reload_Action(void* _this);
};
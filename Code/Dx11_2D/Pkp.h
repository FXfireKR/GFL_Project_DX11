#pragma once
#include "BaseTaticDoll.h"

class Pkp : public BaseTaticDoll
{
public:
	Pkp();
	~Pkp();

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
	static void PKP_Attack_Action(void* _this);
	static void PKP_Reload_Action(void* _this);

protected:
	const double	PKP_SPEED = 100.0;
	const float		PKP_ACL_X = 0.8f;
	const float		PKP_ACL_Y = 0.8f;
	const double	PKP_SKILL_COLTIME = 8.0;
	const double	PKP_SKILL_MAINTAIN = 10.0;
	const BYTE		PKP_MAX_MAGAZINE = 30;

	BYTE safeTrigger;
	BYTE leftBullet;

	double skillTimer;

	bool waitAfter;
	bool isActive;
	bool needReload;
};
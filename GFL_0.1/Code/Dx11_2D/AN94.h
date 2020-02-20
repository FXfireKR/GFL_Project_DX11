#pragma once
#include "TaticDoll.h"

class AN94 : public TaticDoll
{
private:
	const double	AN94_SPEED = 125.0;
	const float		AN94_ACL_X = 0.8f;
	const float		AN94_ACL_Y = 0.8f;
	const float		AN94_SKILL_RANGE_X = 200.0f;
	const float		AN94_SKILL_RANGE_Y = 50.0f;
	const float		AN94_SKILL_COLTIME = 10.5f;
	const float		AN94_SKILL_RENDSCALE = 20.0f;

	BYTE			safeTirgger;

public:
	AN94();
	~AN94();

	void LoadTray_SoundList()override;
	void LoadTray_ImageList()override;
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

private:


public:
	static void AN94_Attack_Action(void* _this);
};


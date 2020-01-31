#pragma once
#include "TaticDoll.h"

class AR15 : public TaticDoll
{
private:
	const double	AR15_SPEED = 125.0;
					
	const float		AR15_ACL_X = 0.8f;
	const float		AR15_ACL_Y = 0.8f;

public:
	AR15();
	~AR15();

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

public:
	static void AR15_Attack_Action(void* _this);
	static void AR15_Skill_Action(void* _this);
};


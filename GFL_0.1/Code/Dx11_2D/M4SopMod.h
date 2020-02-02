#pragma once
#include "TaticDoll.h"

class M4SopMod : public TaticDoll
{
private:
	const double	M4SOP_SPEED = 125.0;
					
	const float		M4SOP_ACL_X = 0.8f;
	const float		M4SOP_ACL_Y = 0.8f;

public:
	M4SopMod();
	~M4SopMod();

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
	static void M4SopMod_Attack_Action(void* _this);
	static void M4SopMod_Skill_Action(void* _this);
};


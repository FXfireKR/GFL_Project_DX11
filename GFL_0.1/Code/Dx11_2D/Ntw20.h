#pragma once
#include "TaticDoll.h"

class Ntw20 : public TaticDoll
{
private:
	const double	NTW20_SPEED = 125.0;
				
	const float		NTW20_ACL_X = 0.8f;
	const float		NTW20_ACL_Y = 0.8f;

public:
	Ntw20();
	~Ntw20();

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
	static void NTW20_Attack_Action(void* _this);
	static void NTW20_Skill_Action(void* _this);
};


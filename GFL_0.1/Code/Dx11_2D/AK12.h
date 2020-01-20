#pragma once
#include "TaticDoll.h"

class AK12 : public TaticDoll
{
private:
	const double	AK12_SPEED = 125.0;

	const float		AK12_ACL_X = 0.8f;
	const float		AK12_ACL_Y = 0.8f;

public:
	AK12();
	~AK12();

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
	static void AK12_Attack_Action(void* _this);
};
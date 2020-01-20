#pragma once
#include "TaticDoll.h"

class GuA91 : public TaticDoll
{
private:
	const double	GuA91_SPEED = 125.0;
					
	const float		GuA91_ACL_X = 0.8f;
	const float		GuA91_ACL_Y = 0.8f;

public:
	GuA91();
	~GuA91();

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
	static void GuA91_Attack_Action(void* _this);
};
#pragma once
#include "TaticDoll.h"

class ARCHITECT : public TaticDoll
{
private:
	const double						ARCHITECT_SPEED = 125.0;
										
	const float							ARCHITECT_ACL_X = 0.8f;
	const float							ARCHITECT_ACL_Y = 0.8f;

	const TATICDOLL_ALIANCE_TYPE		ARCHITECT_ALIANCE = ALIANCE_IRONBLOD;

public:
	ARCHITECT();
	~ARCHITECT();

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
	static void Architect_Attack_Action(void* _this);
};


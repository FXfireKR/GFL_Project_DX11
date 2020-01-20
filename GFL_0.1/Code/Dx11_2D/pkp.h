#pragma once
#include "TaticDoll.h"

class pkp : public TaticDoll
{
private:
	const double	PKP_SPEED = 125.0;

	const float		PKP_ACL_X = 0.8f;
	const float		PKP_ACL_Y = 0.8f;

	const UINT		PKP_MAX_AMMO = 42;

private:
	UINT			mAmmo;		//	½ò¼öÀÖ´Â ÅºÈ¯ ¼ö

public:
	pkp();
	~pkp();

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
	static void pkp_Attack_Action(void* _this);
	static void pkp_Reload_Action(void* _this);
};
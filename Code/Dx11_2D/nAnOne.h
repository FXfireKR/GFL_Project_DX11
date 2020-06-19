#pragma once
#include "BaseTaticDoll.h"

class nAnOne : public BaseTaticDoll
{
public:
	nAnOne();
	virtual ~nAnOne();

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

	inline spineMotion* getMotion() { return motion; }

public:
	static void NA91_Attack_Action(void* _this);

private:
	const float		NA91_SPEED = 125.0f;

	const float		NA91_STAND_Y = 120.0f;
	const float		NA91_DOWN_Y = 80.0f;
					
	const float		NA91_ACL_X = 0.8f;
	const float		NA91_ACL_Y = 0.8f;
};
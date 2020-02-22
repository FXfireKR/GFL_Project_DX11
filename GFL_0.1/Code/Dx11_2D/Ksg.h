#pragma once
#include "TaticDoll.h"

class Ksg : public TaticDoll
{
protected:
	const double	KSG_SPEED = 200.0;
	const float		KSG_ACL_X = 0.8f;
	const float		KSG_ACL_Y = 0.8f;
	const BYTE		KSG_MAX_BULLET = 8;
	
	BYTE safeTrigger;
	BYTE leftBullet;

	bool isNeedReload;
	bool waitAfter;

public:
	Ksg();
	~Ksg();

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
	static void KSG_Attack_Action(void* _this);

};
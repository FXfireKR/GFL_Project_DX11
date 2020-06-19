#pragma once
#include "BaseThermalTeam.h"	

class Bgm71 : public BaseThermalTeam
{
public:
	Bgm71();
	~Bgm71();

	virtual void LoadTray_SoundList()override;
	virtual void LoadTray_ImageList()override;
	virtual HRESULT init()override;
	virtual void release()override;
	virtual void update()override;
	virtual void render()override;
	virtual void MotionUpdate()override;
	virtual void Update_DrawPos()override;

	virtual void render_Motion()override;
	virtual void render_Ellipse()override;

public:
	static void BGM71_Attack_Action(void* _this);
	static void BGM71_Reload_Action(void* _this);

protected:
	const double	BGM71_SPEED = 100.0;
	const float		BGM71_ACL_X = 0.8f;
	const float		BGM71_ACL_Y = 0.8f;

	BYTE safeTrigger;

	bool waitAfter;
	bool needReload;
};
#pragma once
#include "BaseTaticDoll.h"

class M4sopmod : public BaseTaticDoll
{
public:
	M4sopmod();
	~M4sopmod();

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

private:
	void utanUpdate();
	void utanRender();
	void skilUpdate();

public:
	static void M4SopMod_Attack_Action(void* _this);
	static void M4SopMod_Skill_Action(void* _this);


private:
	const double	M4SOP_SPEED = 125.0;

	const float		M4SOP_ACL_X = 0.8f;
	const float		M4SOP_ACL_Y = 0.8f;

	const float		M4SOP_SKILL_RANGE_X = 200.0f;
	const float		M4SOP_SKILL_RANGE_Y = 50.0f;

	const float		M4SOP_SKILL_COLTIME = 10.5f;

	const float		M4SOP_SKILL_RENDSCALE = 40.0f;

	BYTE			safeTirgger;
	bool			sklPreShow;

	YutanBase*		sklBullet;
	D3DXVECTOR2		sklCenter;
	vector<UINT>	vSklRange;
	EllipseBase*	sklRange;
};
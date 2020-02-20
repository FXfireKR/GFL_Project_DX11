#pragma once
#include "TaticDoll.h"

class M4SopMod : public TaticDoll
{
private:
	const double	M4SOP_SPEED = 125.0;
					
	const float		M4SOP_ACL_X = 0.8f;
	const float		M4SOP_ACL_Y = 0.8f;

	const float		M4SOP_SKILL_RANGE_X = 200.0f;
	const float		M4SOP_SKILL_RANGE_Y = 50.0f;

	const float		M4SOP_SKILL_COLTIME = 10.5f;

	const float		M4SOP_SKILL_RENDSCALE = 20.0f;

	BYTE			safeTirgger;
	bool			sklPreShow;

	UtanBase*		sklBullet;
	D3DXVECTOR2		sklCenter;
	vector<UINT>	vSklRange;
	EllipseBase*	sklRange;


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

private:
	void utanUpdate();
	void utanRender();
	void skilUpdate();

public:
	static void M4SopMod_Attack_Action(void* _this);
	static void M4SopMod_Skill_Action(void* _this);
};
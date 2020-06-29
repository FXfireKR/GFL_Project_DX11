#pragma once
#include "BaseTaticDoll.h"

class Ntw20 : public BaseTaticDoll
{
public:
	Ntw20();
	~Ntw20();

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
	static void Ntw20_Attack_Action(void* _this);
	static void Ntw20_Skill_Action(void* _this);


private:
	const double	NTW20_SPEED = 100.0;

	const float		NTW20_ACL_X = 0.8f;
	const float		NTW20_ACL_Y = 0.8f;

	const float		NTW20_SKILL_RANGE_X = 200.0f;
	const float		NTW20_SKILL_RANGE_Y = 50.0f;

	const float		NTW20_SKILL_COLTIME = 10.5f;

	const float		NTW20_SKILL_RENDSCALE = 40.0f;

	BYTE			safeTirgger;
	bool			sklPreShow;

	D3DXVECTOR2		sklCenter;
	vector<UINT>	vSklRange;
	EllipseBase*	sklRange;
};
#pragma once
#include "BaseTaticDoll.h"

class Ump45 : public BaseTaticDoll
{
public:
	Ump45();
	~Ump45();

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
	static Ump45* self;

	static void Ump45_Attack_Action(void* _this);
	static void Ump45_Skill_Action(void* _this);

private:
	const double UMP45_SPEED = 125.0;

	const Vector2 UMP45_ACL = Vector2(0.8f, 0.8f);
	const Vector2 UMP45_SKILL_RANGE = Vector2(200.0f, 50.0f);

	const float UMP45_SKILL_COLTIME = 10.0f;
	
	BYTE			safeTrigger;
	bool			sklPreShow;

	YutanBase*		sklBullet;
	Vector2			sklCenter;
	vector<UINT>	vSklRange;
	EllipseBase*	sklRange;
};
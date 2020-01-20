#include "stdafx.h"
#include "ARCHITECT.h"

ARCHITECT::ARCHITECT()
{
}

ARCHITECT::~ARCHITECT()
{
}

void ARCHITECT::LoadTray_List()
{
}

HRESULT ARCHITECT::init()
{
	TaticDoll::init();

	axisMax_LongRad = 400.0f;
	axisMax_ShortRad = 150.0f;

	if (motion == nullptr)
	{
		motion = new spineMotion;

		if (motion != nullptr)
		{
			motion->loadSpine_FromJsonFile("boss9");
			//motion->setMotionAction("wait", AK12_Attack_Action);
			motion->setMotionObject(this);
			motion->init();
		}
	}

	Pos.x = 850.0f;
	Pos.y = 550.0f;

	mCollision.insert(make_pair("SELF", new EllipseBase(&Pos.x, &Pos.y, 30, 5)));
	//mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&Pos.x, &Pos.y, axisMax_LongRad, axisMax_ShortRad)));


	moveSpd = ARCHITECT_SPEED;

	curState.HitPoint.max = curState.HitPoint.curr = 993000;
	curState.Accuracy = 1.0;
	curState.CriticPoint = 25.5;
	curState.CriticAcl = 50;
	curState.AttackDelay = 0.8f;
	curState.AimDelay = 0.45;
	curState.Avoid = 0.835;
	curState.AttackPoint = 85;

	maxState = curState;

	atkColTime = 0.0;
	sklColTime = 0.0;
	TargetAngle = Angle = 0.0f;

	isAlive = true;
	Select = false;
	moveAble = true;

	charRect = RectMakeCenter(Pos.x, Pos.y + 10, 50, 80);

	return S_OK;
}

void ARCHITECT::release()
{
	TaticDoll::release();
}

void ARCHITECT::update()
{
	TaticDoll::update();

	this->Update_DrawPos();
	this->MotionUpdate();
	motion->update(DELTA);
}

void ARCHITECT::render()
{
}

void ARCHITECT::Use_ActiveSkill()
{
}

void ARCHITECT::MotionUpdate()
{
}

void ARCHITECT::Update_DrawPos()
{
	motion->p_getScale().x = ARCHITECT_ACL_X;
	motion->p_getScale().y = ARCHITECT_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (500.0 * DELTA) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (500.0 * DELTA) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void ARCHITECT::render_VisualBar()
{
}

void ARCHITECT::render_Motion()
{
	TaticDoll::render_Motion();
	motion->render();
}

void ARCHITECT::render_Ellipse()
{
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.8, 0.1, 0.1, 0.9f));
}

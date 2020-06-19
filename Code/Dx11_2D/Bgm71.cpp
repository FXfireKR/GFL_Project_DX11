#include "stdafx.h"
#include "Bgm71.h"

Bgm71::Bgm71()
{
	name = "BGM-71";

	curState.HitPoint.max = curState.HitPoint.curr = 2530;
	//curState.HitPoint.max = curState.HitPoint.curr = 992530;
	curState.Armor = 0;
	curState.Accuracy = 0.1;
	curState.CriticPoint = 40.0;
	curState.CriticAcl = 50;
	curState.AttackDelay = 0.0000;
	curState.AimDelay = 0.59;
	curState.Avoid = 0.135;
	curState.AttackPoint = 80;
	curState.ArmorPierce = 0;

	maxState = curState;

	//statusManager = new StatusManager(&curState, &maxState, NULL);
}

Bgm71::~Bgm71()
{
}

void Bgm71::LoadTray_SoundList() {}

void Bgm71::LoadTray_ImageList()
{
	this->init();
}

HRESULT Bgm71::init()
{
	BaseThermalTeam::init();

	axisMax_LongRad = 300.0f;
	axisMax_ShortRad = 100.0f;
	axisMin_LongRad = axisMax_LongRad * 0.45f;
	axisMin_ShortRad = axisMax_ShortRad * 0.45f;

	if (motion == nullptr)
	{
		motion = new spineMotion;

		if (motion != nullptr)
		{
			motion->loadSpine_FromJsonFile("TOW");
			motion->setMotionAction("attack", BGM71_Attack_Action);
			motion->setMotionAction("reload", BGM71_Reload_Action);

			for (int i = 0; i < 3; ++i)
			{
				motion->changeMotion("move", false, true);
				motion->changeMotion("attack", false, true, 0.001f);
			}
			motion->changeMotion("wait", false, true);

			motion->setMotionObject(this);
			motion->init();
		}
	}

	else
	{
		motion->reload_SpineImage();
		motion->init();
	}

	Pos.x = 200.0f;
	Pos.y = 250.0f;

	
	moveSpd = BGM71_SPEED;
	worldMoveSpd = moveSpd * 2;

	atkColTime = 0.0;
	TargetAngle = Angle = 0.0f;

	isAlive = true;
	Select = false;
	moveAble = true;

	safeTrigger = 0;

	alianceType = ALIANCE_GRIFFON;
	waitAfter = false;

	return S_OK;
}

void Bgm71::release()
{
	BaseThermalTeam::release();
}

void Bgm71::update()
{
	statusManager->update(DELTA() * DeltaAcl);

	BaseThermalTeam::update();

	update_Coltime();
	this->Update_DrawPos();
	motion->update(DELTA() * DeltaAcl);
	this->MotionUpdate();
}

void Bgm71::render()
{
}


void Bgm71::MotionUpdate()
{

}

void Bgm71::Update_DrawPos()
{
	motion->p_getScale().x = BGM71_ACL_X;
	motion->p_getScale().y = BGM71_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0 * DELTA()) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0 * DELTA()) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void Bgm71::render_Motion()
{
	BaseThermalTeam::render_Motion();
	motion->render();
}

void Bgm71::render_Ellipse()
{
}

void Bgm71::BGM71_Attack_Action(void * _this)
{
	Bgm71* object = (Bgm71*)_this;
}

void Bgm71::BGM71_Reload_Action(void * _this)
{
	Bgm71* object = (Bgm71*)_this;
}

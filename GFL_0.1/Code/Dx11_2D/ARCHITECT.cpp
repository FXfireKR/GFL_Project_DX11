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
			motion->setMotionAction("attack", Architect_Attack_Action);
			motion->setMotionObject(this);
			motion->init();
		}
	}

	Pos.x = 850.0f;
	Pos.y = 550.0f;

	mCollision.insert(make_pair("SELF", new EllipseBase(&Pos.x, &Pos.y, 30, 5)));
	mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&Pos.x, &Pos.y, axisMax_LongRad, axisMax_ShortRad)));


	moveSpd = ARCHITECT_SPEED;

	curState.HitPoint.max = curState.HitPoint.curr = 3000;
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

	alianceType = ARCHITECT_ALIANCE;

	return S_OK;
}

void ARCHITECT::release()
{
	TaticDoll::release();
}

void ARCHITECT::update()
{
	TaticDoll::update();

	update_Coltime();
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
	if (curState.HitPoint.curr < 1)
	{
		if (!motion->isCurrent("die"))
		{
			motion->changeMotion("die", false, true);
			isAlive = false;
		}
	}

	else
	{
		if (TargetID != -1)
		{
			if (motion->isCurrent("wait"))
				motion->changeMotion("attack", true, true, 0.125f);

			else if (motion->isCurrent("attack"))
			{
				// 0.075
				if (atkColTime > 0.0)
					motion->pause(-0.01);
			}
		}

		else
		{
			if (motion->isCurrent("attack"))
				motion->changeMotion("wait", false, true);
		}

		if (!motion->isCurrent("attack"))
			atkColTime = curState.AimDelay;
	}

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
	if (isAlive)
		Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - 60.0f), curState.HitPoint.curr, curState.HitPoint.max, DV2(100, 5), ColorF(RGB(150, 0, 0)));
}

void ARCHITECT::render_Motion()
{
	TaticDoll::render_Motion();
	motion->render();
}

void ARCHITECT::render_Ellipse()
{
	if (isAlive)
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.8, 0.1, 0.1, 0.9f));
}

void ARCHITECT::Architect_Attack_Action(void * _this)
{
	ARCHITECT* object = (ARCHITECT*)_this;

	if (object->atkColTime < FLOAT_EPSILON)
	{
		object->moveAble = false;

		spineMotion* motion = object->motion;
		float curTime = motion->getCurTime();
		static SINT safeTirgger = 0;				//	3점사 유지를 위한 안전장치

		//	1
		if (curTime < 0.19997f && curTime > 0.19997f - DELTA)
		{
			if (safeTirgger == 0)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++safeTirgger;
			}
		}

		//	2
		else if (curTime < 0.2666f && curTime > 0.2666f - DELTA)
		{
			if (safeTirgger == 1)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++safeTirgger;
			}
		}

		//	3
		else if (curTime < 0.42223f && curTime > 0.42223f - DELTA)
		{
			if (safeTirgger == 2)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++safeTirgger;
			}
		}

		else if (motion->getCurTime() > motion->getEndTime())
		{
			object->atkColTime = object->curState.AttackDelay;
			motion->changeMotion("attack", false, true, 0.15f);

			safeTirgger = 0;
			object->moveAble = true;
		}
	}
}

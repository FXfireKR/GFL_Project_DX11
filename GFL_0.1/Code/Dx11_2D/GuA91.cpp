#include "stdafx.h"
#include "GuA91.h"

GuA91::GuA91()
{
}

GuA91::~GuA91()
{
}

void GuA91::LoadTray_List()
{
	this->init();
}

HRESULT GuA91::init()
{
	TaticDoll::init();

	axisMax_LongRad = 400.0f;
	axisMax_ShortRad = 150.0f;

	if (motion == nullptr)
	{
		motion = new spineMotion;

		if (motion != nullptr)
		{
			motion->loadSpine_FromJsonFile("9A91");
			motion->setMotionAction("attack", GuA91_Attack_Action);
			motion->setMotionObject(this);
			motion->init();

			motion->changeMotion("attack", true, true, 0.125f);
			motion->changeMotion("die", true, true, 0.125f);
			motion->changeMotion("move", true, true, 0.125f);
			motion->changeMotion("wait", true, true, 0.125f);
		}
	}

	Pos.x = 450.0f;
	Pos.y = 200.0f;

	if (mCollision.count("SELF") == 0)
		mCollision.insert(make_pair("SELF", new EllipseBase(&Pos.x, &Pos.y, 30, 5)));

	if (mCollision.count("MAX_RANGE") == 0)
		mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&Pos.x, &Pos.y, axisMax_LongRad, axisMax_ShortRad)));

	moveSpd = GUA91_SPEED;

	curState.HitPoint.max = curState.HitPoint.curr = 3000;
	curState.Accuracy = 1.0;
	curState.CriticPoint = 25.5;
	curState.CriticAcl = 50;
	curState.AttackDelay = 0.8f;
	curState.AimDelay = 0.45;
	curState.Avoid = 0.835;
	curState.AttackPoint = 85;

	maxState = curState;

	atkColTime = curState.AimDelay;
	sklColTime = 0.0;
	TargetAngle = Angle = 0.0f;

	isAlive = true;
	Select = false;
	moveAble = true;

	alianceType = ALIANCE_GRIFFON;

	return S_OK;
}

void GuA91::release()
{
	TaticDoll::release();
}

void GuA91::update()
{
	TaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	this->MotionUpdate();

	motion->update(DELTA);
}

void GuA91::render()
{
	
}

void GuA91::Use_ActiveSkill()
{
}

void GuA91::MotionUpdate()
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

void GuA91::Update_DrawPos()
{
	motion->p_getScale().x = GUA91_ACL_X;
	motion->p_getScale().y = GUA91_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0f * DELTA) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0f * DELTA) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void GuA91::render_VisualBar()
{
	if (motion->isCurrent("attack"))
		Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - GUA91_DOWN_Y), curState.HitPoint.curr, curState.HitPoint.max, DV2(100, 5), ColorF(RGB(0, 200, 0)));

	else
		Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - GUA91_STAND_Y), curState.HitPoint.curr, curState.HitPoint.max, DV2(100, 5), ColorF(RGB(0, 200, 0)));
}

void GuA91::render_Motion()
{
	TaticDoll::render_Motion();
	motion->render();
}

void GuA91::render_Ellipse()
{
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.1, 0.8, 0.1, 0.9f));
}

void GuA91::GuA91_Attack_Action(void * _this)
{
	GuA91* object = (GuA91*)_this;



	if (object->atkColTime < FLOAT_EPSILON)
	{
		object->moveAble = false;

		spineMotion* motion = object->motion;
		float curTime = motion->getCurTime();

		//ImGui::Text("ak12_Time : %.3f", curTime);
	
		static SINT safeTirgger = 0;				//	3점사 유지를 위한 안전장치
	
		//	1
		if (curTime < 0.211f && curTime > 0.211f - DELTA)
		{
			if (safeTirgger == 0)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++safeTirgger;
			}
		}

		else if (curTime < 0.311f && curTime > 0.311f - DELTA)
		{
			if (safeTirgger == 1)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++safeTirgger;
			}
		}

		else if (curTime < 0.411f && curTime > 0.411f - DELTA)
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

#include "stdafx.h"
#include "AK12.h"

AK12::AK12()
{
}

AK12::~AK12()
{
}

void AK12::LoadTray_List()
{
	this->init();
}

HRESULT AK12::init()
{
	TaticDoll::init();

	axisMax_LongRad = 400.0f;
	axisMax_ShortRad = 150.0f;

	if (motion == nullptr)
	{
		motion = new spineMotion;

		if (motion != nullptr)
		{
			motion->loadSpine_FromJsonFile("ak12");
			motion->setMotionAction("attack", AK12_Attack_Action);
			motion->setMotionObject(this);
			motion->init();
		}
	}

	Pos.x = 150.0f;
	Pos.y = 200.0f;

	if (mCollision.count("SELF") == 0)
		mCollision.insert(make_pair("SELF", new EllipseBase(&Pos.x, &Pos.y, 30, 5)));

	if (mCollision.count("MAX_RANGE") == 0)
		mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&Pos.x, &Pos.y, axisMax_LongRad, axisMax_ShortRad)));
	
	moveSpd = AK12_SPEED;

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

	alianceType = ALIANCE_GRIFFON;

	return S_OK;
}

void AK12::release()
{
	TaticDoll::release();
}

void AK12::update()
{
	TaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	motion->update(DELTA);
	this->MotionUpdate();
}

void AK12::render()
{
}

void AK12::Use_ActiveSkill()
{
	if (sklColTime < D3DX_16F_EPSILON)
	{
		motion->changeMotion("skill", false, true, 0.125f);
		//	skillSound Export 
	}
}

void AK12::MotionUpdate()
{
	if (TargetID != -1)
	{
		if (motion->isCurrent("wait"))
			motion->changeMotion("attack", false, true, 0.125f);

		else if (motion->isCurrent("attack"))
		{
			if (atkColTime > 0.0)		
				motion->pause(-0.055);
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

void AK12::Update_DrawPos()
{
	motion->p_getScale().x = AK12_ACL_X;
	motion->p_getScale().y = AK12_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0 * DELTA) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0 * DELTA) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void AK12::render_VisualBar()
{
}

void AK12::render_Motion()
{
	TaticDoll::render_Motion();
	motion->render();
}

void AK12::render_Ellipse()
{
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.1, 0.8, 0.1, 0.9f));
}

void AK12::AK12_Attack_Action(void * _this)
{
	AK12* object = (AK12*)_this;

	if (object->atkColTime < FLOAT_EPSILON)
	{
		object->moveAble = false;

		spineMotion* motion = object->motion;
		float curTime = motion->getCurTime();
		static SINT safeTirgger = 0;				//	3점사 유지를 위한 안전장치

		//ImGui::Text("ak12_Time : %.3f", curTime);

		//	1
		if (curTime < 0.1333f && curTime > 0.1333f - DELTA)
		{
			if (safeTirgger == 0)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++safeTirgger;
			}
		}

		//	2
		else if (curTime < 0.19997f && curTime > 0.19997f - DELTA)
		{
			if (safeTirgger == 1)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++safeTirgger;
			}
		}

		//	3
		else if (curTime < 0.2666f && curTime > 0.2666f - DELTA)
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

void AK12::AK12_Skill_Action(void * _this)
{
	AK12* object = (AK12*)_this;
	spineMotion* motion = object->motion;
	float curTime = motion->getCurTime();

	if (motion->getCurTime() > motion->getEndTime())
	{
		object->atkColTime = object->curState.AttackDelay;
		motion->changeMotion("wait", false, true, 0.15f);
		object->moveAble = true;
	}
}
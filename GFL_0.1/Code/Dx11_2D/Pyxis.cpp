#include "stdafx.h"
#include "Pyxis.h"

Pyxis::Pyxis()
{
}

Pyxis::~Pyxis()
{
}

void Pyxis::LoadTray_SoundList()
{
}

void Pyxis::LoadTray_ImageList()
{
	this->init();
}

HRESULT Pyxis::init()
{
	TaticDoll::init();

	axisMax_LongRad = 400.0f;
	axisMax_ShortRad = 150.0f;
	axisMin_LongRad = axisMax_LongRad * 0.45f;
	axisMin_ShortRad = axisMax_ShortRad * 0.45f;

	if (motion == nullptr)
	{
		motion = new spineMotion;

		if (motion != nullptr)
		{
			motion->loadSpine_FromJsonFile("Pyxis");
			motion->setMotionAction("attack", Pyxis_Attack_Action);
			motion->setMotionAction("set", Pyxis_Set_Action);

			for (int i = 0; i < 3; ++i)
			{
				motion->changeMotion("die", false, true);
				motion->changeMotion("set", false, true);
				motion->changeMotion("attack", false, true);
				motion->changeMotion("wait", false, true);
				motion->changeMotion("pseudo_setup_pose", false, true);
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

	Pos.x = 800.0f;
	Pos.y = 500.0f;

	if (mCollision.count("SELF") == 0)
		mCollision.insert(make_pair("SELF", new EllipseBase(&Pos.x, &Pos.y, 30, 5)));

	if (mCollision.count("MAX_RANGE") == 0)
		mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&Pos.x, &Pos.y, axisMax_LongRad, axisMax_ShortRad)));

	moveSpd = 0.0;

	curState.HitPoint.max = curState.HitPoint.curr = 3000;
	curState.Accuracy = 999;
	curState.CriticPoint = 50.0;
	curState.CriticAcl = 50;
	curState.AttackDelay = 0.01f;
	curState.AimDelay = 0.15;
	curState.Avoid = 0;
	curState.AttackPoint = 95;

	maxState = curState;

	atkColTime = 0.0;
	sklColTime = 0.0;
	TargetAngle = Angle = 0.0f;

	isAlive = true;
	Select = false;
	moveAble = true;
	setComplete = false;

	safeTirgger = 0;

	alianceType = ALIANCE_IRONBLOD;

	return S_OK;
}

void Pyxis::release()
{
	TaticDoll::release();
}

void Pyxis::update()
{
	if (KEYMANAGER->isKeyDown('I'))
	{
		alianceType = ALIANCE_GRIFFON;
		TargetID = -1;
		vRange.clear();
	}

	TaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	this->MotionUpdate();
	motion->update(DELTA * DeltaAcl);
}

void Pyxis::render()
{
}

void Pyxis::Use_ActiveSkill()
{
}

void Pyxis::MotionUpdate()
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
			{
				if (!setComplete)
					motion->changeMotion("set", false);
			}

			else if (motion->isCurrent("attack"))
			{
				// 0.075
				if (atkColTime > 0.0)
					motion->pause(-0.035);
			}
		}

		else
		{
			if (motion->isCurrent("attack"))
			{
				motion->changeMotion("wait", true, true, 0.09f);
				safeTirgger = 0;
				setComplete = false;
			}
		}

		if (!motion->isCurrent("attack"))
			atkColTime = curState.AimDelay;
	}
}

void Pyxis::Update_DrawPos()
{
	motion->p_getScale().x = PYXIS_ACL_X;
	motion->p_getScale().y = PYXIS_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y > 0.0f ? 0.0f : 0.0f) :
		(motion->getRotate().y < 180.0f ? 180.0f : 180.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void Pyxis::render_VisualBar()
{
	if (isAlive)
	{
		elipseColor = D3DXCOLOR(0.8f, 0, 0, 0.2f);

		if (PLAYER->isAliance(alianceType))
			elipseColor = D3DXCOLOR(0.5f, 0.5f, 0, 0.2f);

		else if (alianceType == ALIANCE_GRIFFON)
			elipseColor = D3DXCOLOR(0, 0.8f, 0, 0.2f);

		Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - 150.0f), curState.HitPoint.curr,
			curState.HitPoint.max, DV2(100, 5), ColorF(elipseColor.r, elipseColor.g, elipseColor.b, elipseColor.a + 0.6f));
	}
}

void Pyxis::render_Motion()
{
	TaticDoll::render_Motion();
	motion->render();
}

void Pyxis::render_Ellipse()
{
	if (isAlive)
		for (auto& iterCollition : mCollision)
			iterCollition.second->Rend_Ellipse(D3DXCOLOR(elipseColor.r, elipseColor.g, elipseColor.b, 0.9f));
}

void Pyxis::Pyxis_Set_Action(void * _this)
{
	Pyxis* object = (Pyxis*)_this;

	spineMotion* motion = object->motion;
	float curTime = motion->getCurTime();

	if (motion->getCurTime() > motion->getEndTime())
	{
		if (object->TargetID != -1)
			motion->changeMotion("attack", false);

		else
			motion->changeMotion("wait", true, true);

		object->safeTirgger = 0;
		object->moveAble = true;

		if (!object->setComplete)
			object->setComplete = true;
	}
}

void Pyxis::Pyxis_Attack_Action(void* _this)
{
	Pyxis* object = (Pyxis*)_this;

	if (object->atkColTime < FLOAT_EPSILON)
	{
		object->moveAble = false;

		spineMotion* motion = object->motion;
		float curTime = motion->getCurTime();

		//	1
		if (curTime < 0.035f && curTime > 0.035f - DELTA)
		{
			if (object->safeTirgger == 0)
			{
				switch (rand()%4)
				{
				case 0:BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1800.0f);
					break;

				case 1:	BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 45, object->TargetID, object->curState, object->alianceType, 1800.0f);
					break;

				case 2:BULLET->CreateBullet("AR_BLT", object->Pos.x + 25, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1800.0f);
					break;

				case 3:BULLET->CreateBullet("AR_BLT", object->Pos.x + 25, object->Pos.y - 45, object->TargetID, object->curState, object->alianceType, 1800.0f);
					break;
				}	

				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);

				++object->safeTirgger;
			}
		}

		else if (motion->getCurTime() > motion->getEndTime())
		{
			object->atkColTime = object->curState.AttackDelay;

			if (object->TargetID != -1)
				motion->changeMotion("attack", false);
			else
				motion->changeMotion("wait", true, true, 0.1f);

			object->safeTirgger = 0;
			object->moveAble = true;
		}

	}
}
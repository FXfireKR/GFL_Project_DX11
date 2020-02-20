#include "stdafx.h"
#include "Strelet.h"

Strelet::Strelet()
{
}

Strelet::~Strelet()
{
}

void Strelet::LoadTray_SoundList()
{
}

void Strelet::LoadTray_ImageList()
{
	this->init();
}

HRESULT Strelet::init()
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
			motion->loadSpine_FromJsonFile("strelet");
			motion->setMotionAction("attack", Strelet_Attack_Action);

			motion->setMotionObject(this);
			motion->init();
		}
	}

	else
	{
		motion->reload_SpineImage();
		motion->init();
	}

	Pos.x = 600.0f;
	Pos.y = 500.0f;

	if (mCollision.count("SELF") == 0)
		mCollision.insert(make_pair("SELF", new EllipseBase(&Pos.x, &Pos.y, 30, 5)));

	if (mCollision.count("MAX_RANGE") == 0)
		mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&Pos.x, &Pos.y, axisMax_LongRad, axisMax_ShortRad)));

	moveSpd = STRELET_SPEED;

	curState.HitPoint.max = curState.HitPoint.curr = 1500;
	curState.Accuracy = 0.75;
	curState.CriticPoint = 25.5;
	curState.CriticAcl = 50;
	curState.AttackDelay = 0.45f;
	curState.AimDelay = 0.15;
	curState.Avoid = 0.125;
	curState.AttackPoint = 115;

	maxState = curState;

	atkColTime = 0.0;
	sklColTime = 0.0;
	TargetAngle = Angle = 0.0f;

	isAlive = true;
	Select = false;
	moveAble = true;

	safeTirgger = 0;

	alianceType = ALIANCE_PEREDEUS;

	return S_OK;
}

void Strelet::release()
{
	TaticDoll::release();
}

void Strelet::update()
{
	TaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	this->MotionUpdate();
	motion->update(DELTA * DeltaAcl);
}

void Strelet::render(){}

void Strelet::Use_ActiveSkill()
{
}

void Strelet::MotionUpdate()
{
	//static float mixTime = 0.0f;
	//ImGui::DragFloat("MixTime", &mixTime, 0.01F, 0.0F);

	if (curState.HitPoint.curr < 1)
	{
		if (!motion->isCurrent("die"))
		{
			motion->changeMotion("die", false, true);
			isAlive = false;
			TargetID = -1;
		}
	}

	else
	{
		if (TargetID != -1)
		{
			if (motion->isCurrent("wait"))
				motion->changeMotion("attack", true, true);

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
				motion->changeMotion("wait", false, true);
				safeTirgger = 0;
			}
		}

		if (!motion->isCurrent("attack"))
			atkColTime = curState.AimDelay;
	}

}

void Strelet::Update_DrawPos()
{
	motion->p_getScale().x = STRELET_ACL_X;
	motion->p_getScale().y = STRELET_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (500.0 * DELTA) : 0.0f) :
		(motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (500.0 * DELTA) : 180.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void Strelet::render_VisualBar()
{
	if (isAlive)
	{
		elipseColor = D3DXCOLOR(0.8f, 0, 0, 0.2f);

		if (PLAYER->isAliance(alianceType))
			elipseColor = D3DXCOLOR(0.5f, 0.5f, 0, 0.2f);

		Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - 150.0f), curState.HitPoint.curr,
			curState.HitPoint.max, DV2(100, 5), ColorF(elipseColor.r, elipseColor.g, elipseColor.b, elipseColor.a + 0.6f));
	}
}

void Strelet::render_Motion()
{
	TaticDoll::render_Motion();
	motion->render();
}

void Strelet::render_Ellipse()
{
	if (isAlive)
		for (auto& iterCollition : mCollision)
			iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.8, 0.1, 0.1, 0.9f));
}

void Strelet::Strelet_Attack_Action(void * _this)
{
	Strelet* object = (Strelet*)_this;

	if (object->atkColTime < FLOAT_EPSILON)
	{
		object->moveAble = false;

		spineMotion* motion = object->motion;
		float curTime = motion->getCurTime();

		//	1
		if (curTime < 0.1333f && curTime > 0.1333f - DELTA)
		{
			if (object->safeTirgger == 0)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++object->safeTirgger;
			}
		}

		//	2
		else if (curTime < 0.19997f && curTime > 0.19997f - DELTA)
		{
			if (object->safeTirgger == 1)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++object->safeTirgger;
			}
		}

		//	3
		else if (curTime < 0.2666f && curTime > 0.2666f - DELTA)
		{
			if (object->safeTirgger == 2)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++object->safeTirgger;
			}
		}

		else if (motion->getCurTime() > motion->getEndTime())
		{
			object->atkColTime = object->curState.AttackDelay;

			if (object->TargetID != -1)
				motion->changeMotion("attack", false, true, 0.001f);
			else
				motion->changeMotion("wait", true, true);

			object->safeTirgger = 0;
			object->moveAble = true;
		}

	}
}
#include "stdafx.h"
#include "pkp.h"

pkp::pkp()
{
}

pkp::~pkp()
{
}

void pkp::LoadTray_List()
{
	this->init();
}

HRESULT pkp::init()
{
	TaticDoll::init();

	axisMax_LongRad = 400.0f;
	axisMax_ShortRad = 150.0f;

	if (motion == nullptr)
	{
		motion = new spineMotion;

		if (motion != nullptr)
		{
			motion->loadSpine_FromJsonFile("pkp");
			motion->setMotionAction("attack", pkp_Attack_Action);
			motion->setMotionAction("reload", pkp_Reload_Action);
			motion->setMotionObject(this);
			motion->init();
		}
	}

	Pos.x = 250.0f;
	Pos.y = 200.0f;

	if (mCollision.count("SELF") == 0)
		mCollision.insert(make_pair("SELF", new EllipseBase(&Pos.x, &Pos.y, 30, 5)));

	if (mCollision.count("MAX_RANGE") == 0)
		mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&Pos.x, &Pos.y, axisMax_LongRad, axisMax_ShortRad)));

	moveSpd = PKP_SPEED;

	curState.HitPoint.max = curState.HitPoint.curr = 3000;
	curState.Accuracy = 1.0;
	curState.CriticPoint = 25.5;
	curState.CriticAcl = 50;
	curState.AttackDelay = 0;
	curState.AimDelay = 0.45;
	curState.Avoid = 0.835;
	curState.AttackPoint = 85;

	maxState = curState;

	mAmmo = PKP_MAX_AMMO;

	atkColTime = curState.AimDelay;
	sklColTime = 0.0;
	TargetAngle = Angle = 0.0f;

	isAlive = true;
	Select = false;
	moveAble = true;

	alianceType = ALIANCE_GRIFFON;

	return S_OK;
}

void pkp::release()
{
	TaticDoll::release();
}

void pkp::update()
{
	TaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	this->MotionUpdate();

	motion->update(DELTA);
}

void pkp::render()
{
}

void pkp::Use_ActiveSkill()
{
}

void pkp::MotionUpdate()
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

	if (!motion->isCurrent("attack"))
		atkColTime = curState.AimDelay;
}

void pkp::Update_DrawPos()
{
	motion->p_getScale().x = PKP_ACL_X;
	motion->p_getScale().y = PKP_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0f * DELTA) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0f * DELTA) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void pkp::render_VisualBar()
{
}

void pkp::render_Motion()
{
	TaticDoll::render_Motion();
	motion->render();
}

void pkp::render_Ellipse()
{
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.1, 0.8, 0.1, 0.9f));
}

void pkp::pkp_Attack_Action(void * _this)
{
	pkp* object = (pkp*)_this;

	//static float shotcount[3] = { 0.0f, };
	//ImGui::DragFloat3("ShotTime", shotcount, 0.01f);


	if (object->atkColTime < FLOAT_EPSILON)
	{
		spineMotion* motion = object->motion;
		float curTime = motion->getCurTime();

		static SINT safeTirgger = 0;				//	3점사 유지를 위한 안전장치

		//for (int i = 0; i < 3; ++i)
		//{
		//	if (curTime < shotcount[i] && curTime > shotcount[i] - DELTA)
		//	{
		//		if (safeTirgger == i)
		//		{
		//			BULLET->CreateBullet("MG_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
		//			SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "mgSound2", 0.05f);
		//			++safeTirgger;
		//			continue;
		//		}
		//	}
		//}

		if (object->mAmmo > 0)
		{
			//	1
			if (curTime < 0.018f && curTime > 0.018f - DELTA)
			{
				if (safeTirgger == 0)
				{
					BULLET->CreateBullet("MG_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
					SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "mgSound2", 0.05f);
					++safeTirgger;
					--object->mAmmo;
				}
			}

			//	2
			else if (curTime < 0.118f && curTime > 0.118f - DELTA)
			{
				if (safeTirgger == 1)
				{
					BULLET->CreateBullet("MG_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
					SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "mgSound2", 0.05f);
					++safeTirgger;
					--object->mAmmo;
				}
			}

			//	3
			else if (curTime < 0.218f && curTime > 0.218f - DELTA)
			{
				if (safeTirgger == 2)
				{
					BULLET->CreateBullet("MG_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
					SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "mgSound2", 0.05f);
					++safeTirgger;
					--object->mAmmo;
				}
			}

			else if (curTime > motion->getEndTime())
			{
				object->atkColTime = object->curState.AttackDelay;
				motion->changeMotion("attack", false);

				safeTirgger = 0;
			}
		}

		else
		{
			object->atkColTime = object->curState.AimDelay;
			motion->changeMotion("reload", true, true, 0.125f);

			safeTirgger = 0;
		}

	}
}

void pkp::pkp_Reload_Action(void * _this)
{
	pkp* object = (pkp*)_this;
	spineMotion* motion = object->motion;
	float curTime = motion->getCurTime();

	if (curTime > motion->getEndTime())
	{
		object->atkColTime = object->curState.AttackDelay;
		object->mAmmo = object->PKP_MAX_AMMO;
		motion->changeMotion("wait", true, true, 0.125f);
	}
}

#include "stdafx.h"
#include "Drone.h"

Drone::Drone()
{
	name = "drone";
}

Drone::~Drone()
{
}

void Drone::LoadImageList()
{
	spineMotion::loadImage_FromFile("drone");
}

void Drone::LoadTray_SoundList()
{
}

void Drone::LoadTray_ImageList()
{
	this->init();
}

HRESULT Drone::init()
{
	TaticDoll::init();

	if (motion == nullptr)
	{
		motion = new spineMotion;

		if (motion != nullptr)
		{
			motion->loadSpine_FromJsonFile("drone");
			motion->setMotionObject(this);
			motion->init();
		}
	}

	Pos.x = 150.0f;
	Pos.y = 200.0f;

	if (mCollision.count("SELF") == 0)
		mCollision.insert(make_pair("SELF", new EllipseBase(&Pos.x, &Pos.y, 45, 10)));

	//moveSpd = NTW20_SPEED;

	curState.HitPoint.max = curState.HitPoint.curr = 150;
	curState.Accuracy = 1.0;
	curState.CriticPoint = 25.5;
	curState.CriticAcl = 50;
	curState.AttackDelay = 0.8f;
	curState.AimDelay = 0.45;
	curState.Avoid = 0.0001f;
	curState.AttackPoint = 0;

	maxState = curState;

	atkColTime = 0.0;
	sklColTime = 0.0;
	TargetAngle = Angle = 0.0f;

	isAlive = true;
	Select = false;
	moveAble = true;

	alianceType = ALIANCE_NONE;

	return S_OK;
}

void Drone::release()
{
	TaticDoll::release();
}

void Drone::update()
{
	TaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	motion->update(DELTA * DeltaAcl);
	this->MotionUpdate();
}

void Drone::render()
{
}

void Drone::Use_ActiveSkill()
{
}

void Drone::MotionUpdate()
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

void Drone::Update_DrawPos()
{
	motion->p_getScale().x = DRONE_ACL_X;
	motion->p_getScale().y = DRONE_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0 * DELTA) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0 * DELTA) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void Drone::render_VisualBar()
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

void Drone::render_Motion()
{
	TaticDoll::render_Motion();
	motion->render();
}

void Drone::render_Ellipse()
{
	if (isAlive)
		for (auto& iterCollition : mCollision)
			iterCollition.second->Rend_Ellipse(elipseColor);
}
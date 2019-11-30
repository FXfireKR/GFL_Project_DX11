#include "stdafx.h"
#include "Jaeger.h"

Jaeger::Jaeger()
{
	motion = new MotionManager();
}

Jaeger::~Jaeger()
{
}

void Jaeger::LoadTray()
{
	//SOUND RESOURCE
	LOADMANAGER->Add_LoadTray("FIRE_SR1", "../../_SoundSource/Battle_181.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	LOADMANAGER->Add_LoadTray("JAEGER_ATTACK", "../../_Textures/JAEGER/Attack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("JAEGER_IDLE", "../../_Textures/JAEGER/Idle.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("JAEGER_WALK", "../../_Textures/JAEGER/Walk.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
}

void Jaeger::LoadTray_List()
{
	LOADMANAGER->Add_LoadTray("JAEGER_ATTACK", "../../_Textures/JAEGER/Attack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("JAEGER_IDLE", "../../_Textures/JAEGER/Idle.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("JAEGER_WALK", "../../_Textures/JAEGER/Walk.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
}

HRESULT Jaeger::init()
{
	SNVtacDoll::init();

	motion->Create("IDLE", "JAEGER_IDLE", 390, 340, 0.025F);
	motion->Create("WALK", "JAEGER_WALK", 390, 340, 0.001F);
	motion->Create("ATTACK", "JAEGER_ATTACK", 390, 340, 0.01F);

	//motion->Create("SKILL", "../RemakeResource/Enemy/Hydra/Skill.png", 300, 250, 1);
	//motion->Create("DEATH", "../RemakeResource/Enemy/Hydra/Death.png", 300, 250, 3);

	motion->RegisterFunction("ATTACK", Jaeger_AttackAction, this);
	//motion->RegisterFunction("SKILL", HYDRA_SkillAction, this);
	//motion->RegisterFunction("DEATH", HYDRA_DeathAction, this);

	motion->Change_Motion("IDLE");

	cPos.x = 1200.0f + (rand() % 400);
	cPos.y = 500.0f + (rand() % 100);

	mCollision.insert(make_pair("SELF", new EllipseBase(&cPos.x, &cPos.y, 28, 6)));
	mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&cPos.x, &cPos.y, 500, 180)));

	name = "JAEGER";
	Speed = JAEGER_SPEED;

	//	9000
	state.HeatPoint.max = state.HeatPoint.current = 1800;
	state.HitPoint = 85.0f;
	state.CriticPoint = 25.5;
	state.CriticAcl = 50;
	state.AttackDelay = 1.25;
	state.AimDelay = 2.55;
	state.AttackPoint = 280;	//280
	state.AvoidPoint = 0.0f;

	SklColTime = 0.0f;

	maxState = state;

	Angle = 3.14f;

	smartTarget = -1;
	smartCounter = HYDRA_SMART_AI_FINDER_TIME;

	return S_OK;
}

void Jaeger::release()
{
	SNVtacDoll::release();
}

void Jaeger::update()
{
	SNVtacDoll::update();

	motion->update();

	//Position Check / ReCalculate
	{
		dPos.x = cPos.x + (!Flip ? 3.0f : -3.0f);
		dPos.y = cPos.y - 47.5f;

		cr = RectMake(dPos.x + (93.0f*JAEGER_ACLRENDER), dPos.y + (49.0f*JAEGER_ACLRENDER),
			(107.0f*JAEGER_ACLRENDER), (149.0f*JAEGER_ACLRENDER));
	}
}

void Jaeger::MotionUpdate()
{
	if (isAlive)
	{
		if (Target_ID != -1)
		{
			smartTarget = Target_ID;

			if (motion->isCurrent("IDLE"))
				motion->Change_Motion("ATTACK");
			//공격모션일시
			if (motion->isCurrent("ATTACK"))
			{
				if (AtkColTime > 0.0f)	//공격 모션시 공격쿨타임이 다 되지않으면, 조준하고 대기한다
				{
					motion->getCurrentMotion()->setFrameX(0);
					motion->pause();
				}
			}
		}

		else
		{
			if (motion->isCurrent("ATTACK"))
				motion->Change_Motion("IDLE");
		}


		//대기중, 걷기일때는 에임딜레이로 계산
		if (motion->isCurrent("IDLE") || motion->isCurrent("WALK"))
			AtkColTime = state.AimDelay + state.AttackDelay;


		if (smartTarget != -1)
		{
			if (targTeam == GRIFFON_TACDOLL)
			{
				if (smartTarget < 100 ? !PLAYER->getIOPdoll_crntSquad(smartTarget)->getAlive() : !BDATA->getEnemyMember(BDATA->getEngageSquadID(), smartTarget - 100)->getAlive())
					smartTarget = -1;
			}

			else
			{
				if (!BDATA->getEnemyMember(BDATA->getEngageSquadID(), smartTarget)->getAlive())
					smartTarget = -1;
			}
		}
	}

	//뒤졋으면
	else
	{
		if (!motion->isCurrent("DEATH"))
			motion->Change_Motion("DEATH");

	}
}

void Jaeger::render_VisualBar()
{
}

void Jaeger::render_Motion()
{
	motion->render(D3DXVECTOR2(dPos.x - VPOS->x, dPos.y - VPOS->y), D3DXVECTOR2(JAEGER_ACLRENDER, JAEGER_ACLRENDER), _color, Flip ? 180.0f : 0.0f);
}

void Jaeger::render_Ellipse()
{
	SNVtacDoll::render_Ellipse();
}

void Jaeger::UseSkill()
{
}

void Jaeger::Action_Algorydm()
{
}

void Jaeger::Jaeger_AttackAction(void * ob)
{
	Jaeger* Object = (Jaeger*)ob;
	int frame = Object->getMotionManager().getCurrent_Frame().x;

	switch (frame)
	{
	case 3:
		if (Object->getMotionManager().getCurrentMotion()->getIsOnce())
		{
			auto pos = Object->getCharacterPos();

			if (!Object->getFlip())
			{
				pos.x -= 50.0f;// +VPOS->x;
				pos.y -= 45.0f;
			}

			else
			{
				pos.x += 90.0f; //+ VPOS->x;
				pos.y -= 50.0f;
			}

			SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "FIRE_SR1", 0.1f);
			BEFCT->CreateBullet("RIFLE_BLT", pos, Object->getTargetID(), Object->getStatus(), BULLET_TYPE::SNV_BLT, 2500.0F);
		}
		break;
	}

	if (Object->getMotionManager().getCurrentMotion()->getIsOver())
	{
		(*(Object->getAttackColTime())) = (float)Object->getStatus().AttackDelay;
		Object->getMotionManager().getCurrentMotion()->Reset_Motion();
	}
}

void Jaeger::Jaeger_SkillAction(void * ob)
{
}

void Jaeger::Jaeger_DeathAction(void * ob)
{
}

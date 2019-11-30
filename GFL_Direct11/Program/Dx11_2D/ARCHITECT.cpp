#include "stdafx.h"
#include "ARCHITECT.h"

ARCHITECT::ARCHITECT()
{
	motion = new MotionManager;
	hpBar = new VisualBar;
}

ARCHITECT::~ARCHITECT()
{
}

void ARCHITECT::LoadTray_List()
{
	LOADMANAGER->Add_LoadTray("ARCHI_YUTAN_1", "../../_Textures/missle.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOADMANAGER->Add_LoadTray("ARCHITECT_IDLE", "../../_Textures/ARCHITECT/Idle.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("ARCHITECT_WALK", "../../_Textures/ARCHITECT/Walk.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("ARCHITECT_ATTACK", "../../_Textures/ARCHITECT/Attack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("ARCHITECT_SATTACK", "../../_Textures/ARCHITECT/sAttack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	//LOADMANAGER->Add_LoadTray("ARCHITECT_PRESKILL", "../../_Textures/ARCHITECT/PreSkill.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	//LOADMANAGER->Add_LoadTray("ARCHITECT_SKILL1", "../../_Textures/ARCHITECT/SkillActive_1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	//LOADMANAGER->Add_LoadTray("ARCHITECT_SKILL2", "../../_Textures/ARCHITECT/SkillActive_2.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	//LOADMANAGER->Add_LoadTray("ARCHITECT_SKILL3", "../../_Textures/ARCHITECT/SkillActive_3.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("ARCHITECT_SKILL4", "../../_Textures/ARCHITECT/aaa.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	//LOADMANAGER->Add_LoadTray("FNFAL_SKILL", "../../_Textures/FNFAL/Skill.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	//LOADMANAGER->Add_LoadTray("FNFAL_DEATH", "../../_Textures/FNFAL/Death.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
}

HRESULT ARCHITECT::init()
{
	IOPtaticDoll::init();

	hpBar->Create_VisualBar(&(state.HeatPoint.current), &(state.HeatPoint.max), 100, 5);

	motion->Create("IDLE", "ARCHITECT_IDLE", 390, 340, 0.025F);
	motion->Create("WALK", "ARCHITECT_WALK", 390, 340, 0.001F);
	motion->Create("ATTACK", "ARCHITECT_ATTACK", 390, 340, 0.0001F);
	motion->Create("SATTACK", "ARCHITECT_SATTACK", 390, 340, 0.02F);

	motion->Create("SKILL1", "ARCHITECT_SKILL1", 720, 640, 0.02F);
	motion->Create("SKILL2", "ARCHITECT_SKILL2", 720, 640, 0.02F);
	motion->Create("SKILL3", "ARCHITECT_SKILL3", 720, 640, 0.02F);
	motion->Create("SKILL4", "ARCHITECT_SKILL4", 720, 640, 0.02F);

	motion->RegisterFunction("ATTACK", ARCHITECT_AttackAction, this);
	motion->RegisterFunction("SATTACK", ARCHITECT_sAttackAction, this);

	motion->RegisterFunction("SKILL1", ARCHITECT_Skill1_Action, this);
	motion->RegisterFunction("SKILL2", ARCHITECT_Skill2_Action, this);
	motion->RegisterFunction("SKILL3", ARCHITECT_Skill3_Action, this);
	motion->RegisterFunction("SKILL4", ARCHITECT_Skill4_Action, this);

	motion->Change_Motion("IDLE");

	mCollision.insert(make_pair("SELF", new EllipseBase(&cPos.x, &cPos.y, 20, 4)));
	mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&cPos.x, &cPos.y, 400, 150)));
	//mCollision.insert(make_pair("SKILL_RANGE", new EllipseBase(&cPos.x, &cPos.y, 800, 350)));

	EFFECT->Create_Sprite("EXPLO1", 120, 150);

	cPos.x = 250.0f;
	cPos.y = 500.0f;

	Speed = ARCHITECT_SPEED;

	for (int i = 0; i < ARCHITECT_YUTAN_MAX; ++i)
	{
		UtanInfo* info = new UtanInfo;
		info->utan = new SmokeBase;
		info->utan->init();
		i ? info->utan->Insert_SmokeImage("ARCHI_YUTAN_1") : info->utan->Insert_SmokeImage("ARCHI_YUTAN_1");

		info->u = new EllipseBase(&info->sklPos.x, &info->sklPos.y, ARCHITECT_SATTACK_LRAD, ARCHITECT_SATTACK_SRAD);

		vUtan.push_back(info);
	}

	//state.HeatPoint.max = state.HeatPoint.current = 9999999;
	state.HeatPoint.max = state.HeatPoint.current = 3000;
	state.HitPoint = 1.0;
	state.CriticPoint = 25.5;
	state.CriticAcl = 50;
	state.AttackDelay = 0.4;
	state.AimDelay = 0.45;
	state.AvoidPoint = 83.5;
	state.AttackPoint = 30;

	maxState = state;

	SklColTime = 0.0f;

	Angle = 0.0f;

	isAlive = true;
	Select = false;
	moveAble = true;

	return S_OK;
}

void ARCHITECT::release()
{
	IOPtaticDoll::release();

	for (auto& it : vUtan)
	{
		if (it != nullptr)
		{
			if (it->utan != nullptr)
			{
				it->utan->release();
				SAFE_DEL(it->utan);
			}

			if (it->u != nullptr)
			{
				it->u->Release_Ellipse();
				SAFE_DEL(it->u);
			}

			SAFE_DEL(it);
		}
	}
}

void ARCHITECT::update()
{
	IOPtaticDoll::update();

	motion->update();

	for (auto& it : vUtan)
	{
		if (it->utan->getFired())
		{
			it->utan->update();
			it->u->Update_Ellipse();
		}

		if (it->utan->getArrived())
		{
			Status st;
			st = state;
			st.AttackPoint = state.AttackPoint * 2;
			st.HitPoint = 9999.99f;

			FLOAT acl = 1.5F;
			//EFFECT->Create_("explo", sklPos.x - (60 * acl), sklPos.y - (135 * acl), 1, acl);

			int eSquad = BDATA->getEngageSquadID();

			for (int i = 0; i < BDATA->getEnemySquad(eSquad)->mSquad.size(); ++i)
			{
				if (myID.SquadMem_ID == i) continue;
				if (BDATA->getEnemyMember(eSquad, i + 1)->getAlive())
				{
					if (it->u->PointCollision_Check(BDATA->getEnemyMember(eSquad, i + 1)->getCharacterPos().x, BDATA->getEnemyMember(eSquad, i + 1)->getCharacterPos().y))
						BDATA->getEnemyMember(eSquad, i + 1)->Character_GetDamage(st);
				}
			}

			EFFECT->Create_("EXPLO1", it->sklPos.x, it->sklPos.y - 125, 0.01F, 2.5F);
			SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "EXPLO_YUTAN", 0.18f);
			it->utan->reset();
		}
	}

	//DrawPosition ReCheck
	this->Camera_Focus();
	this->Update_DrawPos();
}

void ARCHITECT::render()
{
	IOPtaticDoll::render();
}

void ARCHITECT::Use_ActiveSkill()
{
	motion->Change_Motion("SKILL1");
}

void ARCHITECT::MotionUpdate()
{
	if (isAlive)
	{
		if (Target_ID != -1)
		{
			if (motion->isCurrent("IDLE"))
				motion->Change_Motion("ATTACK");

			//공격모션일시
			if (motion->isCurrent("ATTACK") || motion->isCurrent("SATTACK"))
			{
				if (AtkColTime > 0.0f)	//공격 모션시 공격쿨타임이 다 되지않으면, 조준하고 대기한다
				{
					motion->getCurrentMotion()->setFrameX(0);
					motion->pause();
				}
			}

			else if (motion->isCurrent("SKILL"))
			{
				if (AtkColTime > 0.0f)
				{
					motion->getCurrentMotion()->setFrameX(0);
					motion->pause();
				}
			}
		}

		else
		{
			if (motion->isCurrent("ATTACK")) //|| motion->isCurrent("SATTACK"))
				motion->Change_Motion("IDLE");
		}

		//대기중, 걷기일때는 에임딜레이로 계산
		if (motion->isCurrent("IDLE") || motion->isCurrent("WALK"))
			AtkColTime = state.AimDelay + state.AttackDelay;

	}

	else
	{
		//뒤졋으면
		if (!motion->isCurrent("DEATH"))
		{
			moveAble = false;
			StopMoving();
			//motion->Change_Motion("DEATH");
			motion->getCurrentMotion()->setFrameX(0);
			//sound->Play_Effect("Death", 0.12f);
		}

	}
}

void ARCHITECT::Update_DrawPos()
{
	dPos.x = cPos.x;
	dPos.y = cPos.y - 75.0f;
	cr = RectMake(dPos.x + (93.0f*FNFAL_ACLRENDER), dPos.y + (49.0f*FNFAL_ACLRENDER),
		(107.0f*FNFAL_ACLRENDER), (149.0f*FNFAL_ACLRENDER));
}

void ARCHITECT::render_VisualBar()
{
	if (Select)
		hpBar->Render_VisualBar(cPos.x - 50.0F, cPos.y - 140.0F, ColorF(0.0F, 0.85F, 0.0F, 0.4F));
	else
		hpBar->Render_VisualBar(cPos.x - 50.0F, cPos.y - 140.0F, ColorF(0.0F, 0.85F, 0.0F, 0.2F));
}

void ARCHITECT::render_Motion()
{
	motion->render(D3DXVECTOR2(dPos.x - VPOS->x, dPos.y - VPOS->y), D3DXVECTOR2(FNFAL_ACLRENDER, FNFAL_ACLRENDER), 
		_color, 180.0f - DgreeAngle(Angle));
}

void ARCHITECT::render_Ellipse()
{
	IOPtaticDoll::render_Ellipse();


	for (auto& it : vUtan) {
		if (it->utan->getFired())
		{
			it->utan->render(0.7f);
			it->u->Rend_Ellipse(D3DXCOLOR(0.0F, 1.0F, 0.0F, 0.5F));
		}
	}
}

void ARCHITECT::ARCHITECT_AttackAction(void * ob)
{
	ARCHITECT* Object = (ARCHITECT*)ob;
	int frame = Object->getMotionManager().getCurrent_Frame().x;

	switch (frame)
	{
	case 1: case 5: case 9: case 13: 
		if (Object->getMotionManager().getCurrentMotion()->getIsOnce())
		{
			auto pos = Object->getCharacterPos();

			if (!Object->getFlip())
			{
				pos.x += 55.0f;// +VPOS->x;
				pos.y -= 50.0f;
			}

			else
			{
				pos.x -= 57.0f; //+ VPOS->x;
				pos.y -= 53.0f;
			}

			SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "FIRE_AR1", 0.1f);
			//"ARIFLE_BLT", pos, a, 1500.0f, Object->getStatus(), BULLET_TYPE::GRF_BLT, 1000.0F
			//"RF_BLT", p, Object->getTargetID(), Object->getStatus(), GRF_BLT, 50.0f
			BEFCT->CreateBullet("ARIFLE_BLT", pos, Object->getTargetID(), Object->getStatus(), BULLET_TYPE::GRF_BLT, 2500.0F);
		}
		break;
	}

	if (Object->getMotionManager().getCurrentMotion()->getIsOver())
	{
		(*(Object->getAttackColTime())) = (float)Object->getStatus().AttackDelay;

		//if (rand() % 100 < 1)
			Object->getMotionManager().getCurrentMotion()->Reset_Motion();

		//else
			Object->getMotionManager().Change_Motion("SATTACK");
	}
}

void ARCHITECT::ARCHITECT_sAttackAction(void * ob)
{
	ARCHITECT* Object = (ARCHITECT*)ob;
	int frame = Object->getMotionManager().getCurrent_Frame().x;

	switch (frame)
	{
	case 0:
		Object->setMoveAble(false);
		break;

	case 5:
		if (Object->getMotionManager().getCurrentMotion()->getIsOnce())
		{
			auto Yutan = Object->getYutanInfo(0);
			auto enemy = BDATA->getCur_EnemyMember(Object->getTargetID());

			Yutan->sklPos.x = enemy->getCharacterPos().x;
			Yutan->sklPos.y = enemy->getCharacterPos().y;
			Yutan->utan->setBulletSpeed(1200.0f);

			if (Object->getLookAngle() < RadianAngle(90.1f) || Object->getLookAngle() > RadianAngle(270.0f))
			{
				Object->getYutan(0)->Create_Smoke(Object->getCharacterPos().x + 15.0f, Object->getCharacterPos().y - 32.5f,
					Object->getSklPos(0).x, Object->getSklPos(0).y, 25.0f, 80.0f);
			}

			else
			{
				Object->getYutan(0)->Create_Smoke(Object->getCharacterPos().x - 15.0f, Object->getCharacterPos().y - 32.5f,
					Object->getSklPos(0).x, Object->getSklPos(0).y, 150.0f, 80.0f);
			}

			SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "FIRE_YUTAN", 0.08f);

			/*for (auto& it : vUtan)
			{
				it->sklPos.x = AxisPos.x + (rand() % 100) - 50.0f;
				it->sklPos.y = AxisPos.y + (rand() % 50) - 25.0f;

				it->utan->setBulletSpeed(1200.0f);
			}*/
		}		
		break;

	case 15:
		if (Object->getMotionManager().getCurrentMotion()->getIsOnce())
			Object->motion->getCurrentMotion()->setFramePerSec(0.05F);
		
		break;
	}

	if (Object->getMotionManager().getCurrentMotion()->getIsOver())
	{
		Object->motion->getCurrentMotion()->setFramePerSec(0.02F);
		Object->setMoveAble(true);

		(*(Object->getAttackColTime())) = (float)Object->getStatus().AttackDelay;

		Object->getMotionManager().getCurrentMotion()->Reset_Motion();
		Object->getMotionManager().Change_Motion("ATTACK");			
	}
}

void ARCHITECT::ARCHITECT_Skill1_Action(void * ob)
{
	ARCHITECT* Object = (ARCHITECT*)ob;
	int frame = Object->getMotionManager().getCurrent_Frame().x;

	if (Object->getMotionManager().getCurrentMotion()->getIsOver())
	{
		Object->getMotionManager().getCurrentMotion()->Reset_Motion();
		Object->getMotionManager().Change_Motion("SKILL2");
	}
}

void ARCHITECT::ARCHITECT_Skill2_Action(void * ob)
{
	ARCHITECT* Object = (ARCHITECT*)ob;
	int frame = Object->getMotionManager().getCurrent_Frame().x;

	if (Object->getMotionManager().getCurrentMotion()->getIsOver())
	{
		Object->getMotionManager().getCurrentMotion()->Reset_Motion();
		Object->getMotionManager().Change_Motion("SKILL3");
	}
}

void ARCHITECT::ARCHITECT_Skill3_Action(void * ob)
{
	ARCHITECT* Object = (ARCHITECT*)ob;
	int frame = Object->getMotionManager().getCurrent_Frame().x;

	if (Object->getMotionManager().getCurrentMotion()->getIsOver())
	{
		Object->getMotionManager().getCurrentMotion()->Reset_Motion();
		Object->getMotionManager().Change_Motion("SKILL4");
	}
}

void ARCHITECT::ARCHITECT_Skill4_Action(void * ob)
{
	ARCHITECT* Object = (ARCHITECT*)ob;
	int frame = Object->getMotionManager().getCurrent_Frame().x;

	if (Object->getMotionManager().getCurrentMotion()->getIsOver())
	{
		Object->getMotionManager().getCurrentMotion()->Reset_Motion();
		Object->getMotionManager().Change_Motion("IDLE");
	}
}

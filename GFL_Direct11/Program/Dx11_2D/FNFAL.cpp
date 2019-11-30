#include "stdafx.h"
#include "FNFAL.h"

FNFAL::FNFAL()
{
	motion = new MotionManager;
	hpBar = new VisualBar;
}

FNFAL::~FNFAL()
{
}

void FNFAL::LoadTray_List()
{
	LOADMANAGER->Add_LoadTray("EXPLO_YUTAN", "../../_SoundSource/Battle_114.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray("FIRE_YUTAN", "../../_SoundSource/Battle_115.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray("FIRE_AR1", "../../_SoundSource/Battle_AR.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	//LOADMANAGER->Add_LoadTray("FIRE_AR1", "../../_SoundSource/Battle_AR.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOADMANAGER->Add_LoadTray("YUTAN", "../../_Textures/Yutan.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOADMANAGER->Add_LoadTray("FNFAL_IDLE", "../../_Textures/FNFAL/Idle.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("FNFAL_WALK", "../../_Textures/FNFAL/Walk.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("FNFAL_ATTACK", "../../_Textures/FNFAL/Attack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("FNFAL_SKILL", "../../_Textures/FNFAL/Skill.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("FNFAL_DEATH", "../../_Textures/FNFAL/Death.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);


}

HRESULT FNFAL::init()
{
	IOPtaticDoll::init();

	hpBar->Create_VisualBar(&(state.HeatPoint.current), &(state.HeatPoint.max), 100, 5);

	motion->Create("IDLE", "FNFAL_IDLE", 390, 340, 0.025F);
	motion->Create("WALK", "FNFAL_WALK", 390, 340, 0.0001F);
	//motion->getMotion("WALK")->setCountGrav(200.0F);

	motion->Create("ATTACK", "FNFAL_ATTACK", 390, 340, 0.00001F);
	motion->Create("DEATH", "FNFAL_DEATH", 390, 340, 0.025F);

	motion->Create("SKILL", "FNFAL_SKILL", 390, 340, 0.015F);	
	//motion->Create("VICTORY", "NTW20_VICTORY", 390, 340, 0.015F);
	//motion->Create("VICLOOP", "NTW20_VICLOOP", 390, 340, 0.015F);

	motion->RegisterFunction("ATTACK", FNFAL_AttackAction, this);
	motion->RegisterFunction("DEATH", FNFAL_DeathAction, this);
	motion->RegisterFunction("SKILL", FNFAL_SkillAction, this);

	motion->Change_Motion("IDLE");


	mCollision.insert(make_pair("SELF", new EllipseBase(&cPos.x, &cPos.y, 20, 4)));
	mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&cPos.x, &cPos.y, 400, 150)));
	mCollision.insert(make_pair("SKILL_RANGE", new EllipseBase(&cPos.x, &cPos.y, 800, 350)));


	EFFECT->Create_Sprite("EXPLO1", 120, 150);


	for (int i = 0; i < FNFAL_YUTAN_MAX; ++i)
	{
		UtanInfo* info = new UtanInfo;
		info->utan = new SmokeBase;
		info->utan->init();
		info->utan->Insert_SmokeImage("YUTAN");

		info->u = new EllipseBase(&info->sklPos.x, &info->sklPos.y, FNFAL_SKILL_LRAD, FNFAL_SKILL_SRAD);

		vUtan.push_back(info);
	}

	cPos.x = 200.0f;
	cPos.y = 500.0f;

	Speed = FNFAL_SPEED;

	//state.HeatPoint.max = state.HeatPoint.current = 9999999;
	state.HeatPoint.max = state.HeatPoint.current = 3000;
	state.HitPoint = 1.0;
	state.CriticPoint = 25.5;
	state.CriticAcl = 50;
	state.AttackDelay = 0.8f;
	state.AimDelay = 0.45;
	state.AvoidPoint = 83.5;
	state.AttackPoint = 85;

	maxState = state;

	SklColTime = 0.0f;

	Angle = 0.0f;

	isAlive = true;
	Select = false;
	moveAble = true;
	//Focus = true;

	return S_OK;
}

void FNFAL::release()
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

void FNFAL::update()
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

			FLOAT acl = 2.5F;
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

	ImGui::DragFloat("Pos X", &cPos.x); 
	ImGui::DragFloat("Pos Y", &cPos.y); 

	ImGui::Text("VPos X : %d", VPOS->x);
	ImGui::Text("VPos Y : %d", VPOS->y);

	ImGui::Text("mouse X : %d", _ptMouse.x);
	ImGui::Text("mouse Y : %d", _ptMouse.y);

	ImGui::Text("virtualM X : %d", _ptMouse.x + VPOS->x);
	ImGui::Text("virtualM Y : %d", _ptMouse.y + VPOS->y);

	ImGui::Text("AIM DEL : %.5f", AtkColTime);
	ImGui::Text("ATK DEL : %.5f", SklColTime);
}

void FNFAL::render()
{
	IOPtaticDoll::render();
}

void FNFAL::Use_ActiveSkill()
{
	StopMoving();
	motion->Change_Motion("SKILL");

	FPNT AxisPos;
	AxisPos.x = _ptMouse.x + VPOS->x;
	AxisPos.y = _ptMouse.y + VPOS->y;

	Angle = getAngle(cPos.x, cPos.y, AxisPos.x, AxisPos.y);
		

	FiredYutan = 0;
	for (auto& it : vUtan) 
	{
		it->sklPos.x = AxisPos.x + (rand() % 100) - 50.0f;
		it->sklPos.y = AxisPos.y + (rand() % 50) - 25.0f;

		it->utan->setBulletSpeed(1200.0f);
	}

	/*switch (rand() % 3)
	{
	case 0:
		sound->Play_Effect("skil1", 0.15F);
		break;

	case 1:
		sound->Play_Effect("skil2", 0.15F);
		break;

	case 2:
		sound->Play_Effect("skil3", 0.15F);
		break;
	}*/
}

void FNFAL::MotionUpdate()
{
	//Target_ID = 0;

	if (isAlive)
	{
		if (Target_ID != -1)
		{
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
			if (motion->isCurrent("ATTACK"))
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
			motion->Change_Motion("DEATH");
			motion->getCurrentMotion()->setFrameX(0);
			//sound->Play_Effect("Death", 0.12f);
		}

	}
}

void FNFAL::Update_DrawPos()
{
	dPos.x = cPos.x;
	dPos.y = cPos.y - 47.0f;
	cr = RectMake(dPos.x + (93.0f*FNFAL_ACLRENDER), dPos.y + (49.0f*FNFAL_ACLRENDER),
		(107.0f*FNFAL_ACLRENDER), (149.0f*FNFAL_ACLRENDER));
}

void FNFAL::render_VisualBar()
{
	if (Select)
		hpBar->Render_VisualBar(cPos.x - 50.0F, cPos.y - 100.0F, ColorF(0.0F, 0.85F, 0.0F, 0.4F));

	else
		hpBar->Render_VisualBar(cPos.x - 50.0F, cPos.y - 100.0F, ColorF(0.0F, 0.85F, 0.0F, 0.2F));
}

void FNFAL::render_Motion()
{
	//if (isAlive)
		motion->render(D3DXVECTOR2(dPos.x - VPOS->x, dPos.y - VPOS->y), D3DXVECTOR2(FNFAL_ACLRENDER, FNFAL_ACLRENDER), _color, DgreeAngle(Angle));

	//else
		//motion->render(D3DXVECTOR2(dPos.x - VPOS->x, dPos.y - VPOS->y), D3DXVECTOR2(FNFAL_ACLRENDER, FNFAL_ACLRENDER), 1.0F, DgreeAngle(Angle));
}

void FNFAL::render_Ellipse()
{
	IOPtaticDoll::render_Ellipse();

	//DRAWMANAGER->renderRectCenter(dPos.x - VPOS->x, dPos.y - VPOS->y, 15, 40);

	for (auto& it : vUtan) {
		if (it->utan->getFired())
		{
			it->utan->render(0.25F);
			it->u->Rend_Ellipse(D3DXCOLOR(0.0F, 1.0F, 0.0F, 0.5F));
		}
	}
}

void FNFAL::FNFAL_AttackAction(void * ob)
{
	FNFAL* Object = (FNFAL*)ob;
	int frame = Object->getMotionManager().getCurrent_Frame().x;

	switch (frame)
	{
	case 3: case 7: case 14:
		if (Object->getMotionManager().getCurrentMotion()->getIsOnce())
		{
			auto pos = Object->getCharacterPos();

			if (!Object->getFlip())
			{
				pos.x += 50.0f;// +VPOS->x;
				pos.y -= 45.0f;
			}

			else
			{
				pos.x -= 90.0f; //+ VPOS->x;
				pos.y -= 50.0f;
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
		Object->getMotionManager().getCurrentMotion()->Reset_Motion();
	}
}

void FNFAL::FNFAL_SkillAction(void * ob)
{
	FNFAL* Object = (FNFAL*)ob;
	int frame = Object->getMotionManager().getCurrent_Frame().x;
	//auto sklTarget = Object->getSklTarget();

	switch (frame)
	{
	//case 1:
		//if (Object->getMotionManager().getCurrent_Counter() == 1)
		//	Object->getSklPos() = Object->getTargetID();
		//break;

	case 18:
		if (Object->getMotionManager().getCurrentMotion()->getIsOnce())
		{
			auto& left = Object->getFiredYutan();
			
			if (Object->getLookAngle() < RadianAngle(90.1f) || Object->getLookAngle() > RadianAngle(270.0f))
			{
				Object->getYutan(left)->Create_Smoke(Object->getCharacterPos().x + 15.0f, Object->getCharacterPos().y - 32.5f,
					Object->getSklPos(left).x, Object->getSklPos(left).y, 30.0f, 80.0f);
			}

			else
			{
				Object->getYutan(left)->Create_Smoke(Object->getCharacterPos().x - 15.0f, Object->getCharacterPos().y - 32.5f,
					Object->getSklPos(left).x, Object->getSklPos(left).y, 150.0f, 80.0f);
			}

			++left;
			SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "FIRE_YUTAN", 0.08f);
			//Object->getSoundManager().Play_Effect("sh", 0.15f);

			//if (Object->getSklPos() != -1)
			{
				//auto it = BDATA->getEnemyMember(BDATA->getEngageSquadID(), Object->getSklTarget());
				
			}
		}
		break;

	case 25:
		if (Object->getFiredYutan() < FNFAL_YUTAN_MAX)
			Object->getMotionManager().getCurrentMotion()->setFrameX(10);
		break;
	}

	if (Object->getMotionManager().getCurrentMotion()->getIsOver())
	{
		(*(Object->getAttackColTime())) = Object->getStatus().AttackDelay;	
		Object->getMotionManager().getCurrentMotion()->Reset_Motion();
		Object->getMotionManager().Change_Motion("IDLE");
	}
}

void FNFAL::FNFAL_DeathAction(void * ob)
{
	FNFAL* Object = (FNFAL*)ob;
	/*int frameX = Object->getMotionManager().getCurrent_FrameX();

	switch (frameX)
	{
	case 20:
		Object->getMotionManager().getCurrentMotion()->setCountGrav(0.8F);
		break;
	}*/


	if (Object->getMotionManager().getCurrentMotion()->getIsOver())
	{
		Object->getMotionManager().getCurrentMotion()->setFrameX(29);
		Object->getMotionManager().getCurrentMotion()->Pause_Motion();
	}
}

void FNFAL::FNFAL_VictoryAction(void * ob)
{
}

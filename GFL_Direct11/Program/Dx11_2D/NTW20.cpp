#include "stdafx.h"
#include "NTW20.h"

NTW20::NTW20()
{
	motion = new MotionManager;
	hpBar = new VisualBar;
}

NTW20::~NTW20()
{
}

void NTW20::LoadTray_List()
{
	//SOUND RESOURCE
	LOADMANAGER->Add_LoadTray("FIRE_SR1", "../../_SoundSource/Battle_181.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);


	//IMAGE RESOURCE

	LOADMANAGER->Add_LoadTray("NTW20", "../../_Textures/NTW20/NTW20.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20_IDLE", "../../_Textures/NTW20/Idle.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20_ATTACK", "../../_Textures/NTW20/Attack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20_DEATH", "../../_Textures/NTW20/Death.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20_SKILL", "../../_Textures/NTW20/Skill.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20_WALK", "../../_Textures/NTW20/Walk.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20_VICTORY", "../../_Textures/NTW20/Victory.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20_VICLOOP", "../../_Textures/NTW20/VicLoop.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
}

HRESULT NTW20::init()
{
	IOPtaticDoll::init();

	hpBar->Create_VisualBar(&(state.HeatPoint.current), &(state.HeatPoint.max), 100, 5);

	motion->Create("IDLE", "NTW20_IDLE", 390, 340, 0.025F);
	motion->Create("WALK", "NTW20_WALK", 390, 340, 0.0001F);
	motion->getMotion("WALK")->setCountGrav(200.0F);

	motion->Create("ATTACK", "NTW20_ATTACK", 390, 340, 0.001F);
	motion->Create("SKILL", "NTW20_SKILL", 390, 340, 0.015F);
	motion->Create("DEATH", "NTW20_DEATH", 390, 340, 0.015F);
	motion->Create("VICTORY", "NTW20_VICTORY", 390, 340, 0.015F);
	motion->Create("VICLOOP", "NTW20_VICLOOP", 390, 340, 0.015F);

	motion->RegisterFunction("ATTACK", NTW20_AttackAction, this);
	motion->RegisterFunction("VICTORY", NTW20_VictoryAction, this);

	motion->Change_Motion("IDLE");


	mCollision.insert(make_pair("SELF", new EllipseBase(&cPos.x, &cPos.y, 20, 4)));
	mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&cPos.x, &cPos.y, 700, 350)));
	mCollision.insert(make_pair("MIN_RANGE", new EllipseBase(&cPos.x, &cPos.y, 150, 60)));

	cPos.x = 100.0f;
	cPos.y = 500.0f;

	Speed = NTW20_SPEED;

	state.HeatPoint.max = state.HeatPoint.current = 1500;
	state.HitPoint = 3000.0;
	state.CriticPoint = 25.5;
	state.CriticAcl = 50;
	state.AttackDelay = 1.5f;
	state.AimDelay = 0.45;
	state.AvoidPoint = 83.5;
	state.AttackPoint = 230;

	maxState = state;

	Angle = 0.0f;

	isAlive = true;
	Select = false;
	moveAble = true;
	//Focus = true;

	return S_OK;
}

void NTW20::release()
{
	IOPtaticDoll::release();
}

void NTW20::update()
{
	IOPtaticDoll::update();

	motion->update();

	//DrawPosition ReCheck
	this->Camera_Focus();
	this->Update_DrawPos();

	//ImGui::DragFloat("Pos X", &cPos.x);
	//ImGui::DragFloat("Pos Y", &cPos.y);

	//ImGui::Text("VPos X : %d", VPOS->x);
	//ImGui::Text("VPos Y : %d", VPOS->y);

	//ImGui::Text("mouse X : %d", _ptMouse.x);
	//ImGui::Text("mouse Y : %d", _ptMouse.y);

	//ImGui::Text("virtualM X : %d", _ptMouse.x + VPOS->x);
	//ImGui::Text("virtualM Y : %d", _ptMouse.y + VPOS->y);

	//ImGui::Text("AIM DEL : %.5f", AtkColTime);
	//ImGui::Text("ATK DEL : %.5f", SklColTime);
	//ImGui::Text("VPos Y : %d", VPOS->y);

	//ImGui::DragFloat("Pos X", &cPos.x);
	//ImGui::DragFloat("Pos Y", &cPos.y);

}

void NTW20::render()
{
	IOPtaticDoll::render();
}

void NTW20::Use_ActiveSkill()
{
}

void NTW20::MotionUpdate()
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

void NTW20::Update_DrawPos()
{
	dPos.x = cPos.x;
	dPos.y = cPos.y - 47.0f;
	cr = RectMake(dPos.x + (93.0f*NTW20_ACLRENDER), dPos.y + (49.0f*NTW20_ACLRENDER),
		(107.0f*NTW20_ACLRENDER), (149.0f*NTW20_ACLRENDER));

	//if (motion->isCurrent("IDLE") || motion->isCurrent("WALK"))
	//{
	//
	//	dPos.x = cPos.x;
	//	dPos.y = cPos.y - 47.0f;
	//	cr = RectMake(dPos.x + (93.0f*NTW20_ACLRENDER), dPos.y + (49.0f*NTW20_ACLRENDER),
	//		(107.0f*NTW20_ACLRENDER), (149.0f*NTW20_ACLRENDER));
	//}
	//else
	//{
	//	dPos.x = cPos.x - NTW20_DRAWPOS_X2 - BDATA->getVirtualPos().x;
	//	dPos.y = cPos.y - NTW20_DRAWPOS_Y2 - BDATA->getVirtualPos().y;

	//	cr = RectMake(dPos.x + (93.0f*NTW20_ACLRENDER), dPos.y + (88.0f*NTW20_ACLRENDER),
	//		(145.0f*NTW20_ACLRENDER), (149.0f*NTW20_ACLRENDER));
	//}
}

void NTW20::render_VisualBar()
{
	hpBar->Render_VisualBar(cPos.x - 50.0F, cPos.y - 100.0F, ColorF(0.0F, 0.85F, 0.0F, 1.0F));
}

void NTW20::render_Motion()
{
	if (isAlive)
		motion->render(D3DXVECTOR2(dPos.x - VPOS->x, dPos.y - VPOS->y), D3DXVECTOR2(NTW20_ACLRENDER, NTW20_ACLRENDER), 1.0F, DgreeAngle(Angle));

	else
		motion->render(D3DXVECTOR2(dPos.x - VPOS->x, dPos.y - VPOS->y), D3DXVECTOR2(NTW20_ACLRENDER, NTW20_ACLRENDER), 1.0F, DgreeAngle(Angle));
}

void NTW20::render_Ellipse()
{
	IOPtaticDoll::render_Ellipse();
}

void NTW20::NTW20_AttackAction(void * ob)
{
	NTW20* Object = (NTW20*)ob;
	int frame = Object->getMotionManager().getCurrent_Frame().x;

	switch (frame)
	{
	case 3:
		if (Object->getMotionManager().getCurrentMotion()->getIsOnce())
		{
			auto pos = Object->getCharacterPos();
			//float a = getAngle(pos.x, pos.y, _ptMouse.x, _ptMouse.y);

			if (!Object->getFlip())
			{
				pos.x += 90.0f;
				pos.y -= 18.0f;
			}

			else
			{
				pos.x -= 90.0f;
				pos.y -= 18.0f;
			}

			SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "FIRE_SR1", 0.15f);
			BEFCT->CreateBullet("RIFLE_BLT", pos, Object->getTargetID(), Object->getStatus(), BULLET_TYPE::GRF_BLT, 3500.0F);
			//BEFCT->CreateBullet("RIFLE_BLT", pos, a, 1500.0f, Object->getStatus(), BULLET_TYPE::GRF_BLT, 2500.0F);
		}
		break;
	}

	if (Object->getMotionManager().getCurrentMotion()->getIsOver())
	{
		(*(Object->getAttackColTime())) = (float)Object->getStatus().AttackDelay;
		Object->getMotionManager().getCurrentMotion()->Reset_Motion();
	}
}

void NTW20::NTW20_SkillAction(void * ob)
{

}

void NTW20::NTW20_DeathAction(void * ob)
{
}

void NTW20::NTW20_VictoryAction(void * ob)
{
	NTW20* Object = (NTW20*)ob;

	if (Object->getMotionManager().getCurrentMotion()->getIsOver())
		Object->getMotionManager().Change_Motion("VICLOOP");
}
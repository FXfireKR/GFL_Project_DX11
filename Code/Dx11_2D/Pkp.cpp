#include "stdafx.h"
#include "Pkp.h"

Pkp::Pkp()
{
	keys.name = "PKP";

	weaponType = TWT_MG;

	keys.SOUND_ATTACK = "PKP_ATTACK_JP";
	keys.SOUND_BREAK = "PKP_BREAK_JP";
	keys.SOUND_DEFENSE = "PKP_DEFENSE_JP";
	keys.SOUND_DIALOGUE1 = "PKP_DIALOGUE1_JP";
	keys.SOUND_DIALOGUE2 = "PKP_DIALOGUE2_JP";
	keys.SOUND_DIALOGUE3 = "PKP_DIALOGUE3_JP";
	keys.SOUND_FORMATION = "PKP_FORMATION_JP";
	keys.SOUND_GOATTACK = "PKP_GOATTACK_JP";
	keys.SOUND_MEET = "PKP_MEET_JP";
	keys.SOUND_SKILL1 = "PKP_SKILL1_JP";
	keys.SOUND_SKILL2 = "PKP_SKILL2_JP";
	keys.SOUND_SKILL3 = "PKP_SKILL3_JP";
	keys.SOUND_TITLECALL = "PKP_TITLECALL_JP";
	keys.SOUND_WIN = "PKP_WIN_JP";

	keys.vecDialogue.push_back("내가 나서면 승리는 확실하다.");
	keys.vecDialogue.push_back("PK? 그런 어설픈 각오로는 날 따라잡을 수 없어.");
	keys.vecDialogue.push_back("한 번 더 그렇게 손 댔다간 다음엔 병원에 보낼 줄 알어.");

	mEquip.insert(make_pair(EPT_ACESORY, nullptr));		// 사이트
	mEquip.insert(make_pair(EPT_BULLET, nullptr));		// 탄환
	mEquip.insert(make_pair(EPT_ACESORY2, nullptr));	// 외골격

	//	Setting Spec
	//curState.HitPoint.max = curState.HitPoint.curr = 2530;
	curState.HitPoint.max = curState.HitPoint.curr = 992530;
	curState.Armor = 0;
	curState.Accuracy = 0.1;
	curState.CriticPoint = 40.0;
	curState.CriticAcl = 50;
	curState.AttackDelay = 0.0000;
	curState.AimDelay = 0.59;
	curState.Avoid = 0.135;
	curState.AttackPoint = 80;
	curState.ArmorPierce = 0;

	maxState = curState;

	statusManager = new StatusManager(&curState, &maxState, &mEquip);
}

Pkp::~Pkp()
{
}

void Pkp::LoadTray_SoundList()
{
	//	Character Voice Upload
	LOAD->Add_LoadTray(keys.SOUND_ATTACK, "Characters/pkp/PKP_ATTACK_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_BREAK, "Characters/pkp/PKP_BREAK_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DEFENSE, "Characters/pkp/PKP_DEFENSE_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE1, "Characters/pkp/PKP_DIALOGUE1_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE2, "Characters/pkp/PKP_DIALOGUE2_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE3, "Characters/pkp/PKP_DIALOGUE3_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_FORMATION, "Characters/pkp/PKP_FORMATION_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_GOATTACK, "Characters/pkp/PKP_GOATTACK_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_MEET, "Characters/pkp/PKP_MEET_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL1, "Characters/pkp/PKP_SKILL1_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL2, "Characters/pkp/PKP_SKILL2_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL3, "Characters/pkp/PKP_SKILL3_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_TITLECALL, "Characters/pkp/PKP_TITLECALL_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_WIN, "Characters/pkp/PKP_WIN_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	//	Fortrait Image Load
	LOAD->Add_LoadTray("pic_PKP", "Characters/pkp/pic_PKP.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_PKP_alpha", "Characters/pkp/pic_PKP_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_PKP_D", "Characters/pkp/pic_PKP_D.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_PKP_D_alpha", "Characters/pkp/pic_PKP_D_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOAD->Add_LoadTray("PKP_N0", "Characters/pkp/pic_pkp_n_0.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("PKP_N1", "Characters/pkp/pic_pkp_n_1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	keys.original_key = "pic_PKP";
	keys.original_D_key = "pic_PKP_D";

	keys.cardNormalKey = "PKP_N0";
	keys.cardHurtKey = "PKP_N1";
}

void Pkp::LoadTray_ImageList()
{
	this->init();
}

HRESULT Pkp::init()
{
	BaseTaticDoll::init();

	axisMax_LongRad = 300.0f;
	axisMax_ShortRad = 100.0f;
	axisMin_LongRad = axisMax_LongRad * 0.45f;
	axisMin_ShortRad = axisMax_ShortRad * 0.45f;

	if (motion == nullptr)
	{
		motion = new spineMotion;

		if (motion != nullptr)
		{
			motion->loadSpine_FromJsonFile("PKP");
			motion->setMotionAction("attack", PKP_Attack_Action);
			motion->setMotionAction("reload", PKP_Reload_Action);

			for (int i = 0; i < 3; ++i)
			{
				motion->changeMotion("move", false, true);
				motion->changeMotion("attack", false, true, 0.001f);
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

	Pos.x = 200.0f;
	Pos.y = 250.0f;

	if (mCollision.count("SELF") == 0)
		mCollision.insert(make_pair("SELF", new EllipseBase(&Pos.x, &Pos.y, 30, 5)));

	if (mCollision.count("MAX_RANGE") == 0)
		mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&Pos.x, &Pos.y, axisMax_LongRad, axisMax_ShortRad)));

	moveSpd = PKP_SPEED;

	atkColTime = 0.0;
	sklColTime = 0.0;
	TargetAngle = Angle = 0.0f;
	skillTimer = 0.0;

	isAlive = true;
	Select = false;
	moveAble = true;
	isActive = false;

	safeTrigger = 0;
	leftBullet = PKP_MAX_MAGAZINE;

	alianceType = ALIANCE_GRIFFON;
	waitAfter = false;

	return S_OK;
}

void Pkp::release()
{
	BaseTaticDoll::release();
}

void Pkp::update()
{
	statusManager->update(DELTA() * DeltaAcl);

	BaseTaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	motion->update(DELTA() * DeltaAcl);
	this->MotionUpdate();

	if (isActive)
	{
		skillTimer -= DELTA() * DeltaAcl;

		if (skillTimer < DELTA() * DeltaAcl)
		{
			skillTimer = 0.0;
			isActive = false;

			sklColTime = PKP_SKILL_COLTIME;
		}

		else
		{
			if (curState.ArmorPoint.max > curState.ArmorPoint.curr + 1)
				++curState.ArmorPoint.curr;

			else
				curState.ArmorPoint.curr = curState.ArmorPoint.max;
		}

		if (_color.b > 0.9)
			_color.b = 0.0f;
	}
}

void Pkp::render()
{
}

void Pkp::Use_ActiveSkill()
{
	if (!isActive)
	{
		if (sklColTime > 0.0) {}

		else
		{
			isActive = true;

			skillTimer = PKP_SKILL_MAINTAIN;

			Status skillstate;

			skillstate.AimDelay = (curState.AimDelay * 0.8f) * -1.0f;
			skillstate.AttackDelay = (curState.AttackDelay * 0.8f) * -1.0f;
			skillstate.AttackPoint = (curState.AttackPoint * 3) / 2;

			statusManager->create(skillstate, PKP_SKILL_MAINTAIN);

			switch (rand() % 3)
			{
			case 0:
				SOUND->Play_Effect(SOUND_CHANNEL::CH_VOICE, keys.SOUND_SKILL1, 0.15f);
				break;

			case 1:
				SOUND->Play_Effect(SOUND_CHANNEL::CH_VOICE, keys.SOUND_SKILL2, 0.15f);
				break;

			case 2:
				SOUND->Play_Effect(SOUND_CHANNEL::CH_VOICE, keys.SOUND_SKILL2, 0.15f);
				break;
			}
		}
	}
}

void Pkp::MotionUpdate()
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
				if (leftBullet == 0)
					motion->changeMotion("reload", false, true);

				else
					motion->changeMotion("attack", false);

				waitAfter = true;
			}

			else if (motion->isCurrent("attack"))
			{
				if (atkColTime > 0.0)
				{
					safeTrigger = 0;
					if (!waitAfter)
						motion->pauseAt(-0.0167f);

					else
						motion->pauseAt(-0.016650f);
				}
			}
		}

		else
		{
			if (motion->isCurrent("attack"))
			{
				motion->changeMotion("wait", true, true);
				moveAble = true;
			}
		}

		if (!motion->isCurrent("attack"))
			atkColTime = curState.AimDelay;
	}
}

void Pkp::Update_DrawPos()
{
	motion->p_getScale().x = PKP_ACL_X;
	motion->p_getScale().y = PKP_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0 * DELTA()) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0 * DELTA()) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void Pkp::render_VisualBar()
{
	if (isAlive)
	{
		//	HP - hit point
		Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - 150.0f), curState.HitPoint.curr,
			curState.HitPoint.max, VEC2(100, 5), ColorF(0, 0.8, 0, 0.5f));

		//	AP - Armor point
		/*if (curState.ArmorPoint.curr > 0)
		{

			Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - 150.0f), curState.ArmorPoint.curr,
				curState.ArmorPoint.max, VEC2(100, 5), ColorF(0.1, 0.1, 0.1, 0.5f));

			DRAW->render("amo", VEC2(10, 10), VEC2(Pos.x - 58.0f, Pos.y - 148.0f), COLR(1, 1, 1, 1));
		}
		else
			DRAW->render("bamo", VEC2(10, 10), VEC2(Pos.x - 58.0f, Pos.y - 148.0f), COLR(1, 1, 1, 1));
		*/

		//	SP - skill cool down
		if (isActive)
		{
			if (skillTimer > 0.0f)
				Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - 145.0f), skillTimer,
					PKP_SKILL_MAINTAIN, VEC2(100, 5), ColorF(0.1, 0.1, 0.8, 0.5f));
		}

		else
		{
			if (sklColTime > 0.0f)
				Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - 145.0f), sklColTime,
					PKP_SKILL_COLTIME, VEC2(100, 5), ColorF(0.1, 0.1, 0.8, 0.5f));
		}
	}
}

void Pkp::render_Motion()
{
	BaseTaticDoll::render_Motion();
	motion->render();
}

void Pkp::render_Ellipse()
{
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.1, 0.8, 0.1, Select ? 0.8f : 0.1f));
}

void Pkp::PKP_Attack_Action(void * _this)
{
	Pkp* object = (Pkp*)_this;

	if (object->atkColTime < FLOAT_EPSILON)
	{
		object->moveAble = false;

		spineMotion* motion = object->motion;
		float curTime = motion->getCurTime();

		//	1
		if (curTime < 0.036f && curTime > 0.036f - DELTA())
		{
			if (object->safeTrigger == 0)
			{
				BULLET->CreateBullet("MG_BLT", object->getCharacterPos(), object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUND->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "mgSound2", 0.15f);
				++object->safeTrigger;
				--object->leftBullet;
			}
		}

		else if (curTime < 0.136f && curTime > 0.136f - DELTA())
		{
			if (object->safeTrigger == 1)
			{
				BULLET->CreateBullet("MG_BLT", object->getCharacterPos(), object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUND->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "mgSound2", 0.15f);
				++object->safeTrigger;
				--object->leftBullet;
			}
		}

		else if (curTime < 0.24f && curTime > 0.24f - DELTA())
		{
			if (object->safeTrigger == 2)
			{
				BULLET->CreateBullet("MG_BLT", object->getCharacterPos(), object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUND->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "mgSound2", 0.15f);
				++object->safeTrigger;
				--object->leftBullet;
			}
		}


		else if (motion->getCurTime() > motion->getEndTime())
		{
			object->atkColTime = object->curState.AttackDelay;

			if (object->TargetID != -1)
			{
				if (object->leftBullet == 0)
					motion->changeMotion("reload", false, true);

				else
					motion->changeMotion("attack", false);

				object->waitAfter = false;
			}
			else
				motion->changeMotion("wait", true, true);

			object->safeTrigger = 0;
			object->moveAble = true;
		}

	}
}

void Pkp::PKP_Reload_Action(void * _this)
{
	Pkp* object = (Pkp*)_this;
	spineMotion* motion = object->motion;
	float curTime = motion->getCurTime();

	object->moveAble = false;

	if (curTime < 4.032f && curTime > 4.032f - DELTA())
	{
		if (object->safeTrigger == 0)
		{
			SOUND->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "mgReload", 0.1f);
			++object->safeTrigger;
			object->leftBullet = object->PKP_MAX_MAGAZINE;
		}
	}

	else if (motion->getCurTime() > motion->getEndTime())
	{
		object->moveAble = true;
		motion->changeMotion("wait", true, true);
		object->safeTrigger = 0;
	}
}

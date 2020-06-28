#include "stdafx.h"
#include "Ksg.h"

Ksg::Ksg()
{
	keys.name = "KSG";

	weaponType = TWT_SG;

	keys.SOUND_ATTACK = "KSG_ATTACK_JP";
	keys.SOUND_BREAK = "KSG_BREAK_JP";
	keys.SOUND_DEFENSE = "KSG_DEFENSE_JP";
	keys.SOUND_DIALOGUE1 = "KSG_DIALOGUE1_JP";
	keys.SOUND_DIALOGUE2 = "KSG_DIALOGUE2_JP";
	keys.SOUND_DIALOGUE3 = "KSG_DIALOGUE3_JP";
	keys.SOUND_FORMATION = "KSG_FORMATION_JP";
	keys.SOUND_GOATTACK = "KSG_GOATTACK_JP";
	keys.SOUND_MEET = "KSG_MEET_JP";
	keys.SOUND_SKILL1 = "KSG_SKILL1_JP";
	keys.SOUND_SKILL2 = "KSG_SKILL2_JP";
	keys.SOUND_SKILL3 = "KSG_SKILL3_JP";
	keys.SOUND_TITLECALL = "KSG_TITLECALL_JP";
	keys.SOUND_WIN = "KSG_WIN_JP";

	keys.vecDialogue.push_back("작전은 언제든 개시될 수 있으므로, 대비해주십시오.");
	keys.vecDialogue.push_back("지난번 훈련 데이터가 발표되었습니까? 아직 좀 더 향상할 수 있을지도 모릅니다.");
	keys.vecDialogue.push_back("그 손 멈추시죠. 지휘관, 저의 인내심과 반사 신경을 시험하실 필요는 없습니다.");

	mEquip.insert(make_pair(EPT_ACESORY, nullptr));		// 사이트
	mEquip.insert(make_pair(EPT_BULLET, nullptr));		// 탄환
	mEquip.insert(make_pair(EPT_ACESORY2, nullptr));	// 외골격

	//	Setting Spec
	curState.HitPoint.max = curState.HitPoint.curr = 2530;
	curState.ArmorPoint.max = curState.ArmorPoint.curr = 240;
	curState.Armor = 15;
	curState.Accuracy = 0.9;
	curState.CriticPoint = 40.0;
	curState.CriticAcl = 50;
	curState.AttackDelay = 0.8;
	curState.AimDelay = 0.59;
	curState.Avoid = 0.135;
	curState.AttackPoint = 80;
	curState.ArmorPierce = 0;

	origState = maxState = curState;

	statusManager = new StatusManager(&curState, &maxState, &mEquip);
}

Ksg::~Ksg()
{
}

void Ksg::LoadTray_SoundList()
{
	//	Character Voice Upload
	LOAD->Add_LoadTray(keys.SOUND_ATTACK, "Characters/ksg/KSG_ATTACK_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_BREAK, "Characters/ksg/KSG_BREAK_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DEFENSE, "Characters/ksg/KSG_DEFENSE_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE1, "Characters/ksg/KSG_DIALOGUE1_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE2, "Characters/ksg/KSG_DIALOGUE2_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE3, "Characters/ksg/KSG_DIALOGUE3_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_FORMATION, "Characters/ksg/KSG_FORMATION_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_GOATTACK, "Characters/ksg/KSG_GOATTACK_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_MEET, "Characters/ksg/KSG_MEET_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL1, "Characters/ksg/KSG_SKILL1_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL2, "Characters/ksg/KSG_SKILL2_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL3, "Characters/ksg/KSG_SKILL3_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_TITLECALL, "Characters/ksg/KSG_TITLECALL_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_WIN, "Characters/ksg/KSG_WIN_JP.acb", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	//	Fortrait Image Load
	LOAD->Add_LoadTray("pic_KSG", "Characters/ksg/pic_KSG.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_KSG_alpha", "Characters/ksg/pic_KSG_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_KSG_D", "Characters/ksg/pic_KSG_D.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_KSG_D_alpha", "Characters/ksg/pic_KSG_D_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOAD->Add_LoadTray("KSG_N0", "Characters/ksg/pic_ksg_n_0.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("KSG_N1", "Characters/ksg/pic_ksg_n_1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	keys.original_key = "pic_KSG";
	keys.original_D_key = "pic_KSG_D";

	keys.cardNormalKey = "KSG_N0";
	keys.cardHurtKey = "KSG_N1";
}

void Ksg::LoadTray_ImageList()
{
	this->init();
}

HRESULT Ksg::init()
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
			motion->loadSpine_FromJsonFile("KSG");
			motion->setMotionAction("attack", KSG_Attack_Action);
			motion->setMotionAction("reload", KSG_Reload_Action);

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

	moveSpd = KSG_SPEED;

	atkColTime = 0.0;
	sklColTime = 0.0;
	TargetAngle = Angle = 0.0f;
	skillTimer = 0.0;

	isAlive = true;
	Select = false;
	moveAble = true;
	isActive = false;

	safeTrigger = 0;
	leftBullet = KSG_MAX_BULLET;

	alianceType = ALIANCE_GRIFFON;
	waitAfter = false;

	return S_OK;
}

void Ksg::release()
{
	BaseTaticDoll::release();
}

void Ksg::update()
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

			sklColTime = KSG_SKILL_COLTIME;
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

void Ksg::render()
{
}

void Ksg::Use_ActiveSkill()
{
	if (!isActive)
	{
		if (sklColTime > 0.0) {}

		else
		{
			isActive = true;

			skillTimer = KSG_SKILL_MAINTAIN;

			Status skillstate;

			skillstate.AimDelay = (curState.AimDelay * 0.8f) * -1.0f;
			skillstate.AttackDelay = (curState.AttackDelay * 0.8f) * -1.0f;
			skillstate.AttackPoint = (curState.AttackPoint * 3) / 2;

			statusManager->create(skillstate, KSG_SKILL_MAINTAIN);

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

void Ksg::MotionUpdate()
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

void Ksg::Update_DrawPos()
{
	motion->p_getScale().x = KSG_ACL_X;
	motion->p_getScale().y = KSG_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0 * DELTA()) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0 * DELTA()) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void Ksg::render_VisualBar()
{
	if (isAlive)
	{
		//	HP - hit point
		Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - 150.0f), curState.HitPoint.curr,
			curState.HitPoint.max, VEC2(100, 5), ColorF(0, 0.8, 0, 0.5f));

		//	AP - Armor point
		if (curState.ArmorPoint.curr > 0)
		{

			Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - 150.0f), curState.ArmorPoint.curr,
				curState.ArmorPoint.max, VEC2(100, 5), ColorF(0.1, 0.1, 0.1, 0.5f));

			DRAW->render("amo", VEC2(20, 20), VEC2(Pos.x - 58.0f, Pos.y - 148.0f), COLR(1, 1, 1, 1));
		}
		else
			DRAW->render("bamo", VEC2(20, 20), VEC2(Pos.x - 58.0f, Pos.y - 148.0f), COLR(1, 1, 1, 1));


		//	SP - skill cool down
		if (isActive)
		{
			if (skillTimer > 0.0f)
				Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - 145.0f), skillTimer,
					KSG_SKILL_MAINTAIN, VEC2(100, 5), ColorF(0.1, 0.1, 0.8, 0.5f));
		}

		else
		{
			if (sklColTime > 0.0f)
				Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - 145.0f), sklColTime,
					KSG_SKILL_COLTIME, VEC2(100, 5), ColorF(0.1, 0.1, 0.8, 0.5f));
		}

		//	Left Bullets
		for (int i = 0; i < leftBullet; ++i)
			DRAW->render("sgmm", VEC2(20, 10), VEC2(Pos.x - 40.0f + (i * 20), Pos.y - 130.0f), COLR(1, 1, 1, 1));
	}
}

void Ksg::render_Motion()
{
	BaseTaticDoll::render_Motion();
	motion->render();
}

void Ksg::render_Ellipse()
{
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.1, 0.8, 0.1, Select ? 0.8f : 0.1f));
}

void Ksg::KSG_Attack_Action(void * _this)
{
	Ksg* object = (Ksg*)_this;

	if (object->atkColTime < FLOAT_EPSILON)
	{
		object->moveAble = false;

		spineMotion* motion = object->motion;
		float curTime = motion->getCurTime();

		//	1
		if (curTime < 0.058f && curTime > 0.058f - DELTA())
		{
			if (object->safeTrigger == 0)
			{
				//SG_BLT
				EFFECT->createEffect("SG_BLT", VEC2(object->Pos.x + (float)(120 * object->flipVal), object->Pos.y - 65)
					, DELTA() * 2.0f, 80.0f, 1.0f, object->Flip);

				BULLET->CreateBullet(object, object->TargetID, object->curState, object->alianceType, 90.0f, 150.0f);

				// SG는 Bullet을 사용하지않는다.

				SOUND->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "sgSound", 0.05f);
				++object->safeTrigger;
				--object->leftBullet;
			}
		}

		if (curTime < 0.883f && curTime > 0.883f - DELTA())
		{
			if (object->safeTrigger == 1)
			{
				SOUND->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "sgReload", 0.05f);
				++object->safeTrigger;
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

void Ksg::KSG_Reload_Action(void * _this)
{
	Ksg* object = (Ksg*)_this;
	spineMotion* motion = object->motion;
	float curTime = motion->getCurTime();

	object->moveAble = false;

	if (curTime < 2.645f && curTime > 2.645f - DELTA())
	{
		if (object->safeTrigger == 0)
		{
			SOUND->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "sgReload", 0.05f);
			++object->safeTrigger;
			object->leftBullet = object->KSG_MAX_BULLET;
		}
	}

	else if (motion->getCurTime() > motion->getEndTime())
	{
		object->moveAble = true;
		motion->changeMotion("wait", true, true);
		object->safeTrigger = 0;
	}
}
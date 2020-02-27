#include "stdafx.h"
#include "Ksg.h"

Ksg::Ksg()
{
	name = "KSG";

	SOUND_ATTACK = "KSG_ATTACK_JP";
	SOUND_BREAK = "KSG_BREAK_JP";
	SOUND_DEFENSE = "KSG_DEFENSE_JP";
	SOUND_DIALOGUE1 = "KSG_DIALOGUE1_JP";
	SOUND_DIALOGUE2 = "KSG_DIALOGUE2_JP";
	SOUND_DIALOGUE3 = "KSG_DIALOGUE3_JP";
	SOUND_FORMATION = "KSG_FORMATION_JP";
	SOUND_GOATTACK = "KSG_GOATTACK_JP";
	SOUND_MEET = "KSG_MEET_JP";
	SOUND_SKILL1 = "KSG_SKILL1_JP";
	SOUND_SKILL2 = "KSG_SKILL2_JP";
	SOUND_SKILL3 = "KSG_SKILL3_JP";
	SOUND_TITLECALL = "KSG_TITLECALL_JP";
	SOUND_WIN = "KSG_WIN_JP";

	vecDialogue.push_back("������ ������ ���õ� �� �����Ƿ�, ������ֽʽÿ�.");
	vecDialogue.push_back("������ �Ʒ� �����Ͱ� ��ǥ�Ǿ����ϱ�? ���� �� �� ����� �� �������� �𸨴ϴ�.");
	vecDialogue.push_back("�� �� ���߽���. ���ְ�, ���� �γ��ɰ� �ݻ� �Ű��� �����Ͻ� �ʿ�� �����ϴ�.");

	mEquip.insert(make_pair(EPT_ACESORY, nullptr));		// ����Ʈ
	mEquip.insert(make_pair(EPT_BULLET, nullptr));		// źȯ
	mEquip.insert(make_pair(EPT_ACESORY2, nullptr));	// �ܰ��

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

	maxState = curState;

	buffList = new BuffManager(&curState, &maxState);
}

Ksg::~Ksg()
{
}

void Ksg::LoadTray_SoundList()
{
	//	Character Voice Upload
	LOADMANAGER->Add_LoadTray(SOUND_ATTACK, "../../_Assets/Characters/ksg/KSG_ATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_BREAK, "../../_Assets/Characters/ksg/KSG_BREAK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DEFENSE, "../../_Assets/Characters/ksg/KSG_DEFENSE_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE1, "../../_Assets/Characters/ksg/KSG_DIALOGUE1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE2, "../../_Assets/Characters/ksg/KSG_DIALOGUE2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE3, "../../_Assets/Characters/ksg/KSG_DIALOGUE3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_FORMATION, "../../_Assets/Characters/ksg/KSG_FORMATION_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_GOATTACK, "../../_Assets/Characters/ksg/KSG_GOATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_MEET, "../../_Assets/Characters/ksg/KSG_MEET_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL1, "../../_Assets/Characters/ksg/KSG_SKILL1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL2, "../../_Assets/Characters/ksg/KSG_SKILL2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL3, "../../_Assets/Characters/ksg/KSG_SKILL3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_TITLECALL, "../../_Assets/Characters/ksg/KSG_TITLECALL_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_WIN, "../../_Assets/Characters/ksg/KSG_WIN_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	//	Fortrait Image Load
	LOADMANAGER->Add_LoadTray("pic_KSG", "../../_Assets/Characters/ksg/pic_KSG.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_KSG_alpha", "../../_Assets/Characters/ksg/pic_KSG_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_KSG_D", "../../_Assets/Characters/ksg/pic_KSG_D.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_KSG_D_alpha", "../../_Assets/Characters/ksg/pic_KSG_D_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOADMANAGER->Add_LoadTray("KSG_N0", "../../_Assets/Characters/ksg/pic_ksg_n_0.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("KSG_N1", "../../_Assets/Characters/ksg/pic_ksg_n_1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	original_key = "pic_KSG";
	original_D_key = "pic_KSG_D";

	cardNormalKey = "KSG_N0";
	cardHurtKey = "KSG_N1";
}

void Ksg::LoadTray_ImageList()
{
	this->init();
}

HRESULT Ksg::init()
{
	TaticDoll::init();

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
	TaticDoll::release();
}

void Ksg::update()
{
	buffList->update(DELTA * DeltaAcl);

	TaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	motion->update(DELTA * DeltaAcl);
	this->MotionUpdate();

	if (isActive)
	{
		skillTimer -= DELTA * DeltaAcl;

		if (skillTimer < DELTA * DeltaAcl)
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

			buffList->create(skillstate, KSG_SKILL_MAINTAIN);

			switch (rand() % 3)
			{
			case 0:
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_VOICE, SOUND_SKILL1, 0.15f);
				break;

			case 1:
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_VOICE, SOUND_SKILL2, 0.15f);
				break;

			case 2:
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_VOICE, SOUND_SKILL2, 0.15f);
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

	//	Flip�� ���� ��ȯ��
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0 * DELTA) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0 * DELTA) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void Ksg::render_VisualBar()
{
	if (isAlive)
	{
		//	HP - hit point
		Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - 150.0f), curState.HitPoint.curr,
			curState.HitPoint.max, DV2(100, 5), ColorF(0, 0.8, 0, 0.5f));
		
		//	AP - Armor point
		if (curState.ArmorPoint.curr > 0)
		{		
			
			Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - 150.0f), curState.ArmorPoint.curr,
				curState.ArmorPoint.max, DV2(100, 5), ColorF(0.1, 0.1, 0.1, 0.5f));

			DRAW->render("amo", DV2(10, 10), DV2(Pos.x - 58.0f, Pos.y - 148.0f), DCR(1, 1, 1, 1));
		}
		else
			DRAW->render("bamo", DV2(10, 10), DV2(Pos.x - 58.0f, Pos.y - 148.0f), DCR(1, 1, 1, 1));


		//	SP - skill cool down
		if (isActive)
		{
			if (skillTimer > 0.0f)
				Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - 145.0f), skillTimer,
					KSG_SKILL_MAINTAIN, DV2(100, 5), ColorF(0.1, 0.1, 0.8, 0.5f));
		}

		else
		{
			if (sklColTime > 0.0f)
				Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - 145.0f), sklColTime,
					KSG_SKILL_COLTIME, DV2(100, 5), ColorF(0.1, 0.1, 0.8, 0.5f));
		}

		//	Left Bullets
		for (int i = 0; i < leftBullet; ++i)
			DRAW->render("sgmm", DV2(10, 5), DV2(Pos.x - 40.0f + (i * 20), Pos.y - 130.0f), DCR(1, 1, 1, 1));
	}
}

void Ksg::render_Motion()
{
	TaticDoll::render_Motion();
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
		if (curTime < 0.058f && curTime > 0.058f - DELTA)
		{
			if (object->safeTrigger == 0)
			{
				//SG_BLT
				EFFECT->createEffect("SG_BLT", DV2(object->Pos.x + 120, object->Pos.y - 65), DELTA * 2.0f, 80.0f);

				BULLET->CreateBullet(object, object->TargetID, object->curState, object->alianceType, 90.0f, 150.0f);

				// SG�� Bullet�� ��������ʴ´�.

				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "sgSound", 0.05f);
				++object->safeTrigger;
				--object->leftBullet;
			}
		}

		if (curTime < 0.883f && curTime > 0.883f - DELTA)
		{
			if (object->safeTrigger == 1)
			{
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "sgReload", 0.05f);
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

	if (curTime < 2.645f && curTime > 2.645f - DELTA)
	{
		if (object->safeTrigger == 0)
		{
			SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "sgReload", 0.05f);
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
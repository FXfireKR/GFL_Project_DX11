#include "stdafx.h"
#include "Ar15.h"

Ar15::Ar15()
{
	keys.name = "AR15";

	weaponType = TWT_AR;

	keys.SOUND_ATTACK = "AR15Mod_ATTACK_JP";
	keys.SOUND_BREAK = "AR15Mod_BREAK_JP";
	keys.SOUND_DEFENSE = "AR15Mod_DEFENSE_JP";
	keys.SOUND_DIALOGUE1 = "AR15Mod_DIALOGUE1_JP";
	keys.SOUND_DIALOGUE2 = "AR15Mod_DIALOGUE2_JP";
	keys.SOUND_DIALOGUE3 = "AR15Mod_DIALOGUE3_JP";
	keys.SOUND_FORMATION = "AR15Mod_FORMATION_JP";
	keys.SOUND_GOATTACK = "AR15Mod_GOATTACK_JP";
	keys.SOUND_MEET = "AR15Mod_MEET_JP";
	keys.SOUND_SKILL1 = "AR15Mod_SKILL1_JP";
	keys.SOUND_SKILL2 = "AR15Mod_SKILL2_JP";
	keys.SOUND_SKILL3 = "AR15Mod_SKILL3_JP";
	keys.SOUND_TITLECALL = "AR15Mod_TITLECALL_JP";
	keys.SOUND_WIN = "AR15Mod_WIN_JP";

	keys.vecDialogue.push_back("모두가 날 필요로 할 때, 나는 결코 망설이지 않겠어.");
	keys.vecDialogue.push_back("최근 들어 근접전 훈련 강도를 높여봤어요. 어쩌면 새로운 전술 성과가 있을수도 있으니, 지휘관도 신경 써주세요.");
	keys.vecDialogue.push_back("지휘관, 제가 어떤 모습으로 변한다고 해도... 저는 당신의 인정만을 바랄 뿐이에요.");

	mEquip.insert(make_pair(EPT_ACESORY, nullptr));		// 사이트
	mEquip.insert(make_pair(EPT_BULLET, nullptr));		// 탄환
	mEquip.insert(make_pair(EPT_ACESORY2, nullptr));	// 외골격

	//	Setting Spec
	curState.HitPoint.max = curState.HitPoint.curr = 2530;
	curState.Armor = 0;
	curState.Accuracy = 0.9;
	curState.CriticPoint = 40.0;
	curState.CriticAcl = 50;
	curState.AttackDelay = 0.8;
	curState.AimDelay = 0.59;
	curState.Avoid = 0.135;
	curState.AttackPoint = 80;
	curState.ArmorPierce = 0;

	maxState = curState;

	statusManager = new StatusManager(&curState, &maxState, &mEquip);
}

Ar15::~Ar15()
{
}

void Ar15::LoadTray_SoundList()
{
	//	Character Voice Upload
	LOAD->Add_LoadTray(keys.SOUND_ATTACK, "../../_Assets/Characters/ar15mod/AR15Mod_ATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_BREAK, "../../_Assets/Characters/ar15mod/AR15Mod_BREAK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DEFENSE, "../../_Assets/Characters/ar15mod/AR15Mod_DEFENSE_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE1, "../../_Assets/Characters/ar15mod/AR15Mod_DIALOGUE1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE2, "../../_Assets/Characters/ar15mod/AR15Mod_DIALOGUE2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE3, "../../_Assets/Characters/ar15mod/AR15Mod_DIALOGUE3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_FORMATION, "../../_Assets/Characters/ar15mod/AR15Mod_FORMATION_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_GOATTACK, "../../_Assets/Characters/ar15mod/AR15Mod_GOATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_MEET, "../../_Assets/Characters/ar15mod/AR15Mod_MEET_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL1, "../../_Assets/Characters/ar15mod/AR15Mod_SKILL1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL2, "../../_Assets/Characters/ar15mod/AR15Mod_SKILL2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL3, "../../_Assets/Characters/ar15mod/AR15Mod_SKILL3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_TITLECALL, "../../_Assets/Characters/ar15mod/AR15Mod_TITLECALL_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_WIN, "../../_Assets/Characters/ar15mod/AR15Mod_WIN_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	//	Fortrait Image Load
	LOAD->Add_LoadTray("pic_AR15MOD", "../../_Assets/Characters/ar15mod/pic_AR15MOD.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_AR15MOD_alpha", "../../_Assets/Characters/ar15mod/pic_AR15MOD_alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_AR15MOD_D", "../../_Assets/Characters/ar15mod/pic_AR15MOD_D.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_AR15MOD_D_alpha", "../../_Assets/Characters/ar15mod/pic_AR15MOD_D_alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	//LOAD->Add_LoadTray("KSG_N0", "../../_Assets/Characters/ar15mod/pic_ksg_n_0.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	//LOAD->Add_LoadTray("KSG_N1", "../../_Assets/Characters/ar15mod/pic_ksg_n_1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	keys.original_key = "pic_AR15MOD";
	keys.original_D_key = "pic_AR15MOD_D";

	keys.cardNormalKey = "KSG_N0";
	keys.cardHurtKey = "KSG_N1";
}

void Ar15::LoadTray_ImageList()
{
	this->init();
}

HRESULT Ar15::init()
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
			motion->loadSpine_FromJsonFile("AR15Mod");
			motion->setMotionAction("attack", AR15_Attack_Action);

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

	moveSpd = AR15_SPEED;

	atkColTime = 0.0;
	sklColTime = 0.0;
	TargetAngle = Angle = 0.0f;
	skillTimer = 0.0;

	isAlive = true;
	Select = false;
	moveAble = true;
	isActive = false;

	safeTrigger = 0;

	alianceType = ALIANCE_GRIFFON;
	waitAfter = false;

	return S_OK;
}

void Ar15::release()
{
	BaseTaticDoll::release();
}

void Ar15::update()
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

			sklColTime = AR15_SKILL_COLTIME;
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

void Ar15::render()
{
}

void Ar15::Use_ActiveSkill()
{
	if (!isActive)
	{
		if (sklColTime > 0.0) {}

		else
		{
			isActive = true;

			skillTimer = AR15_SKILL_MAINTAIN;

			Status skillstate;

			skillstate.AimDelay = (curState.AimDelay * 0.8f) * -1.0f;
			skillstate.AttackDelay = (curState.AttackDelay * 0.8f) * -1.0f;
			skillstate.AttackPoint = (curState.AttackPoint * 3) / 2;

			statusManager->create(skillstate, AR15_SKILL_MAINTAIN);

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

void Ar15::MotionUpdate()
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

void Ar15::Update_DrawPos()
{
	motion->p_getScale().x = AR15_ACL_X;
	motion->p_getScale().y = AR15_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0 * DELTA()) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0 * DELTA()) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void Ar15::render_VisualBar()
{
	if (isAlive)
	{
		//	HP - hit point
		Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - 150.0f), curState.HitPoint.curr,
			curState.HitPoint.max, VEC2(100, 5), ColorF(0, 0.8, 0, 0.5f));

		//	SP - skill cool down
		if (isActive)
		{
			if (skillTimer > 0.0f)
				Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - 145.0f), skillTimer,
					AR15_SKILL_MAINTAIN, VEC2(100, 5), ColorF(0.1, 0.1, 0.8, 0.5f));
		}

		else
		{
			if (sklColTime > 0.0f)
				Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - 145.0f), sklColTime,
					AR15_SKILL_COLTIME, VEC2(100, 5), ColorF(0.1, 0.1, 0.8, 0.5f));
		}

	}
}

void Ar15::render_Motion()
{
	BaseTaticDoll::render_Motion();
	motion->render();
}

void Ar15::render_Ellipse()
{
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.1, 0.8, 0.1, Select ? 0.8f : 0.1f));
}

void Ar15::AR15_Attack_Action(void * _this)
{
	Ar15* object = (Ar15*)_this;

	if (object->atkColTime < FLOAT_EPSILON)
	{
		object->moveAble = false;

		spineMotion* motion = object->motion;
		float curTime = motion->getCurTime();

		//	1
		if (curTime < 0.1333f && curTime > 0.1333f - DELTA())
		{
			if (object->safeTrigger == 0)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUND->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++object->safeTrigger;
			}
		}

		//	2
		else if (curTime < 0.19997f && curTime > 0.19997f - DELTA())
		{
			if (object->safeTrigger == 1)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUND->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++object->safeTrigger;
			}
		}

		//	3
		else if (curTime < 0.2666f && curTime > 0.2666f - DELTA())
		{
			if (object->safeTrigger == 2)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUND->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++object->safeTrigger;
			}
		}

		else if (motion->getCurTime() > motion->getEndTime())
		{
			object->atkColTime = object->curState.AttackDelay;

			if (object->TargetID != -1)
			{
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
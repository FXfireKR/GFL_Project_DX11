#include "stdafx.h"
#include "AN94.h"

AN94::AN94()
{
	name = "AN94";

	SOUND_ATTACK = "AN94_ATTACK_JP";
	SOUND_BREAK = "AN94_BREAK_JP";
	SOUND_DEFENSE = "AN94_DEFENSE_JP";
	SOUND_DIALOGUE1 = "AN94_DIALOGUE1_JP";
	SOUND_DIALOGUE2 = "AN94_DIALOGUE2_JP";
	SOUND_DIALOGUE3 = "AN94_DIALOGUE3_JP";
	SOUND_FORMATION = "AN94_FORMATION_JP";
	SOUND_GOATTACK = "AN94_GOATTACK_JP";
	SOUND_MEET = "AN94_MEET_JP";
	SOUND_SKILL1 = "AN94_SKILL1_JP";
	SOUND_SKILL2 = "AN94_SKILL2_JP";
	SOUND_SKILL3 = "AN94_SKILL3_JP";
	SOUND_TITLECALL = "AN94_TITLECALL_JP";
	SOUND_WIN = "AN94_WIN_JP";

	vecDialogue.push_back("무기란 끊임없이 혁신을 거듭하는 도구이므로, 우리도 언젠가는 대체되겠지......");
	vecDialogue.push_back("AK-12는 특별한 존재다... 그녀를 위해서라면 무엇이든 할 수 있다. 이것이 나의 사명이다.");
	vecDialogue.push_back("지휘관... 나에게... 무슨 불만이라도?");

	mEquip.insert(make_pair(EPT_ACESORY, nullptr));		// 사이트
	mEquip.insert(make_pair(EPT_BULLET, nullptr));		// 탄환
	mEquip.insert(make_pair(EPT_ACESORY2, nullptr));	// 외골격
}

AN94::~AN94()
{
}

void AN94::LoadTray_SoundList()
{
	//	Character Voice Upload
	LOADMANAGER->Add_LoadTray(SOUND_ATTACK, "../../_Assets/Characters/an94/AN94_ATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_BREAK, "../../_Assets/Characters/an94/AN94_BREAK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DEFENSE, "../../_Assets/Characters/an94/AN94_DEFENSE_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE1, "../../_Assets/Characters/an94/AN94_DIALOGUE1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE2, "../../_Assets/Characters/an94/AN94_DIALOGUE2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE3, "../../_Assets/Characters/an94/AN94_DIALOGUE3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_FORMATION, "../../_Assets/Characters/an94/AN94_FORMATION_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_GOATTACK, "../../_Assets/Characters/an94/AN94_GOATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_MEET, "../../_Assets/Characters/an94/AN94_MEET_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL1, "../../_Assets/Characters/an94/AN94_SKILL1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL2, "../../_Assets/Characters/an94/AN94_SKILL2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL3, "../../_Assets/Characters/an94/AN94_SKILL3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_TITLECALL, "../../_Assets/Characters/an94/AN94_TITLECALL_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_WIN, "../../_Assets/Characters/an94/AN94_WIN_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	//	Fortrait Image Load
	LOADMANAGER->Add_LoadTray("pic_AN94", "../../_Assets/Characters/an94/pic_AN94.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_AN94_alpha", "../../_Assets/Characters/an94/pic_AN94_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_AN94_D", "../../_Assets/Characters/an94/pic_AN94_D.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_AN94_D_alpha", "../../_Assets/Characters/an94/pic_AN94_D_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOADMANAGER->Add_LoadTray("AN94_N0", "../../_Assets/Characters/an94/pic_an94_n_0.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("AN94_N1", "../../_Assets/Characters/an94/pic_an94_n_1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	original_key = "pic_AN94";
	original_D_key = "pic_AN94_D";

	cardNormalKey = "AN94_N0";
	cardHurtKey = "AN94_N1";
}

void AN94::LoadTray_ImageList()
{
	//LOADMANAGER->Add_LoadTray("utan_1", "../../_Assets/Texture2D/Utan.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	this->init();
}

HRESULT AN94::init()
{
	TaticDoll::init();

	axisMax_LongRad = 400.0f;
	axisMax_ShortRad = 150.0f;
	axisMin_LongRad = axisMax_LongRad * 0.45f;
	axisMin_ShortRad = axisMax_ShortRad * 0.45f;

	if (motion == nullptr)
	{
		motion = new spineMotion;

		if (motion != nullptr)
		{
			motion->loadSpine_FromJsonFile("an94");
			motion->setMotionAction("attack", AN94_Attack_Action);
			//motion->setMotionAction("s", M4SopMod_Skill_Action);

			motion->setMotionObject(this);
			motion->init();
		}
	}

	else
	{
		motion->reload_SpineImage();
		motion->init();
	}

	Pos.x = 245.0f;
	Pos.y = 200.0f;

	if (mCollision.count("SELF") == 0)
		mCollision.insert(make_pair("SELF", new EllipseBase(&Pos.x, &Pos.y, 30, 5)));

	if (mCollision.count("MAX_RANGE") == 0)
		mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&Pos.x, &Pos.y, axisMax_LongRad, axisMax_ShortRad)));

	moveSpd = AN94_SPEED;

	curState.HitPoint.max = curState.HitPoint.curr = 3000;
	curState.Accuracy = 1.0;
	curState.CriticPoint = 25.5;
	curState.CriticAcl = 50;
	curState.AttackDelay = 0.8f;
	curState.AimDelay = 0.45;
	curState.Avoid = 0.835;
	curState.AttackPoint = 85;

	maxState = curState;

	atkColTime = 0.0;
	sklColTime = 0.0;
	TargetAngle = Angle = 0.0f;

	isAlive = true;
	Select = false;
	moveAble = true;

	safeTirgger = 0;

	alianceType = ALIANCE_GRIFFON;

	return S_OK;
}

void AN94::release()
{
	TaticDoll::release();
}

void AN94::update()
{
	TaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	motion->update(DELTA * DeltaAcl);
	this->MotionUpdate();
}

void AN94::render()
{
	TaticDoll::render_Motion();
	motion->render();
}

void AN94::Use_ActiveSkill()
{
}

void AN94::MotionUpdate()
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
				motion->changeMotion("attack", false, true, 0.125f);

			else if (motion->isCurrent("attack"))
			{
				if (atkColTime > 0.0)
				{
					safeTirgger = 0;
					motion->pause(-0.055);
				}
			}
		}

		else
		{
			if (motion->isCurrent("attack"))
				motion->changeMotion("wait", true, true);
		}

		if (!motion->isCurrent("attack"))
			atkColTime = curState.AimDelay;
	}
}

void AN94::Update_DrawPos()
{
	motion->p_getScale().x = AN94_ACL_X;
	motion->p_getScale().y = AN94_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0 * DELTA) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0 * DELTA) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void AN94::render_VisualBar()
{
	if (isAlive)
	{
		//	HP - hit point
		Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - 150.0f), curState.HitPoint.curr,
			curState.HitPoint.max, DV2(100, 5), ColorF(0, 0.8, 0, 0.5f));

		//	SP - skill cool down
		if (sklColTime > 0.0f)
			Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - 145.0f), sklColTime,
				AN94_SKILL_COLTIME, DV2(100, 5), ColorF(0, 0, 0.8, 0.5f));
	}
}

void AN94::render_Motion()
{
	TaticDoll::render_Motion();
	motion->render();
}

void AN94::render_Ellipse()
{
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.1, 0.8, 0.1, Select ? 0.8f : 0.1f));
}

void AN94::AN94_Attack_Action(void * _this)
{
	AN94* object = (AN94*)_this;

	if (object->atkColTime < FLOAT_EPSILON)
	{
		object->moveAble = false;

		spineMotion* motion = object->motion;
		float curTime = motion->getCurTime();

		//	1
		if (curTime < 0.1333f && curTime > 0.1333f - DELTA)
		{
			if (object->safeTirgger == 0)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.15f);
				++object->safeTirgger;
			}
		}

		//	2
		else if (curTime < 0.19997f && curTime > 0.19997f - DELTA)
		{
			if (object->safeTirgger == 1)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.15f);
				++object->safeTirgger;
			}
		}

		//	3
		else if (curTime < 0.2666f && curTime > 0.2666f - DELTA)
		{
			if (object->safeTirgger == 2)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.15f);
				++object->safeTirgger;
			}
		}

		else if (motion->getCurTime() > motion->getEndTime())
		{
			object->atkColTime = object->curState.AttackDelay;

			if (object->TargetID != -1)
				motion->changeMotion("attack", false, true, 0.15f);
			else
				motion->changeMotion("wait", true, true);

			object->safeTirgger = 0;
			object->moveAble = true;
		}

	}
}

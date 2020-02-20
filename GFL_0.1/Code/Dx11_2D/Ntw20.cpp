#include "stdafx.h"
#include "Ntw20.h"

Ntw20::Ntw20()
{
	name = "NTW20";

	SOUND_ATTACK = "NTW20_ATTACK_JP";
	SOUND_BREAK = "NTW20_BREAK_JP";
	SOUND_DEFENSE = "NTW20_DEFENSE_JP";
	SOUND_DIALOGUE1 = "NTW20_DIALOGUE1_JP";
	SOUND_DIALOGUE2 = "NTW20_DIALOGUE2_JP";
	SOUND_DIALOGUE3 = "NTW20_DIALOGUE3_JP";
	SOUND_FORMATION = "NTW20_FORMATION_JP";
	SOUND_GOATTACK = "NTW20_GOATTACK_JP";
	SOUND_MEET = "NTW20_MEET_JP";
	SOUND_SKILL1 = "NTW20_SKILL1_JP";
	SOUND_SKILL2 = "NTW20_SKILL2_JP";
	SOUND_SKILL3 = "NTW20_SKILL3_JP";
	SOUND_TITLECALL = "NTW20_TITLECALL_JP";
	SOUND_WIN = "NTW20_WIN_JP";

	vecDialogue.push_back("내 뒤에 서 있지 말아 주겠어?");
	vecDialogue.push_back("누구냐! ...뭐야, 지휘관인가. 부주의하게 만지지 마라. 상처를 입을 테니까.");
	vecDialogue.push_back("저격 임무라면 나한테 맡겨줘. 초원을 질주하는 표범일지라도, 일격에 끝내줄 테니까.");

	mEquip.insert(make_pair(EPT_ACESORY, nullptr));		// 사이트
	mEquip.insert(make_pair(EPT_BULLET, nullptr));		// 탄환
	mEquip.insert(make_pair(EPT_ACESORY2, nullptr));	// 외골격
}


Ntw20::~Ntw20()
{
}

void Ntw20::LoadTray_SoundList()
{
	//	Character Voice Upload
	LOADMANAGER->Add_LoadTray(SOUND_ATTACK, "../../_Assets/Characters/ntw20mod/NTW20_ATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_BREAK, "../../_Assets/Characters/ntw20mod/NTW20_BREAK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DEFENSE, "../../_Assets/Characters/ntw20mod/NTW20_DEFENSE_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE1, "../../_Assets/Characters/ntw20mod/NTW20_DIALOGUE1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE2, "../../_Assets/Characters/ntw20mod/NTW20_DIALOGUE2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE3, "../../_Assets/Characters/ntw20mod/NTW20_DIALOGUE3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_FORMATION, "../../_Assets/Characters/ntw20mod/NTW20_FORMATION_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_GOATTACK, "../../_Assets/Characters/ntw20mod/NTW20_GOATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_MEET, "../../_Assets/Characters/ntw20mod/NTW20_MEET_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL1, "../../_Assets/Characters/ntw20mod/NTW20_SKILL1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL2, "../../_Assets/Characters/ntw20mod/NTW20_SKILL2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL3, "../../_Assets/Characters/ntw20mod/NTW20_SKILL3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_TITLECALL, "../../_Assets/Characters/ntw20mod/NTW20_TITLECALL_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_WIN, "../../_Assets/Characters/ntw20mod/NTW20_WIN_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	//	Fortrait Image Load
	LOADMANAGER->Add_LoadTray("pic_NTW20MOD", "../../_Assets/Characters/ntw20mod/pic_NTW20Mod.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_NTW20MOD_alpha", "../../_Assets/Characters/ntw20mod/pic_NTW20Mod_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_NTW20MOD_D", "../../_Assets/Characters/ntw20mod/pic_NTW20Mod_D.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_NTW20MOD_D_alpha", "../../_Assets/Characters/ntw20mod/pic_NTW20Mod_D_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20MOD_N0", "../../_Assets/Characters/ntw20mod/pic_ntw20mod_n_0.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20MOD_N1", "../../_Assets/Characters/ntw20mod/pic_ntw20mod_n_1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	original_key = "pic_NTW20MOD";
	original_D_key = "pic_NTW20MOD_D";

	cardNormalKey = "NTW20MOD_N0";
	cardHurtKey = "NTW20MOD_N1";
}

void Ntw20::LoadTray_ImageList()
{
	this->init();
}

HRESULT Ntw20::init()
{
	TaticDoll::init();

	axisMax_LongRad = 600.0f;
	axisMax_ShortRad = 350.0f;

	axisMin_LongRad = axisMax_LongRad * 0.3f;
	axisMin_ShortRad = axisMax_ShortRad * 0.2f;

	if (motion == nullptr)
	{
		motion = new spineMotion;

		if (motion != nullptr)
		{
			motion->loadSpine_FromJsonFile("NTW20Mod");
			motion->setMotionAction("attack", NTW20_Attack_Action);
			motion->setMotionObject(this);
			motion->init();
		}
	}

	Pos.x = 150.0f;
	Pos.y = 200.0f;

	if (mCollision.count("SELF") == 0)
		mCollision.insert(make_pair("SELF", new EllipseBase(&Pos.x, &Pos.y, 30, 5)));

	if (mCollision.count("MAX_RANGE") == 0)
		mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&Pos.x, &Pos.y, axisMax_LongRad, axisMax_ShortRad)));

	if (mCollision.count("MIN_RANGE") == 0)
		mCollision.insert(make_pair("MIN_RANGE", new EllipseBase(&Pos.x, &Pos.y, axisMin_LongRad, axisMin_ShortRad)));


	moveSpd = NTW20_SPEED;

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

	safeTirgger = 0;
	sklPreShow = false;

	isAlive = true;
	Select = false;
	moveAble = true;

	alianceType = ALIANCE_GRIFFON;

	return S_OK;
}

void Ntw20::release()
{
	TaticDoll::release();
}

void Ntw20::update()
{
	TaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	motion->update(DELTA * DeltaAcl);
	this->MotionUpdate();
}

void Ntw20::render()
{
}

void Ntw20::Use_ActiveSkill()
{
	if (sklColTime < D3DX_16F_EPSILON)
	{
		motion->changeMotion("skill", false, true, 0.125f);
		//	skillSound Export 
	}
}

void Ntw20::MotionUpdate()
{
	if (TargetID != -1)
	{
		if (motion->isCurrent("wait"))
			motion->changeMotion("attack", false, true, 0.125f);

		else if (motion->isCurrent("attack"))
		{
			if (atkColTime > 0.0)
				motion->pause(-0.055);
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

void Ntw20::Update_DrawPos()
{
	motion->p_getScale().x = NTW20_ACL_X;
	motion->p_getScale().y = NTW20_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0 * DELTA) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0 * DELTA) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void Ntw20::render_VisualBar()
{
	if (isAlive)
	{
		//	HP - hit point
		Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - 150.0f), curState.HitPoint.curr,
			curState.HitPoint.max, DV2(100, 5), ColorF(0, 0.8, 0, 0.5f));
	}
}

void Ntw20::render_Motion()
{
	TaticDoll::render_Motion();
	motion->render();
}

void Ntw20::render_Ellipse()
{
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.1, 0.8, 0.1, Select ? 0.8f : 0.1f));
}

void Ntw20::NTW20_Attack_Action(void * _this)
{
	Ntw20* object = (Ntw20*)_this;

	if (object->atkColTime < FLOAT_EPSILON)
	{
		object->moveAble = false;

		spineMotion* motion = object->motion;
		float curTime = motion->getCurTime();

		//	1
		if (curTime < 0.14f && curTime > 0.14f - DELTA)
		{
			if (object->safeTirgger == 0)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 20, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect2(SOUND_CHANNEL::CH_EFFECT, "srSound1", 0.15f);
				++object->safeTirgger;
			}
		}

		else if (motion->getCurTime() > motion->getEndTime())
		{
			object->atkColTime = object->curState.AttackDelay;

			if (object->TargetID != -1)
				motion->changeMotion("attack", false, true);
			else
				motion->changeMotion("wait", true, true);

			object->safeTirgger = 0;
			object->moveAble = true;
		}

	}
}

void Ntw20::NTW20_Skill_Action(void * _this)
{
}

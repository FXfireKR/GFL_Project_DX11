#include "stdafx.h"
#include "AR15.h"

AR15::AR15()
{
	SOUND_ATTACK = "AR15Mod_ATTACK_JP";
	SOUND_BREAK = "AR15Mod_BREAK_JP";
	SOUND_DEFENSE = "AR15Mod_DEFENSE_JP";
	SOUND_DIALOGUE1 = "AR15Mod_DIALOGUE1_JP";
	SOUND_DIALOGUE2 = "AR15Mod_DIALOGUE2_JP";
	SOUND_DIALOGUE3 = "AR15Mod_DIALOGUE3_JP";
	SOUND_FORMATION = "AR15Mod_FORMATION_JP";
	SOUND_GOATTACK = "AR15Mod_GOATTACK_JP";
	SOUND_MEET = "AR15Mod_MEET_JP";
	SOUND_SKILL1 = "AR15Mod_SKILL1_JP";
	SOUND_SKILL2 = "AR15Mod_SKILL2_JP";
	SOUND_SKILL3 = "AR15Mod_SKILL3_JP";
	SOUND_TITLECALL = "AR15Mod_TITLECALL_JP";
	SOUND_WIN = "AR15Mod_WIN_JP";

	vecDialogue.push_back("모두가 날 필요로 할 때, 나는 결코 망설이지 않겠어.");
	vecDialogue.push_back("최근 들어 근접전 훈련 강도를 높여봤어요. 어쩌면 새로운 전술 성과가 있을수도 있으니, 지휘관도 신경 써주세요.");
	vecDialogue.push_back("지휘관, 제가 어떤 모습으로 변한다고 해도... 저는 당신의 인정만을 바랄 뿐이에요.");
}


AR15::~AR15()
{
}

void AR15::LoadTray_SoundList()
{
	//	Character Voice Upload
	LOADMANAGER->Add_LoadTray(SOUND_ATTACK, "../../_Assets/Characters/ar15mod/AR15Mod_ATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_BREAK, "../../_Assets/Characters/ar15mod/AR15Mod_BREAK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DEFENSE, "../../_Assets/Characters/ar15mod/AR15Mod_DEFENSE_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE1, "../../_Assets/Characters/ar15mod/AR15Mod_DIALOGUE1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE2, "../../_Assets/Characters/ar15mod/AR15Mod_DIALOGUE2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE3, "../../_Assets/Characters/ar15mod/AR15Mod_DIALOGUE3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_FORMATION, "../../_Assets/Characters/ar15mod/AR15Mod_FORMATION_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_GOATTACK, "../../_Assets/Characters/ar15mod/AR15Mod_GOATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_MEET, "../../_Assets/Characters/ar15mod/AR15Mod_MEET_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL1, "../../_Assets/Characters/ar15mod/AR15Mod_SKILL1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL2, "../../_Assets/Characters/ar15mod/AR15Mod_SKILL2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL3, "../../_Assets/Characters/ar15mod/AR15Mod_SKILL3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_TITLECALL, "../../_Assets/Characters/ar15mod/AR15Mod_TITLECALL_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_WIN, "../../_Assets/Characters/ar15mod/AR15Mod_WIN_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	//	Fortrait Image Load
	LOADMANAGER->Add_LoadTray("pic_AR15MOD", "../../_Assets/Characters/ar15mod/pic_AR15MOD.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_AR15MOD_alpha", "../../_Assets/Characters/ar15mod/pic_AR15MOD_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_AR15MOD_D", "../../_Assets/Characters/ar15mod/pic_AR15MOD_D.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_AR15MOD_D_alpha", "../../_Assets/Characters/ar15mod/pic_AR15MOD_D_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	original_key = "pic_AR15MOD";
	original_D_key = "pic_AR15MOD_D";
}

void AR15::LoadTray_ImageList()
{
	this->init();
}

HRESULT AR15::init()
{
	TaticDoll::init();

	axisMax_LongRad = 400.0f;
	axisMax_ShortRad = 150.0f;

	if (motion == nullptr)
	{
		motion = new spineMotion;

		if (motion != nullptr)
		{
			motion->loadSpine_FromJsonFile("AR15Mod");
			motion->setMotionAction("attack", AR15_Attack_Action);
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

	moveSpd = AR15_SPEED;

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

	alianceType = ALIANCE_GRIFFON;

	return S_OK;
}

void AR15::release()
{
	TaticDoll::release();
}

void AR15::update()
{
	TaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	motion->update(DELTA);
	this->MotionUpdate();
}

void AR15::render()
{
}

void AR15::Use_ActiveSkill()
{
	if (sklColTime < D3DX_16F_EPSILON)
	{
		motion->changeMotion("skill", false, true, 0.125f);
		//	skillSound Export 
	}
}

void AR15::MotionUpdate()
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

void AR15::Update_DrawPos()
{
	motion->p_getScale().x = AR15_ACL_X;
	motion->p_getScale().y = AR15_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0 * DELTA) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0 * DELTA) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void AR15::render_VisualBar()
{
}

void AR15::render_Motion()
{
	TaticDoll::render_Motion();
	motion->render();
}

void AR15::render_Ellipse()
{
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.1, 0.8, 0.1, 0.9f));
}

void AR15::AR15_Attack_Action(void * _this)
{
	AR15* object = (AR15*)_this;

	if (object->atkColTime < FLOAT_EPSILON)
	{
		object->moveAble = false;

		spineMotion* motion = object->motion;
		float curTime = motion->getCurTime();
		static SINT safeTirgger = 0;				//	3점사 유지를 위한 안전장치

		//ImGui::Text("ak12_Time : %.3f", curTime);

		//	1
		if (curTime < 0.1333f && curTime > 0.1333f - DELTA)
		{
			if (safeTirgger == 0)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++safeTirgger;
			}
		}

		//	2
		else if (curTime < 0.19997f && curTime > 0.19997f - DELTA)
		{
			if (safeTirgger == 1)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++safeTirgger;
			}
		}

		//	3
		else if (curTime < 0.2666f && curTime > 0.2666f - DELTA)
		{
			if (safeTirgger == 2)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++safeTirgger;
			}
		}

		else if (motion->getCurTime() > motion->getEndTime())
		{
			object->atkColTime = object->curState.AttackDelay;
			motion->changeMotion("attack", false, true, 0.15f);

			safeTirgger = 0;
			object->moveAble = true;
		}

	}


}

void AR15::AR15_Skill_Action(void * _this)
{
	AR15* object = (AR15*)_this;
	spineMotion* motion = object->motion;
	float curTime = motion->getCurTime();

	if (motion->getCurTime() > motion->getEndTime())
	{
		object->atkColTime = object->curState.AttackDelay;
		motion->changeMotion("wait", false, true, 0.15f);
		object->moveAble = true;
	}
}
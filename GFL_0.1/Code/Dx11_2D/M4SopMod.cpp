#include "stdafx.h"
#include "M4SopMod.h"

M4SopMod::M4SopMod()
{
	SOUND_ATTACK = "M4SOPMOD_ATTACK_JP";
	SOUND_BREAK = "M4SOPMOD_BREAK_JP";
	SOUND_DEFENSE = "M4SOPMOD_DEFENSE_JP";
	SOUND_DIALOGUE1 = "M4SOPMOD_DIALOGUE1_JP";
	SOUND_DIALOGUE2 = "M4SOPMOD_DIALOGUE2_JP";
	SOUND_DIALOGUE3 = "M4SOPMOD_DIALOGUE3_JP";
	SOUND_FORMATION = "M4SOPMOD_FORMATION_JP";
	SOUND_GOATTACK = "M4SOPMOD_GOATTACK_JP";
	SOUND_MEET = "M4SOPMOD_MEET_JP";
	SOUND_SKILL1 = "M4SOPMOD_SKILL1_JP";
	SOUND_SKILL2 = "M4SOPMOD_SKILL2_JP";
	SOUND_SKILL3 = "M4SOPMOD_SKILL3_JP";
	SOUND_TITLECALL = "M4SOPMOD_TITLECALL_JP";
	SOUND_WIN = "M4SOPMOD_WIN_JP";

	vecDialogue.push_back("지휘관~ 다음에 돌아올 땐 더 재미있는 기념품을 가지고 올게~");
	vecDialogue.push_back("감정조절이라는 게 무슨 말인지는 잘 모르겠지만, 그냥 모두 마음껏 즐거워하면 충분하잖아?");
	vecDialogue.push_back("꺄하하~ 같이 놀고 싶어? 그럼 봐주지 않을 거야!");

}

M4SopMod::~M4SopMod()
{
}

void M4SopMod::LoadTray_SoundList()
{
	//	Character Voice Upload
	LOADMANAGER->Add_LoadTray(SOUND_ATTACK, "../../_Assets/Characters/m4sopmod/M4SOPMOD_ATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_BREAK, "../../_Assets/Characters/m4sopmod/M4SOPMOD_BREAK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DEFENSE, "../../_Assets/Characters/m4sopmod/M4SOPMOD_DEFENSE_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE1, "../../_Assets/Characters/m4sopmod/M4SOPMOD_DIALOGUE1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE2, "../../_Assets/Characters/m4sopmod/M4SOPMOD_DIALOGUE2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_DIALOGUE3, "../../_Assets/Characters/m4sopmod/M4SOPMOD_DIALOGUE3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_FORMATION, "../../_Assets/Characters/m4sopmod/M4SOPMOD_FORMATION_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_GOATTACK, "../../_Assets/Characters/m4sopmod/M4SOPMOD_GOATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_MEET, "../../_Assets/Characters/m4sopmod/M4SOPMOD_MEET_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL1, "../../_Assets/Characters/m4sopmod/M4SOPMOD_SKILL1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL2, "../../_Assets/Characters/m4sopmod/M4SOPMOD_SKILL2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_SKILL3, "../../_Assets/Characters/m4sopmod/M4SOPMOD_SKILL3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_TITLECALL, "../../_Assets/Characters/m4sopmod/M4SOPMOD_TITLECALL_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray(SOUND_WIN, "../../_Assets/Characters/m4sopmod/M4SOPMOD_WIN_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	//	Fortrait Image Load
	LOADMANAGER->Add_LoadTray("pic_M4SOPMOD", "../../_Assets/Characters/m4sopmod/pic_M4SOPMOD.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_M4SOPMOD_alpha", "../../_Assets/Characters/m4sopmod/pic_M4SOPMOD_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_M4SOPMOD_D", "../../_Assets/Characters/m4sopmod/pic_M4SOPMOD_D.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("pic_M4SOPMOD_D_alpha", "../../_Assets/Characters/m4sopmod/pic_M4SOPMOD_D_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("M4SOPMOD(1)_alpha", "../../_Assets/Characters/m4sopmod/M4SOPMOD(1)_alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	original_key = "pic_M4SOPMOD";
	original_D_key = "pic_M4SOPMOD_D";
}

void M4SopMod::LoadTray_ImageList()
{
	this->init();
}

HRESULT M4SopMod::init()
{
	TaticDoll::init();

	axisMax_LongRad = 400.0f;
	axisMax_ShortRad = 150.0f;

	if (motion == nullptr)
	{
		motion = new spineMotion;

		if (motion != nullptr)
		{
			motion->loadSpine_FromJsonFile("M4SOPMOD");
			motion->setMotionAction("attack", M4SopMod_Attack_Action);
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

	moveSpd = M4SOP_SPEED;

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

void M4SopMod::release()
{
	TaticDoll::release();
}

void M4SopMod::update()
{
	TaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	motion->update(DELTA);
	this->MotionUpdate();
}

void M4SopMod::render()
{
}

void M4SopMod::Use_ActiveSkill()
{
}

void M4SopMod::MotionUpdate()
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

void M4SopMod::Update_DrawPos()
{
	motion->p_getScale().x = M4SOP_ACL_X;
	motion->p_getScale().y = M4SOP_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0 * DELTA) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0 * DELTA) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void M4SopMod::render_VisualBar()
{
}

void M4SopMod::render_Motion()
{
	TaticDoll::render_Motion();
	motion->render();
}

void M4SopMod::render_Ellipse()
{
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.1, 0.8, 0.1, 0.9f));
}

void M4SopMod::M4SopMod_Attack_Action(void * _this)
{
}

void M4SopMod::M4SopMod_Skill_Action(void * _this)
{
}

#include "stdafx.h"
#include "Ntw20.h"

Ntw20::Ntw20()
{
	keys.name = "NTW20Mod";

	weaponType = TWT_SR;

	keys.SOUND_ATTACK = "NTW20_ATTACK_JP";
	keys.SOUND_BREAK = "NTW20_BREAK_JP";
	keys.SOUND_DEFENSE = "NTW20_DEFENSE_JP";
	keys.SOUND_DIALOGUE1 = "NTW20_DIALOGUE1_JP";
	keys.SOUND_DIALOGUE2 = "NTW20_DIALOGUE2_JP";
	keys.SOUND_DIALOGUE3 = "NTW20_DIALOGUE3_JP";
	keys.SOUND_FORMATION = "NTW20_FORMATION_JP";
	keys.SOUND_GOATTACK = "NTW20_GOATTACK_JP";
	keys.SOUND_MEET = "NTW20_MEET_JP";
	keys.SOUND_SKILL1 = "NTW20_SKILL1_JP";
	keys.SOUND_SKILL2 = "NTW20_SKILL2_JP";
	keys.SOUND_SKILL3 = "NTW20_SKILL3_JP";
	keys.SOUND_TITLECALL = "NTW20_TITLECALL_JP";
	keys.SOUND_WIN = "NTW20_WIN_JP";

	keys.vecDialogue.push_back("내 뒤에 서 있지 말아 주겠어?");
	keys.vecDialogue.push_back("누구냐! ...뭐야, 지휘관인가. 부주의하게 만지지 마라. 상처를 입을 테니까.");
	keys.vecDialogue.push_back("저격 임무라면 나한테 맡겨줘. 초원을 질주하는 표범일지라도, 일격에 끝내줄 테니까.");

	mEquip.insert(make_pair(EPT_ACESORY, nullptr));		// 사이트
	mEquip.insert(make_pair(EPT_BULLET, nullptr));		// 탄환
	mEquip.insert(make_pair(EPT_ACESORY2, nullptr));	// 외골격

	curState.HitPoint.max = curState.HitPoint.curr = 3000;
	curState.Accuracy = 0.75;
	curState.CriticPoint = 25.5;
	curState.CriticAcl = 50;
	curState.AttackDelay = 1.8;
	curState.AimDelay = 2.0;
	curState.Avoid = 0.835;
	curState.AttackPoint = 210;

	maxState = curState;

	statusManager = new StatusManager(&curState, &maxState, &mEquip);
}

Ntw20::~Ntw20()
{
}

void Ntw20::LoadTray_SoundList()
{
	//	Character Voice Upload
	LOAD->Add_LoadTray(keys.SOUND_ATTACK, "Characters/ntw20mod/NTW20_ATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_BREAK, "Characters/ntw20mod/NTW20_BREAK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DEFENSE, "Characters/ntw20mod/NTW20_DEFENSE_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE1, "Characters/ntw20mod/NTW20_DIALOGUE1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE2, "Characters/ntw20mod/NTW20_DIALOGUE2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE3, "Characters/ntw20mod/NTW20_DIALOGUE3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_FORMATION, "Characters/ntw20mod/NTW20_FORMATION_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_GOATTACK, "Characters/ntw20mod/NTW20_GOATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_MEET, "Characters/ntw20mod/NTW20_MEET_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL1, "Characters/ntw20mod/NTW20_SKILL1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL2, "Characters/ntw20mod/NTW20_SKILL2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL3, "Characters/ntw20mod/NTW20_SKILL3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_TITLECALL, "Characters/ntw20mod/NTW20_TITLECALL_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_WIN, "Characters/ntw20mod/NTW20_WIN_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	//	Fortrait Image Load
	LOAD->Add_LoadTray("pic_NTW20MOD", "Characters/ntw20mod/pic_NTW20Mod.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_NTW20MOD_alpha", "Characters/ntw20mod/pic_NTW20Mod_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_NTW20MOD_D", "Characters/ntw20mod/pic_NTW20Mod_D.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_NTW20MOD_D_alpha", "Characters/ntw20mod/pic_NTW20Mod_D_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOAD->Add_LoadTray("NTW20MOD_N0", "Characters/ntw20mod/pic_ntw20mod_n_0.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("NTW20MOD_N1", "Characters/ntw20mod/pic_ntw20mod_n_1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	keys.original_key = "pic_NTW20MOD";
	keys.original_D_key = "pic_NTW20MOD_D";

	keys.cardNormalKey = "NTW20MOD_N0";
	keys.cardHurtKey = "NTW20MOD_N1";

	maxState = curState;

	statusManager = new StatusManager(&curState, &maxState, &mEquip);
}

void Ntw20::LoadTray_ImageList()
{
	LOAD->Add_LoadTray("utan_1", "Texture2D/Utan.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	this->init();
}

HRESULT Ntw20::init()
{
	BaseTaticDoll::init();

	axisMax_LongRad = 500.0f;
	axisMax_ShortRad = 185.0f;
	axisMin_LongRad = axisMax_LongRad * 0.45f;
	axisMin_ShortRad = axisMax_ShortRad * 0.45f;

	if (motion == nullptr)
	{
		motion = new spineMotion;

		if (motion != nullptr)
		{
			motion->loadSpine_FromJsonFile("NTW20Mod");
			motion->setMotionAction("attack", Ntw20_Attack_Action);
			motion->setMotionAction("s", Ntw20_Skill_Action);

			motion->setMotionObject(this);
			motion->init();
		}
	}

	else
	{
		motion->reload_SpineImage();
		motion->init();
	}

	Pos.x = 220.0f;
	Pos.y = 200.0f;

	if (mCollision.count("SELF") == 0)
		mCollision.insert(make_pair("SELF", new EllipseBase(&Pos.x, &Pos.y, 30, 5)));

	if (mCollision.count("MAX_RANGE") == 0)
		mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&Pos.x, &Pos.y, axisMax_LongRad, axisMax_ShortRad)));

	if (mCollision.count("MIN_RANGE") == 0)
		mCollision.insert(make_pair("MIN_RANGE", new EllipseBase(&Pos.x, &Pos.y, axisMin_LongRad, axisMin_ShortRad)));

	moveSpd = NTW20_SPEED;


	atkColTime = 0.0;
	sklColTime = 0.0;
	TargetAngle = Angle = 0.0f;

	isAlive = true;
	Select = false;
	moveAble = true;
	sklPreShow = false;

	safeTirgger = 0;

	alianceType = ALIANCE_GRIFFON;

	return S_OK;
}

void Ntw20::release()
{
	BaseTaticDoll::release();
}

void Ntw20::update()
{
	statusManager->update(DELTA() * DeltaAcl);

	BaseTaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	motion->update(DELTA() * DeltaAcl);
	this->MotionUpdate();

}

void Ntw20::render(){}

void Ntw20::Use_ActiveSkill()
{
	if (sklColTime > 0.0) {}
	else
		sklPreShow = !sklPreShow;
}

void Ntw20::MotionUpdate()
{
	/*static double pauseTimer = 0.0;
	static float ImGuiFort = 0.0f;
	ImGui::DragFloat("PauseTimer", &ImGuiFort, 0.001F, -0.125f, 0.5f);
	pauseTimer = static_cast<double>(ImGuiFort);*/

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
					motion->pause(-0.125);
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

void Ntw20::Update_DrawPos()
{
	motion->p_getScale().x = NTW20_ACL_X;
	motion->p_getScale().y = NTW20_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0 * DELTA()) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0 * DELTA()) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void Ntw20::render_VisualBar()
{
	if (isAlive)
	{
		//	HP - hit point
		Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - 150.0f), curState.HitPoint.curr,
			curState.HitPoint.max, VEC2(100, 5), ColorF(0, 0.8, 0, 0.5f));

		//	SP - skill cool down
		if (sklColTime > 0.0f)
			Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - 145.0f), sklColTime,
				NTW20_SKILL_COLTIME, VEC2(100, 5), ColorF(0, 0, 0.8, 0.5f));
	}
}

void Ntw20::render_Motion()
{
	BaseTaticDoll::render_Motion();
	motion->render();
}

void Ntw20::render_Ellipse()
{
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.1, 0.8, 0.1, Select ? 0.8f : 0.1f));
}

void Ntw20::Ntw20_Attack_Action(void * _this)
{
	Ntw20* object = (Ntw20*)_this;

	if (object->atkColTime < FLOAT_EPSILON)
	{
		object->moveAble = false;

		spineMotion* motion = object->motion;
		float curTime = motion->getCurTime();

		//	1
		if (curTime < 0.027f && curTime > 0.027f - DELTA())
		{
			if (object->safeTirgger == 0)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 20, object->TargetID, object->curState, object->alianceType, 2000.0f);
				SOUND->Play_Effect2(SOUND_CHANNEL::CH_EFFECT, "srSound1", 0.15f);
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

void Ntw20::Ntw20_Skill_Action(void * _this)
{
}

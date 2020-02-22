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

	vecDialogue.push_back("작전은 언제든 개시될 수 있으므로, 대비해주십시오.");
	vecDialogue.push_back("지난번 훈련 데이터가 발표되었습니까? 아직 좀 더 향상할 수 있을지도 모릅니다.");
	vecDialogue.push_back("그 손 멈추시죠. 지휘관, 저의 인내심과 반사 신경을 시험하실 필요는 없습니다.");

	mEquip.insert(make_pair(EPT_ACESORY, nullptr));		// 사이트
	mEquip.insert(make_pair(EPT_BULLET, nullptr));		// 탄환
	mEquip.insert(make_pair(EPT_ACESORY2, nullptr));	// 외골격
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

			for (int i = 0; i < 3; ++i)
			{
				motion->changeMotion("die", false, true);
				motion->changeMotion("reload", false, true);
				motion->changeMotion("wait", false, true);
				motion->changeMotion("attack", false, true);
				motion->changeMotion("pseudo_setup_pose", false, true);
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

	curState.HitPoint.max = curState.HitPoint.curr = 993000;
	curState.Armor = 9950;
	curState.Accuracy = 1.0;
	curState.CriticPoint = 25.5;
	curState.CriticAcl = 50;
	curState.AttackDelay = 2.8f;
	curState.AimDelay = 2.999;
	curState.Avoid = 0.435;
	curState.AttackPoint = 85;

	maxState = curState;

	atkColTime = 0.0;
	sklColTime = 0.0;
	TargetAngle = Angle = 0.0f;

	isAlive = true;
	Select = false;
	moveAble = true;

	safeTrigger = 0;

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
	TaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	motion->update(DELTA * DeltaAcl);
	this->MotionUpdate();
}

void Ksg::render()
{
}

void Ksg::Use_ActiveSkill()
{
}

void Ksg::MotionUpdate()
{
	static float mixTime = -0.033f;
	ImGui::DragFloat("mixTime", &mixTime, 0.01f);
	ImGui::Text("KSG Motion Timer : %.4f", motion->getCurTime());

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
				motion->changeMotion("attack", false, true, 0.001f);
				waitAfter = true;
			}

			else if (motion->isCurrent("attack"))
			{
				if (atkColTime > 0.0)
				{
					safeTrigger = 0;
					motion->pauseAt(mixTime);
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

void Ksg::Update_DrawPos()
{
	motion->p_getScale().x = KSG_ACL_X;
	motion->p_getScale().y = KSG_ACL_Y;

	//	Flip시 각도 변환량
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

		//	SP - skill cool down
		/*if (sklColTime > 0.0f)
			Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - 145.0f), sklColTime,
				M4SOP_SKILL_COLTIME, DV2(100, 5), ColorF(0, 0, 0.8, 0.5f));*/
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
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "sgSound", 0.05f);
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
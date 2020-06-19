#include "stdafx.h"
#include "nAnOne.h"

nAnOne::nAnOne()
{
	keys.SOUND_ATTACK = "9A91_ATTACK_JP";
	keys.SOUND_BREAK = "9A91_BREAK_JP";
	keys.SOUND_DEFENSE = "9A91_DEFENSE_JP";
	keys.SOUND_DIALOGUE1 = "9A91_DIALOGUE1_JP";
	keys.SOUND_DIALOGUE2 = "9A91_DIALOGUE2_JP";
	keys.SOUND_DIALOGUE3 = "9A91_DIALOGUE3_JP";
	keys.SOUND_FORMATION = "9A91_FORMATION_JP";
	keys.SOUND_GOATTACK = "9A91_GOATTACK_JP";
	keys.SOUND_MEET = "9A91_MEET_JP";
	keys.SOUND_SKILL1 = "9A91_SKILL1_JP";
	keys.SOUND_SKILL2 = "9A91_SKILL2_JP";
	keys.SOUND_SKILL3 = "9A91_SKILL3_JP";
	keys.SOUND_TITLECALL = "9A91_TITLECALL_JP";
	keys.SOUND_WIN = "9A91_WIN_JP";

	keys.vecDialogue.push_back("AS VAL과 74는 여기서 뭘 하는 걸까?");
	keys.vecDialogue.push_back("지휘관, 제 시선에서 벗어날 때에는 제대로 알려주세요?");
	keys.vecDialogue.push_back("당신에게 인정받지 못한 채였다면… 저는…");
}

nAnOne::~nAnOne()
{
}

void nAnOne::LoadTray_SoundList()
{
	//	Character Voice Upload
	LOAD->Add_LoadTray(keys.SOUND_ATTACK, "../../_Assets/Characters/9a91/9A91_ATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_BREAK, "../../_Assets/Characters/9a91/9A91_BREAK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DEFENSE, "../../_Assets/Characters/9a91/9A91_DEFENSE_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE1, "../../_Assets/Characters/9a91/9A91_DIALOGUE1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE2, "../../_Assets/Characters/9a91/9A91_DIALOGUE2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_DIALOGUE3, "../../_Assets/Characters/9a91/9A91_DIALOGUE3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_FORMATION, "../../_Assets/Characters/9a91/9A91_FORMATION_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_GOATTACK, "../../_Assets/Characters/9a91/9A91_GOATTACK_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_MEET, "../../_Assets/Characters/9a91/9A91_MEET_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL1, "../../_Assets/Characters/9a91/9A91_SKILL1_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL2, "../../_Assets/Characters/9a91/9A91_SKILL2_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_SKILL3, "../../_Assets/Characters/9a91/9A91_SKILL3_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_TITLECALL, "../../_Assets/Characters/9a91/9A91_TITLECALL_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray(keys.SOUND_WIN, "../../_Assets/Characters/9a91/9A91_WIN_JP.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	//	Fortrait Image Load
	LOAD->Add_LoadTray("pic_9A91", "../../_Assets/Characters/9a91/pic_9A91.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_9A91_alpha", "../../_Assets/Characters/9a91/pic_9A91_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_9A91_D", "../../_Assets/Characters/9a91/pic_9A91_D.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("pic_9A91_D_alpha", "../../_Assets/Characters/9a91/pic_9A91_D_Alpha.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	keys.original_key = "pic_9A91";
	keys.original_D_key = "pic_9A91_D";
}

void nAnOne::LoadTray_ImageList()
{
	if (motion == nullptr){
		motion = new spineMotion;

		if (motion != nullptr)
		{
			motion->loadSpine_FromJsonFile("9A91");
			motion->setMotionAction("attack", NA91_Attack_Action);
			motion->setMotionObject(this);
			motion->init();

			motion->changeMotion("attack", true, true, 0.125f);
			motion->changeMotion("die", true, true, 0.125f);
			motion->changeMotion("move", true, true, 0.125f);
			motion->changeMotion("wait", true, true, 0.125f);
		}
	}
}

HRESULT nAnOne::init()
{
	BaseTaticDoll::init();

	axisMax_LongRad = 400.0f;
	axisMax_ShortRad = 150.0f;

	Pos.x = 450.0f;
	Pos.y = 200.0f;

	if (mCollision.count("SELF") == 0)
		mCollision.insert(make_pair("SELF", new EllipseBase(&Pos.x, &Pos.y, 30, 5)));

	if (mCollision.count("MAX_RANGE") == 0)
		mCollision.insert(make_pair("MAX_RANGE", new EllipseBase(&Pos.x, &Pos.y, axisMax_LongRad, axisMax_ShortRad)));

	moveSpd = NA91_SPEED;

	curState.HitPoint.max = curState.HitPoint.curr = 3000;
	curState.Accuracy = 1.0;
	curState.CriticPoint = 25.5;
	curState.CriticAcl = 50;
	curState.AttackDelay = 0.8f;
	curState.AimDelay = 0.45;
	curState.Avoid = 0.835;
	curState.AttackPoint = 85;

	maxState = curState;

	atkColTime = curState.AimDelay;
	sklColTime = 0.0;
	TargetAngle = Angle = 0.0f;

	isAlive = true;
	Select = false;
	moveAble = true;

	alianceType = ALIANCE_GRIFFON;

	return S_OK;
}

void nAnOne::release()
{
	BaseTaticDoll::release();
}

void nAnOne::update()
{
	BaseTaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	this->MotionUpdate();

	motion->update(DELTA());
}

void nAnOne::render()
{
}

void nAnOne::Use_ActiveSkill()
{
}

void nAnOne::MotionUpdate()
{
	if (curState.HitPoint.curr < 1) {
		
		if (!motion->isCurrent("die"))
		{
			motion->changeMotion("die", false, true);
			isAlive = false;
		}
	}

	else{
		if (TargetID != -1) {
			
			if (motion->isCurrent("wait"))
				motion->changeMotion("attack", true, true, 0.125f);

			else if (motion->isCurrent("attack")) {

				// 0.075
				if (atkColTime > 0.0)
					motion->pause(-0.01);
			}
		}

		else {
			
			if (motion->isCurrent("attack"))
				motion->changeMotion("wait", false, true);
		}


		if (!motion->isCurrent("attack"))
			atkColTime = curState.AimDelay;
	}
}

void nAnOne::Update_DrawPos()
{
	motion->p_getScale().x = NA91_ACL_X;
	motion->p_getScale().y = NA91_ACL_Y;

	//	Flip시 각도 변환량
	Flip = (Angle > HPI && Angle < TPI) ? true : false;
	motion->p_getRotate().y = Flip ? (motion->getRotate().y < 180.0f ? motion->p_getRotate().y + (1000.0f * DELTA()) : 180.0f)
		: (motion->getRotate().y > 0.0f ? motion->p_getRotate().y - (1000.0f * DELTA()) : 0.0f);

	motion->p_getTrans().x = Pos.x;
	motion->p_getTrans().y = Pos.y;
}

void nAnOne::render_VisualBar()
{
	if (motion->isCurrent("attack"))
		Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - NA91_DOWN_Y), 
			curState.HitPoint.curr, curState.HitPoint.max, VEC2(100, 5), ColorF(RGB(0, 200, 0)));

	else
		Render_VisualBar(VEC2(Pos.x - 50.0f, Pos.y - NA91_STAND_Y),
			curState.HitPoint.curr, curState.HitPoint.max, VEC2(100, 5), ColorF(RGB(0, 200, 0)));
}

void nAnOne::render_Motion()
{
	BaseTaticDoll::render_Motion();
	motion->render();
}

void nAnOne::render_Ellipse()
{
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.1f, 0.8f, 0.1f, 0.9f));
}

void nAnOne::NA91_Attack_Action(void * _this)
{
	nAnOne* object = (nAnOne*)_this;

	if (object->atkColTime < FLOAT_EPSILON)
	{
		object->moveAble = false;

		spineMotion* motion = object->motion;
		float curTime = motion->getCurTime();

		//ImGui::Text("ak12_Time : %.3f", curTime);

		static SINT safeTirgger = 0;				//	3점사 유지를 위한 안전장치

		////	1
		//if (curTime < 0.211f && curTime > 0.211f - DELTA)
		//{
		//	if (safeTirgger == 0)
		//	{
		//		BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
		//		SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
		//		++safeTirgger;
		//	}
		//}

		//else if (curTime < 0.311f && curTime > 0.311f - DELTA)
		//{
		//	if (safeTirgger == 1)
		//	{
		//		BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
		//		SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
		//		++safeTirgger;
		//	}
		//}

		//else if (curTime < 0.411f && curTime > 0.411f - DELTA)
		//{
		//	if (safeTirgger == 2)
		//	{
		//		BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
		//		SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
		//		++safeTirgger;
		//	}
		//}

		//else if (motion->getCurTime() > motion->getEndTime())
		//{
		//	object->atkColTime = object->curState.AttackDelay;
		//	motion->changeMotion("attack", false, true, 0.15f);
		//
		//	safeTirgger = 0;
		//	object->moveAble = true;
		//}

		if (motion->getCurTime() > motion->getEndTime())
		{
			object->atkColTime = object->curState.AttackDelay;
			motion->changeMotion("attack", false, true, 0.15f);
		
			safeTirgger = 0;
			object->moveAble = true;
		}

	}
}
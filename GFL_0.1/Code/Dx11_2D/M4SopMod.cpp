#include "stdafx.h"
#include "M4SopMod.h"

M4SopMod::M4SopMod()
	:sklBullet(nullptr), sklRange(nullptr)
{
	name = "M4SopMod";

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

	mEquip.insert(make_pair(EPT_ACESORY, nullptr));		// 사이트
	mEquip.insert(make_pair(EPT_BULLET, nullptr));		// 탄환
	mEquip.insert(make_pair(EPT_ACESORY2, nullptr));	// 외골격
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

	LOADMANAGER->Add_LoadTray("M4SOPMOD_N0", "../../_Assets/Characters/m4sopmod/pic_m4sopmod_n_0.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("M4SOPMOD_N1", "../../_Assets/Characters/m4sopmod/pic_m4sopmod_n_1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	original_key = "pic_M4SOPMOD";
	original_D_key = "pic_M4SOPMOD_D";

	cardNormalKey = "M4SOPMOD_N0";
	cardHurtKey = "M4SOPMOD_N1";
}

void M4SopMod::LoadTray_ImageList()
{
	LOADMANAGER->Add_LoadTray("utan_1", "../../_Assets/Texture2D/Utan.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	this->init();
}

HRESULT M4SopMod::init()
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
			motion->loadSpine_FromJsonFile("M4SOPMOD");
			motion->setMotionAction("attack", M4SopMod_Attack_Action);
			motion->setMotionAction("s", M4SopMod_Skill_Action);
			
			motion->setMotionObject(this);
			motion->init();
		}
	}

	else
	{
		motion->reload_SpineImage();
		motion->init();
	}

	if (sklBullet == nullptr)
	{
		sklBullet = new UtanBase;
		sklBullet->init();
		sklBullet->Insert_UtanImage("utan_1");
	}

	if (sklRange == nullptr)
	{
		sklRange = new EllipseBase(&sklCenter.x, &sklCenter.y, M4SOP_SKILL_RANGE_X, M4SOP_SKILL_RANGE_Y);
	}

	Pos.x = 220.0f;
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
	sklPreShow = false;

	safeTirgger = 0;

	alianceType = ALIANCE_GRIFFON;

	return S_OK;
}

void M4SopMod::release()
{
	TaticDoll::release();

	if (sklRange != nullptr)
	{
		sklRange->Release_Ellipse();
		SAFE_DELETE(sklRange);
	}

	if (sklBullet != nullptr)
	{
		sklBullet->release();
		SAFE_DELETE(sklBullet);
	}
}

void M4SopMod::update()
{
	TaticDoll::update();

	update_Coltime();
	this->Update_DrawPos();
	motion->update(DELTA * DeltaAcl);
	this->MotionUpdate();
	utanUpdate();
	skilUpdate();
}

void M4SopMod::render()
{
}

void M4SopMod::Use_ActiveSkill()
{
	if (sklColTime > 0.0) {}
	else
		sklPreShow = !sklPreShow;
}

void M4SopMod::MotionUpdate()
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
	if (isAlive)
	{
		//	HP - hit point
		Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - 150.0f), curState.HitPoint.curr,
			curState.HitPoint.max, DV2(100, 5), ColorF(0, 0.8, 0, 0.5f));

		//	SP - skill cool down
		if (sklColTime > 0.0f)
			Render_VisualBar(DV2(Pos.x - 50.0f, Pos.y - 145.0f), sklColTime,
				M4SOP_SKILL_COLTIME, DV2(100, 5), ColorF(0, 0, 0.8, 0.5f));
	}
}

void M4SopMod::render_Motion()
{
	TaticDoll::render_Motion();
	motion->render();
} 

void M4SopMod::render_Ellipse()
{
	for (auto& iterCollition : mCollision)
		iterCollition.second->Rend_Ellipse(D3DXCOLOR(0.1, 0.8, 0.1, Select ? 0.8f : 0.1f));

	utanRender();	
}

void M4SopMod::utanUpdate()
{
	if (sklPreShow)
	{
		sklCenter.x = g_ptMouse.x + CameraPositionX;
		sklCenter.y = g_ptMouse.y - CameraPositionY;

		sklRange->Update_Ellipse();

		DeltaAcl = DeltaAcl > 0.2f ? DeltaAcl -= DELTA : 0.2f;
	}

	else
		DeltaAcl = DeltaAcl < 1.0f ? DeltaAcl += DELTA : 1.0f;

	if (sklBullet->getFired())
	{
		sklBullet->update();

		if (sklBullet->getArrived())
		{
			for (size_t i = 0; i < BDATA->getObjectVector().size(); ++i)
			{
				TaticDoll* iter = BDATA->getObject(i);
				if (iter->getAlive())
				{
					if (sklRange->EllipseCollision_Check(iter->getCollision("SELF")))
					{
						Status skillState = curState;
						skillState.AttackPoint *= 3;
						iter->Character_GetDamage(skillState);
					}
				}
			}

			sklBullet->reset();
		}
	}
}

void M4SopMod::utanRender()
{
	if (sklPreShow)
		sklRange->Rend_Ellipse(D3DXCOLOR(0.1, 0.8, 0.1, 0.7f));

	if (sklBullet->getFired())
	{
		sklRange->Rend_Ellipse(D3DXCOLOR(0.1, 0.8, 0.1, 0.7f));
		sklBullet->render(M4SOP_SKILL_RENDSCALE);
	}
}

void M4SopMod::skilUpdate()
{
	if (sklPreShow && Select)
	{
		if (KEYMANAGER->isKeyStayDown(VK_RBUTTON))
		{
			if (mCollision["MAX_RANGE"]->PointCollision_Check(sklCenter.x, sklCenter.y))
			{
				sklPreShow = false;

				Angle = Pos.x < sklCenter.x ? 0.0f : PI;

				if (!motion->isCurrent("s"))
				{
					switch (rand() % 3)
					{
					case 0:
						SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_VOICE, SOUND_SKILL1, 0.25f);
						break;

					case 1:
						SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_VOICE, SOUND_SKILL2, 0.25f);
						break;

					case 2:
						SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_VOICE, SOUND_SKILL2, 0.25f);
						break;
					}

					motion->changeMotion("s", false, true);
				}
			}
		}
	}

	else if (!Select)
	{
		sklPreShow = false;
	}

}

void M4SopMod::M4SopMod_Attack_Action(void * _this)
{
	M4SopMod* object = (M4SopMod*)_this;

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
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++object->safeTirgger;
			}
		}

		//	2
		else if (curTime < 0.19997f && curTime > 0.19997f - DELTA)
		{
			if (object->safeTirgger == 1)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
				++object->safeTirgger;
			}
		}

		//	3
		else if (curTime < 0.2666f && curTime > 0.2666f - DELTA)
		{
			if (object->safeTirgger == 2)
			{
				BULLET->CreateBullet("AR_BLT", object->Pos.x, object->Pos.y - 65, object->TargetID, object->curState, object->alianceType, 1100.0f);
				SOUNDMANAGER->Play_Effect(SOUND_CHANNEL::CH_EFFECT, "arSound", 0.05f);
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

void M4SopMod::M4SopMod_Skill_Action(void * _this)
{
	M4SopMod* object = (M4SopMod*)_this;
	spineMotion* motion = object->motion;
	float curTime = motion->getCurTime();

	object->moveAble = false;

	if (curTime < 1.005f && curTime > 1.005f - DELTA)
	{
		if (object->Pos.x < object->sklCenter.x)
			object->sklBullet->Create_Utan(object->Pos.x, object->Pos.y - 65.0f,
				object->sklCenter.x, object->sklCenter.y);
		else
			object->sklBullet->Create_Utan(object->Pos.x, object->Pos.y - 65.0f,
				object->sklCenter.x, object->sklCenter.y, 135.0f);
	}

	else if (motion->getCurTime() > motion->getEndTime())
	{
		object->sklColTime = 0.5f;	//10.5f
		object->atkColTime = object->curState.AttackDelay;

		motion->changeMotion("wait", true, true);

		object->safeTirgger = 0;
		object->moveAble = true;
	}
}

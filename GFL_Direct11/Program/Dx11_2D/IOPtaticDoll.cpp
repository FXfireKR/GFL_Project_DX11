#include "stdafx.h"
#include "IOPtaticDoll.h"

IOPtaticDoll::IOPtaticDoll()
{
	static int seed = 0;
	myID.All_ID = seed;
	++seed;
}

IOPtaticDoll::~IOPtaticDoll()
{
}

void IOPtaticDoll::LoadTray_List()
{
}
HRESULT IOPtaticDoll::init()
{
	TaticDollBase::init();

	//sound = new SoundManager;
	//sound->init_Manager();

	//sound->Insert_Sound("WALK", "../SoundData/Walk.wav");
	//sound->Insert_Sound("FIRE", "../SoundData/BT_AK.wav");
	//sound->Insert_Sound("FIRE2", "../SoundData/BT_loop.wav");
	//sound->Insert_Sound("FIRE3", "../SoundData/SR_ATK.wav");

	//sound->Insert_Sound("EXPL1", "../resSoundData/Battle_141.wav");
	//sound->Insert_Sound("EXPL2", "../resSoundData/Battle_142.wav");

	myID.SquadMem_ID = 0;
	myID.Squad_ID = 0;

	Focus = false;
	Target_ID = -1;

	return S_OK;
}

void IOPtaticDoll::release()
{
	TaticDollBase::release();
}

void IOPtaticDoll::update()
{
	TaticDollBase::update();

	float dAngle = DGR(Angle);

	if (dAngle > 90.0f && dAngle < 270.0f)
		Flip = true;
	else
		Flip = false;
	

	//쿨타임 재사용 대기시간
	AtkColTime = AtkColTime > 0.0f ? AtkColTime -= DELTA : AtkColTime = 0.0f;
	SklColTime = SklColTime > 0.0f ? SklColTime -= DELTA : SklColTime = 0.0f;


	if (isAlive)
	{
		MoveClickPoint();
		IsEnemy_at();
		Set_Targetting_Angle();
		Unit_CollitionCheck();
	}
	else
		Target_ID = -1;

	MotionUpdate();
	
}

void IOPtaticDoll::render()
{
	TaticDollBase::render();
}

void IOPtaticDoll::Update_DrawPos()
{
}

void IOPtaticDoll::render_VisualBar()
{
}

void IOPtaticDoll::render_Motion()
{
}

void IOPtaticDoll::render_Ellipse()
{
	if (isAlive)
	{
		for (auto& it : mCollision)
		{
			if (Select)
				it.second->Rend_Ellipse(D3DXCOLOR(0.0F, 1.0F, 0.0F, 0.8F));

			else
				it.second->Rend_Ellipse(D3DXCOLOR(0.0F, 1.0F, 0.0F, 0.1F));
		}


	/*	if (motion->isCurrent("WALK"))
			DRAW->render_Line(cPos.x - BDATA->getVirtualPos().x, cPos.y - BDATA->getVirtualPos().y,
				msPointX - BDATA->getVirtualPos().x, msPointY - BDATA->getVirtualPos().y, ColorF(0.0F, 0.0F, 150.0F));

		for (auto it = vPath.begin(); it < vPath.end(); ++it)
			if (vPath.begin() == it)
				DRAW->render_Line(msPointX - BDATA->getVirtualPos().x, msPointY - BDATA->getVirtualPos().y, it->x - BDATA->getVirtualPos().x, it->y - BDATA->getVirtualPos().y, ColorF(0.0F, 0.0F, 150.0F));
			else
				DRAW->render_Line((it - 1)->x - BDATA->getVirtualPos().x, (it - 1)->y - BDATA->getVirtualPos().y, it->x - BDATA->getVirtualPos().x, it->y - BDATA->getVirtualPos().y, ColorF(0.0F, 0.0F, 150.0F));*/
	}
}

void IOPtaticDoll::MotionUpdate()
{
}

void IOPtaticDoll::AttachEquipment()
{
	state = maxState;

	//for (auto& it : mEquip)
	//{
	//	if (it.second == nullptr) continue;

	//	auto& atchState = it.second->getState();

	//	state.HitPoint += atchState.HitPoint;
	//	state.AttackDelay -= (state.AttackDelay * atchState.AttackDelay) * 0.01f;
	//	state.AttackPoint += atchState.AttackPoint;
	//	state.CriticPoint += atchState.CriticPoint;
	//}

	////옵티컬일때 최소사거리를 주고, 사거리를 증가시킴
	//if (mEquip.count(EPT_ACESORY))
	//{
	//	if (mEquip.find(EPT_ACESORY)->second != nullptr)
	//	{
	//		if (mEquip.find(EPT_ACESORY)->second->getItemType() == EPC_OPTICAL)
	//		{
	//			auto& maxRange = mCollision.find("MAX_RANGE")->second;

	//			float l, s;
	//			l = maxRange->getLongRad() + (maxRange->getLongRad() * 0.25f);
	//			s = maxRange->getShortRad() + (maxRange->getShortRad() * 0.125f);

	//			if (!mCollision.count("MIN_RANGE"))
	//				mCollision.insert(make_pair("MIN_RANGE", new EllipseBase(&cPos.x, &cPos.y, maxRange->getLongRad() * 0.5f, maxRange->getShortRad() * 0.4f)));

	//			maxRange->ChangeLadius(l, s);
	//		}
	//	}
	//}

	////연장총열일때 사거리를 증가시킴
	//if (mEquip.count(EPT_ACESORY2))
	//{
	//	if (mEquip.find(EPT_ACESORY2)->second != nullptr)
	//	{
	//		if (mEquip.find(EPT_ACESORY2)->second->getItemType() == EPC_EXTBAR)
	//		{
	//			auto& maxRange = mCollision.find("MAX_RANGE")->second;

	//			float l, s;
	//			l = maxRange->getLongRad() + (maxRange->getLongRad() * 0.15f);
	//			s = maxRange->getShortRad() + (maxRange->getShortRad() * 0.05f);

	//			maxRange->ChangeLadius(l, s);
	//		}
	//	}
	//}
}

void IOPtaticDoll::Use_ActiveSkill()
{
}

void IOPtaticDoll::Unit_CollitionCheck()
{
	//	1. 자신과 맞닿아있는 유닛을 찾아낸다. [ 적군 ]
	if (mCollision.count("SELF")) 
	{
		// IOP Verse SNV
		int e_SquadID = BDATA->getEngageSquadID();
		for (int i = 1; i < BDATA->getEnemySquad(e_SquadID)->mSquad.size() + 1; ++i)
		{
			auto enemy = BDATA->getEnemyMember(e_SquadID, i);

			//죽어있으면 넘겨버림
			if (!enemy->getAlive())
				continue;			

			//충돌박스가 이미 맞닿아 있다면
			else if (mCollision.find("SELF")->second->EllipseCollision_Check(enemy->getCollision("SELF")))
			{
				cPos.x -= cosf(Angle) * Speed * DELTA; //* 1.2f;
				cPos.y += sinf(Angle) * Speed * DELTA; //* 1.2f;
			}
		}


		//IOP Verse IOP
		e_SquadID = PLAYER->getCurrentSquad();
		for (int i = 1; i < PLAYER->getPlayerSquad(e_SquadID)->mSquad.size() + 1; ++i)
		{
			if (i == myID.SquadMem_ID) continue;
			
			auto enemy = PLAYER->getIOPdoll_crntSquad(i);

			//죽어있으면 넘겨버림
			if (!enemy->getAlive())
				continue;		

			//충돌박스가 이미 맞닿아 있다면
			else if (mCollision.find("SELF")->second->EllipseCollision_Check(enemy->getCollision("SELF")))
			{
				cPos.x -= cosf(Angle) * Speed * DELTA * 2.0f;
				cPos.y += sinf(Angle) * Speed * DELTA * 2.0f;
			}
		}

	}

}

void IOPtaticDoll::StopMoving()
{
	if (motion->isCurrent("WALK"))
	{
		//sound->Stop_Sound("WALK");
		motion->Change_Motion("IDLE");

		if (vPath.size() > 0)
			vPath.clear();
	}
}

void IOPtaticDoll::SetMovePoint()
{
	//마우스가 눌렸을때 호출되어, 마우스 포인터로 위치를 지정해준다.
	//SHIFT가 활성화 상태이면, 지정된 위치를 저장해준다.
	if (isAlive)
	{
		if (KEYMANAGER->isKeyStayDown(VK_LSHIFT))
		{
			FPNT temp;
			temp.x = (float)_ptMouse.x + VPOS->x;
			temp.y = (float)_ptMouse.y + VPOS->y;

			if (temp.y < CHARACTERPOS_LIM_Y)
				temp.y = CHARACTERPOS_LIM_Y;

			vPath.push_back(temp);

			if (!motion->isCurrent("WALK"))
			{
				msPointX = vPath.begin()->x;
				msPointY = vPath.begin()->y;
				Angle = getAngle(cPos.x, cPos.y, msPointX, msPointY);

				if (moveAble)
					motion->Change_Motion("WALK");
			}

		}

		else
		{
			vPath.clear();

			msPointX = (double)_ptMouse.x + VPOS->x;
			msPointY = (double)_ptMouse.y + VPOS->y;

			if (msPointY < CHARACTERPOS_LIM_Y)
				msPointY = CHARACTERPOS_LIM_Y;


			Angle = getAngle(cPos.x, cPos.y, msPointX, msPointY);

			if (!motion->isCurrent("WALK"))
				if (moveAble)
					motion->Change_Motion("WALK");
		}

	}
}

void IOPtaticDoll::MoveClickPoint()
{
	if (motion->isCurrent("WALK"))
	{
		float Spd = 0.0f;
		float s_angle = sinf(Angle);
		float c_angle = cosf(Angle);

		if (s_angle >= 0)
			Spd = Speed - (s_angle * 0.5f)*Speed;

		else
			Spd = Speed + (s_angle * 0.5f)*Speed;

		Spd *= DELTA;

		//삼각함수 계산
		cPos.x = cPos.x + (Spd * c_angle);
		cPos.y = cPos.y - (Spd * s_angle);

		//sound->Play_Sound("WALK");


		if (cPos.x >= msPointX - (Spd * 0.5f) && cPos.x <= msPointX + (Spd * 0.5f))
			if (cPos.y >= msPointY - (Spd * 0.5f) && cPos.y <= msPointY + (Spd * 0.5f))
			{
				if (vPath.size() <= 0)
				{
					//sound->Stop_Sound("WALK");
					motion->Change_Motion("IDLE");
				}

				else
				{
					msPointX = vPath.begin()->x;
					msPointY = vPath.begin()->y;
					Angle = getAngle(cPos.x, cPos.y, msPointX, msPointY);
					vPath.erase(vPath.begin());
				}
			}

	}

}

bool IOPtaticDoll::MovePoint(float* nx, float* ny, float x, float y)
{

	/*if (motion->getNowMotion() != WALK_M)
		motion->Change_Motion(WALK_M);

	sound->Play_Sound("Walk");

	float Angle = getAngle(*nx, *ny, x, y);

	float s_angle = sinf(Angle);
	float c_angle = cosf(Angle);

	*nx += (2.5f * c_angle);
	*ny -= (2.5f * s_angle);

	if (*nx >= x - 2.0f && *nx <= x + 2.0f)
		if (*ny >= y - 2.0f && *ny <= y + 2.0f)
		{
			motion->Change_Motion(IDLE_M);
			sound->Stop_Sound("Walk");

			*nx = x + 4.0f;
			*ny = y - 2.0f;

			return true;
		}

	return false;*/
	return false;
}

void IOPtaticDoll::MovePoint(float x, float y)
{
	msPointX = x;
	msPointY = y;

	Angle = getAngle(cPos.x, cPos.y, msPointX, msPointY);

	if (moveAble)
		if (!motion->isCurrent("WALK"))
			motion->Change_Motion("WALK");
}

void IOPtaticDoll::Camera_Focus()
{
	if (Focus)
	{
		VPOS->x = cPos.x - (WINSIZEX * 0.5f);
		VPOS->y = cPos.y - (WINSIZEY * 0.5f);

		BDATA->VirtualPos_Limit(300);
	}
}

void IOPtaticDoll::IsEnemy_at()
{
	/*
		기존 공격범위에 의한 공격
	*/

	int e_SquadID = BDATA->getEngageSquadID();

	//사정거리가 존재한다면
	if (mCollision.count("MAX_RANGE"))
	{
		for (int i = 1; i < BDATA->getEnemySquad(e_SquadID)->mSquad.size() + 1; ++i)
		{
			auto enemy = BDATA->getEnemyMember(e_SquadID, i);

			//죽어있으면 넘겨버림
			if (!enemy->getAlive())
			{
				if (Target_ID == enemy->getID()->SquadMem_ID)
				{
					if (getEnemy_ID_Pos(Target_ID) != -1)
						vRange.erase(vRange.begin() + getEnemy_ID_Pos(Target_ID));
					Target_ID = -1;
				}
				continue;
			}

			//이미 사거리내에 있는 적인가?
			if (FindEnemy_ID(i))
			{
				if (!mCollision.find("MAX_RANGE")->second->EllipseCollision_Check(enemy->getCollision("SELF")))
				{
					vRange.erase(vRange.begin() + getEnemy_ID_Pos(i));
					if (Target_ID == enemy->getID()->SquadMem_ID)
						Target_ID = -1;
				}
				continue;
			}

			//적군의 SELF와 최대 사거리가 접하고있다면 , 살아있다면
			if (mCollision.find("MAX_RANGE")->second->EllipseCollision_Check(enemy->getCollision("SELF")) && enemy->getAlive())
			{
				if (mCollision.count("MIN_RANGE"))
				{
					if (!mCollision.find("MIN_RANGE")->second->PointCollision_Check(enemy->getCharacterPos().x, enemy->getCharacterPos().y))
						vRange.push_back(i);
				}
				else
					vRange.push_back(i);
			}
		}
	}


	Set_MinTargetting();

	Set_Targetting();
}

void IOPtaticDoll::Set_MinTargetting()
{
	//최소 사거리가 존재한다면
	if (mCollision.count("MIN_RANGE"))
	{
		int e_SquadID = BDATA->getEngageSquadID();

		//사거리 내의 적으로 판단한다.
		for (auto& it : vRange)
		{
			if (mCollision.find("MIN_RANGE")->second->PointCollision_Check(BDATA->getEnemyMember(e_SquadID, it)->getCollision("SELF")->getCenterX(),
				BDATA->getEnemyMember(e_SquadID, it)->getCollision("SELF")->getCenterY()))
			{
				if (getEnemy_ID_Pos(it) != -1)
				{
					vRange.erase(vRange.begin() + getEnemy_ID_Pos(it));

					if (Target_ID == (int)it)
						Target_ID = -1;
				}
			}
		}
	}
}

void IOPtaticDoll::Set_Targetting_Angle()
{
	//Target_ID는 분대 멤버중 몇번째인지이다.
	//타겟이 존재하고, 이동중이 아니라면
	if (Target_ID != -1 && !motion->isCurrent("WALK"))
	{
		auto it = BDATA->getEnemyMember(BDATA->getEngageSquadID(), Target_ID);

		if (it != nullptr)
			Angle = getAngle(cPos.x, cPos.y, it->getCharacterPos().x, it->getCharacterPos().y);
		else
			it = nullptr;
	}
}

void IOPtaticDoll::Set_Targetting_Other(UINT id)
{
	if (FindEnemy_ID(id))
		Target_ID = id;
}

bool IOPtaticDoll::FindEnemy_ID(UINT id)
{
	for (auto& it : vRange)
		if (it == id)
			return true;
	return false;
}

int IOPtaticDoll::getEnemy_ID_Pos(UINT id)
{
	int count = 0;

	for (auto& it : vRange)
	{
		if (it == id)
			return count;

		++count;
	}

	return -1;
}

void IOPtaticDoll::Set_Targetting()
{
	if (vRange.size() < 1)
		Target_ID = -1;
	else
	{
		if (!FindEnemy_ID(Target_ID))
			Target_ID = -1;

		if (Target_ID == -1)
			Target_ID = vRange.at(0);
	}

}
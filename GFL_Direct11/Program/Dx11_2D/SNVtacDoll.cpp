#include "stdafx.h"
#include "SNVtacDoll.h"

SNVtacDoll::SNVtacDoll()
{
}

SNVtacDoll::~SNVtacDoll()
{
}

void SNVtacDoll::LoadTray_List()
{
}

HRESULT SNVtacDoll::init()
{
	TaticDollBase::init();

	static int Seed = 0;		//적군의 ID를 부여하기위한 SEED생성

	myID.SquadMem_ID = 0;
	myID.Squad_ID = 0;
	myID.All_ID = Seed;
	Target_ID = -1;

	targTeam = GRIFFON_TACDOLL;

	++Seed;

	return S_OK;
}

void SNVtacDoll::release()
{
	TaticDollBase::release();
}

void SNVtacDoll::update()
{
	TaticDollBase::update();

	float dAngle = DGR(Angle);
	if (dAngle > 90.0f && dAngle < 270.0f)
		Flip = false;
	else
		Flip = true;

	//쿨타임 재사용 대기시간
	AtkColTime = AtkColTime > 0.0f ? AtkColTime -= DELTA : AtkColTime = 0.0f;
	SklColTime = SklColTime > 0.0f ? SklColTime -= DELTA : SklColTime = 0.0f;

	if (isAlive)
	{
		IsEnemy_at();
		Set_Targetting_Angle();
		MoveToSetPoint();
	}

	MotionUpdate();
}

void SNVtacDoll::render()
{
	TaticDollBase::render();
}

void SNVtacDoll::render_VisualBar()
{
}

void SNVtacDoll::render_Motion()
{
}

void SNVtacDoll::render_Ellipse()
{
	if (isAlive)
		for (auto& it : mCollision)
		{
			if (targTeam == GRIFFON_TACDOLL)
				it.second->Rend_Ellipse(D3DXCOLOR(1.0F, 0.0F, 0.0F, 0.4F));

			else
				it.second->Rend_Ellipse(D3DXCOLOR(0.0F, 1.0F, 0.0F, 0.4F));
		}
}

void SNVtacDoll::MotionUpdate()
{
}

void SNVtacDoll::IsEnemy_at()
{
	/*
		기존 공격범위에 의한 공격
	*/

	//사정거리가 존재한다면
	if (mCollision.count("MAX_RANGE"))
	{
		if (targTeam == GRIFFON_TACDOLL)
		{
			//GFL분대 판별
			int e_SquadID = PLAYER->getCurrentSquad();
			for (int i = 1; i < PLAYER->getPlayerSquad(e_SquadID)->mSquad.size() + 1; ++i)
			{
				auto enemy = PLAYER->getIOPdoll_crntSquad(i);

				//죽어있으면 넘겨버림
				if (!enemy->getAlive())
				{
					if (Target_ID == enemy->getID()->SquadMem_ID)
					{
						if (getEnemy_ID_Pos(Target_ID) != -1)
						{
							vRange.erase(vRange.begin() + getEnemy_ID_Pos(Target_ID));
							Target_ID = -1;
						}
					}
					continue;
				}

				//이미 사거리내에 있는 적인가?
				if (FindEnemy_ID(i))
				{
					if (!mCollision.find("MAX_RANGE")->second->EllipseCollision_Check(enemy->getCollision("SELF")))
						vRange.erase(vRange.begin() + getEnemy_ID_Pos(i));
					continue;
				}

				//적군의 SELF와 최대 사거리가 접하고있다면
				if (mCollision.find("MAX_RANGE")->second->EllipseCollision_Check(enemy->getCollision("SELF")))
					vRange.push_back(i);
			}

			//아군중, 해킹당한 아군 확인
			e_SquadID = BDATA->getEngageSquadID();
			for (int i = 1; i < BDATA->getEnemySquad(e_SquadID)->mSquad.size() + 1; ++i)
			{
				if (i == myID.SquadMem_ID)continue;
				auto enemy = BDATA->getEnemyMember(e_SquadID, i);

				if (!BDATA->getCur_EnemyMember(i)->getAlive()) {

					if (Target_ID > 99 && Target_ID - 100 == enemy->getID()->SquadMem_ID) {
						vRange.erase(vRange.begin() + getEnemy_ID_Pos(Target_ID));		//해킹이 풀렸는지 여부를 확인한다
						Target_ID = -1;
					}
					continue;
				}

				if (enemy->getTargetOS() != SANGVIS_TACDOLL) {
					if (getEnemy_ID_Pos(Target_ID) != -1 && Target_ID > 99 && Target_ID - 100 == enemy->getID()->SquadMem_ID)
					{
						vRange.erase(vRange.begin() + getEnemy_ID_Pos(Target_ID));		//해킹이 풀렸는지 여부를 확인한다
						Target_ID = -1;
						continue;
					}
				}

				else
				{
					//죽어있으면 넘겨버림
					if (!enemy->getAlive())
					{
						if ((Target_ID) == enemy->getID()->SquadMem_ID)
						{
							if (getEnemy_ID_Pos(Target_ID) != -1)
							{
								vRange.erase(vRange.begin() + getEnemy_ID_Pos(Target_ID));		//10X면 해킹당한 아군을 공격한다.
								Target_ID = -1;
							}
						}
						continue;
					}

					//이미 사거리내에 있는 적인가?
					if (FindEnemy_ID(i + 100))
					{
						if (!mCollision.find("MAX_RANGE")->second->EllipseCollision_Check(enemy->getCollision("SELF")))
							vRange.erase(vRange.begin() + getEnemy_ID_Pos(i + 100));
						continue;
					}

					//적군의 SELF와 최대 사거리가 접하고있다면
					if (mCollision.find("MAX_RANGE")->second->EllipseCollision_Check(enemy->getCollision("SELF")))
						vRange.push_back(i + 100);
				}
			}
		}

		//전자전 공격을 받아서 상대팀으로 합류되었을 경우
		else
		{
			int e_SquadID = BDATA->getEngageSquadID();
			for (int i = 1; i < BDATA->getEnemySquad(e_SquadID)->mSquad.size() + 1; ++i)
			{
				//자기자신이 나올경우 패스
				if (i == myID.SquadMem_ID)continue;

				auto enemy = BDATA->getEnemyMember(e_SquadID, i);

				//죽어있으면 넘겨버림
				if (!enemy->getAlive())
				{
					if (Target_ID == enemy->getID()->SquadMem_ID)
					{
						if (getEnemy_ID_Pos(Target_ID) != -1)
						{
							vRange.erase(vRange.begin() + getEnemy_ID_Pos(Target_ID));
							Target_ID = -1;
						}
					}
					continue;
				}

				//이미 사거리내에 있는 적인가?
				if (FindEnemy_ID(i))
				{
					if (!mCollision.find("MAX_RANGE")->second->EllipseCollision_Check(enemy->getCollision("SELF")))
						vRange.erase(vRange.begin() + getEnemy_ID_Pos(i));
					continue;
				}

				//적군의 SELF와 최대 사거리가 접하고있다면
				if (mCollision.find("MAX_RANGE")->second->EllipseCollision_Check(enemy->getCollision("SELF")))
					vRange.push_back(i);
			}
		}
	}

	Set_Targetting();
}

bool SNVtacDoll::FindEnemy_ID(UINT id)
{
	for (auto& it : vRange)
		if (it == id)
			return true;
	return false;
}

int SNVtacDoll::getEnemy_ID_Pos(UINT id)
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

void SNVtacDoll::Set_Targetting()
{
	if (vRange.size() < 1)
		Target_ID = -1;
	else
	{
		if (Target_ID == -1)
			Target_ID = *(vRange.begin());

		else
		{
			//올바른 타겟팅인지 체킹한다.
			if (!FindEnemy_ID(Target_ID))
				Target_ID = -1;
		}
	}
}

void SNVtacDoll::Set_Targetting_Angle()
{
	//Target_ID는 분대 멤버중 몇번째인지이다.
	//타겟이 존재하고, 이동중이 아니라면
	if (Target_ID != -1 && !motion->isCurrent("WALK"))
	{
		if (targTeam == GRIFFON_TACDOLL)
		{
			if (Target_ID < 100)
			{
				auto it = PLAYER->getIOPdoll_crntSquad(Target_ID);
				Angle = rGetAngle(cPos.x, cPos.y, it->getCharacterPos().x, it->getCharacterPos().y);
			}

			else
			{
				auto it = BDATA->getEnemyMember(BDATA->getEngageSquadID(), Target_ID - 100);
				Angle = rGetAngle(cPos.x, cPos.y, it->getCharacterPos().x, it->getCharacterPos().y);
			}
		}

		else
		{
			auto it = BDATA->getEnemyMember(BDATA->getEngageSquadID(), Target_ID);
			Angle = rGetAngle(cPos.x, cPos.y, it->getCharacterPos().x, it->getCharacterPos().y);
		}
	}
}

void SNVtacDoll::SetMovePoint(int x, int y)
{
	if (isAlive)
	{
		mvPointX = (double)x;
		mvPointY = (double)y;

		if (mvPointY < CHARACTERPOS_LIM_Y)
			mvPointY = CHARACTERPOS_LIM_Y;


		Angle = rGetAngle(cPos.x, cPos.y, mvPointX, mvPointY);

		if (moveAble)
		{
			if (!motion->isCurrent("WALK"))
				motion->Change_Motion("WALK");
		}
	}
}

void SNVtacDoll::MoveToSetPoint()
{
	if (motion->isCurrent("WALK"))
	{
		float Spd = 0.0f;
		float s_angle = sinf(Angle);
		float c_angle = cosf(Angle);

		if (s_angle >= 0)
			Spd = Speed - (s_angle / 2)*Speed;

		else
			Spd = Speed + (s_angle / 2)*Speed;


		//삼각함수 계산
		cPos.x = cPos.x + (Spd * c_angle);
		cPos.y = cPos.y - (Spd * s_angle);


		if (cPos.x >= mvPointX - 2.0f && cPos.x <= mvPointX + 2.0f)
			if (cPos.y >= mvPointY - 2.0f && cPos.y <= mvPointY + 2.0f)
			{
				//sound->Stop_Sound("WALK");
				motion->Change_Motion("IDLE");
			}

	}
}
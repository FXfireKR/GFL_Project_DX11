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

	static int Seed = 0;		//������ ID�� �ο��ϱ����� SEED����

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

	//��Ÿ�� ���� ���ð�
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
		���� ���ݹ����� ���� ����
	*/

	//�����Ÿ��� �����Ѵٸ�
	if (mCollision.count("MAX_RANGE"))
	{
		if (targTeam == GRIFFON_TACDOLL)
		{
			//GFL�д� �Ǻ�
			int e_SquadID = PLAYER->getCurrentSquad();
			for (int i = 1; i < PLAYER->getPlayerSquad(e_SquadID)->mSquad.size() + 1; ++i)
			{
				auto enemy = PLAYER->getIOPdoll_crntSquad(i);

				//�׾������� �Ѱܹ���
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

				//�̹� ��Ÿ����� �ִ� ���ΰ�?
				if (FindEnemy_ID(i))
				{
					if (!mCollision.find("MAX_RANGE")->second->EllipseCollision_Check(enemy->getCollision("SELF")))
						vRange.erase(vRange.begin() + getEnemy_ID_Pos(i));
					continue;
				}

				//������ SELF�� �ִ� ��Ÿ��� ���ϰ��ִٸ�
				if (mCollision.find("MAX_RANGE")->second->EllipseCollision_Check(enemy->getCollision("SELF")))
					vRange.push_back(i);
			}

			//�Ʊ���, ��ŷ���� �Ʊ� Ȯ��
			e_SquadID = BDATA->getEngageSquadID();
			for (int i = 1; i < BDATA->getEnemySquad(e_SquadID)->mSquad.size() + 1; ++i)
			{
				if (i == myID.SquadMem_ID)continue;
				auto enemy = BDATA->getEnemyMember(e_SquadID, i);

				if (!BDATA->getCur_EnemyMember(i)->getAlive()) {

					if (Target_ID > 99 && Target_ID - 100 == enemy->getID()->SquadMem_ID) {
						vRange.erase(vRange.begin() + getEnemy_ID_Pos(Target_ID));		//��ŷ�� Ǯ�ȴ��� ���θ� Ȯ���Ѵ�
						Target_ID = -1;
					}
					continue;
				}

				if (enemy->getTargetOS() != SANGVIS_TACDOLL) {
					if (getEnemy_ID_Pos(Target_ID) != -1 && Target_ID > 99 && Target_ID - 100 == enemy->getID()->SquadMem_ID)
					{
						vRange.erase(vRange.begin() + getEnemy_ID_Pos(Target_ID));		//��ŷ�� Ǯ�ȴ��� ���θ� Ȯ���Ѵ�
						Target_ID = -1;
						continue;
					}
				}

				else
				{
					//�׾������� �Ѱܹ���
					if (!enemy->getAlive())
					{
						if ((Target_ID) == enemy->getID()->SquadMem_ID)
						{
							if (getEnemy_ID_Pos(Target_ID) != -1)
							{
								vRange.erase(vRange.begin() + getEnemy_ID_Pos(Target_ID));		//10X�� ��ŷ���� �Ʊ��� �����Ѵ�.
								Target_ID = -1;
							}
						}
						continue;
					}

					//�̹� ��Ÿ����� �ִ� ���ΰ�?
					if (FindEnemy_ID(i + 100))
					{
						if (!mCollision.find("MAX_RANGE")->second->EllipseCollision_Check(enemy->getCollision("SELF")))
							vRange.erase(vRange.begin() + getEnemy_ID_Pos(i + 100));
						continue;
					}

					//������ SELF�� �ִ� ��Ÿ��� ���ϰ��ִٸ�
					if (mCollision.find("MAX_RANGE")->second->EllipseCollision_Check(enemy->getCollision("SELF")))
						vRange.push_back(i + 100);
				}
			}
		}

		//������ ������ �޾Ƽ� ��������� �շ��Ǿ��� ���
		else
		{
			int e_SquadID = BDATA->getEngageSquadID();
			for (int i = 1; i < BDATA->getEnemySquad(e_SquadID)->mSquad.size() + 1; ++i)
			{
				//�ڱ��ڽ��� ���ð�� �н�
				if (i == myID.SquadMem_ID)continue;

				auto enemy = BDATA->getEnemyMember(e_SquadID, i);

				//�׾������� �Ѱܹ���
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

				//�̹� ��Ÿ����� �ִ� ���ΰ�?
				if (FindEnemy_ID(i))
				{
					if (!mCollision.find("MAX_RANGE")->second->EllipseCollision_Check(enemy->getCollision("SELF")))
						vRange.erase(vRange.begin() + getEnemy_ID_Pos(i));
					continue;
				}

				//������ SELF�� �ִ� ��Ÿ��� ���ϰ��ִٸ�
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
			//�ùٸ� Ÿ�������� üŷ�Ѵ�.
			if (!FindEnemy_ID(Target_ID))
				Target_ID = -1;
		}
	}
}

void SNVtacDoll::Set_Targetting_Angle()
{
	//Target_ID�� �д� ����� ���°�����̴�.
	//Ÿ���� �����ϰ�, �̵����� �ƴ϶��
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


		//�ﰢ�Լ� ���
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
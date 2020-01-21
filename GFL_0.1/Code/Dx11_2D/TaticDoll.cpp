#include "stdafx.h"
#include "TaticDoll.h"

ID3D11Buffer* TaticDoll::_colorBuffer = nullptr;

TaticDoll::TaticDoll()
	: motion(nullptr)
{
	myID.All_ID = myID.SquadMem_ID = myID.Squad_ID = -1;
	alianceType = ALIANCE_NONE;

	if (_colorBuffer == nullptr)
		CreateConstantBuffer(&_colorBuffer, sizeof(D3DXCOLOR), &_color);

}
TaticDoll::~TaticDoll(){}
void TaticDoll::LoadTray_List() {}
void TaticDoll::render() {}
void TaticDoll::render_VisualBar() {}
void TaticDoll::render_Motion() 
{
	DeviceContext->UpdateSubresource(_colorBuffer, 0, NULL, _color, 0, 0);
	DeviceContext->PSSetConstantBuffers(2, 1, &_colorBuffer);
}
void TaticDoll::Use_ActiveSkill() {}
void TaticDoll::MotionUpdate() {}
void TaticDoll::Update_DrawPos() {}
void TaticDoll::Unit_CollitionCheck() {}

void TaticDoll::update_Coltime()
{
	if (atkColTime > 0.0)
		atkColTime -= DELTA;
	else
		atkColTime = 0.0;


	if (sklColTime > 0.0)
		sklColTime -= DELTA;
	else
		sklColTime = 0.0;
}

HRESULT TaticDoll::init()
{
	Flip = false;
	isAlive = true;

	sklColTime = 0.0f;
	atkColTime = 0.0f;

	_color = D3DXCOLOR(1, 1, 1, 1);

	return S_OK;
}

void TaticDoll::release()
{
	SAFE_DELETE(motion);
}

void TaticDoll::update()
{
	_color.r = _color.r < 1.0F ? _color.r + (DELTA * 5.0f) : 1.0F;
	_color.g = _color.g < 1.0F ? _color.g + (DELTA * 5.0f) : 1.0F;
	_color.b = _color.b < 1.0F ? _color.b + (DELTA * 5.0f) : 1.0F;

	MoveClickPoint();
	Limit_CharacterPosition();

	MoveClickPoint();
	IsEnemy_at();
	Set_Targetting_Angle();
}

void TaticDoll::render_Ellipse()
{

}

void TaticDoll::Limit_CharacterPosition()
{
	/*if (Pos.y < CHARACTERPOS_LIM_Y)
	{
		Pos.y = CHARACTERPOS_LIM_Y;
	}*/
}

void TaticDoll::StopMoving()
{
	if (motion->isCurrent("move"))
	{
		//sound->Stop_Sound("WALK");
		motion->changeMotion("wait", true, true, 0.125f);

		if (vPath.size() > 0)
			vPath.clear();
	}
}

void TaticDoll::SetMovePoint()
{
	//���콺�� �������� ȣ��Ǿ�, ���콺 �����ͷ� ��ġ�� �������ش�.
	//SHIFT�� Ȱ��ȭ �����̸�, ������ ��ġ�� �������ش�.
	if (isAlive)
	{
		if (KEYMANAGER->isKeyStayDown(VK_LSHIFT))
		{
			D3DXVECTOR2 temp;
			temp.x = (float)g_ptMouse.x + CameraPositionX;
			temp.y = (float)g_ptMouse.y - CameraPositionY;

			//if (temp.y < CHARACTERPOS_LIM_Y)
			//	temp.y = CHARACTERPOS_LIM_Y;

			vPath.push_back(temp);

			if (!motion->isCurrent("move"))
			{
				msPointX = vPath.begin()->x;
				msPointY = vPath.begin()->y;
				Angle = getAngle(Pos.x, Pos.y, msPointX, msPointY);

				if (moveAble)
					motion->changeMotion("move", true, true, 0.125f);
			}

		}

		else
		{
			vPath.clear();

			msPointX = (double)g_ptMouse.x + CameraPositionX;
			msPointY = (double)g_ptMouse.y - CameraPositionY;

			double t_dTotalPos = abs(Pos.x - msPointX) + abs(Pos.y - msPointY);

			if (t_dTotalPos > 1)
			{

				//if (msPointY < CHARACTERPOS_LIM_Y)
				//	msPointY = CHARACTERPOS_LIM_Y;


				Angle = getAngle(Pos.x, Pos.y, msPointX, msPointY);

				if (!motion->isCurrent("move"))
					if (moveAble)
						motion->changeMotion("move", true, true, 0.125f);
			}
		}

	}
}

void TaticDoll::MoveClickPoint()
{
	if (motion->isCurrent("move"))
	{
		float Spd = 0.0f;
		float s_angle = sinf(Angle);
		float c_angle = cosf(Angle);

		if (s_angle >= 0)
			Spd = moveSpd - (s_angle * 0.5f) * moveSpd;

		else
			Spd = moveSpd + (s_angle * 0.5f) * moveSpd;

		Spd *= DELTA;

		//�ﰢ�Լ� ���
		Pos.x = Pos.x + (Spd * c_angle);
		Pos.y = Pos.y - (Spd * s_angle);

		//sound->Play_Sound("WALK");


		if (Pos.x >= msPointX - (Spd * 0.5f) && Pos.x <= msPointX + (Spd * 0.5f))
			if (Pos.y >= msPointY - (Spd * 0.5f) && Pos.y <= msPointY + (Spd * 0.5f))
			{
				if (vPath.size() <= 0)
				{
					//sound->Stop_Sound("WALK");
					motion->changeMotion("wait", true, true, 0.125f);
				}

				else
				{
					msPointX = vPath.begin()->x;
					msPointY = vPath.begin()->y;
					Angle = getAngle(Pos.x, Pos.y, msPointX, msPointY);
					vPath.erase(vPath.begin());
				}
			}

	}
}

bool TaticDoll::MovePoint(float * nx, float * ny, float x, float y)
{
	return false;
}

void TaticDoll::MovePoint(float x, float y)
{
	msPointX = x;
	msPointY = y;

	Angle = getAngle(Pos.x, Pos.y, msPointX, msPointY);

	if (moveAble)
		if (!motion->isCurrent("move"))
			motion->changeMotion("move", true, true, 0.125f);
}

void TaticDoll::IsEnemy_at()
{
	/*
		���� ���ݹ����� ���� ����
	*/

	iter_mCollision iter;

	size_t EnemyLoopSize;		//	��ȸ�ؾ��� ������
	
	switch (alianceType)
	{
	case ALIANCE_NONE:
		break;
	case ALIANCE_GRIFFON:
		EnemyLoopSize = BDATA->getCurrUnits().size();
		break;

	case ALIANCE_IRONBLOD:
	case ALIANCE_SCCOM:
	case ALIANCE_PEREDEUS:
		EnemyLoopSize = PLAYER->getPlayerSquad(PLAYER->getCurrentSquad())->mSquad.size();	
		break;
	}

	//	�����Ÿ��� �����Ѵٸ�,
	if ((iter = mCollision.find("MAX_RANGE")) != mCollision.end())
	{
		for (size_t i = 0; i < EnemyLoopSize; ++i)
		{
			TaticDoll* object;
			iter = mCollision.find("MAX_RANGE");

			switch (alianceType)
			{
			case ALIANCE_NONE:
				break;
			case ALIANCE_GRIFFON:
				object = BDATA->getObject(i);
				break;

			case ALIANCE_IRONBLOD:
			case ALIANCE_SCCOM:
			case ALIANCE_PEREDEUS:
				object = PLAYER->getIOPdoll_crntSquad(i);
				break;
			}
			
			//	������������� �н�
			if (!object->getAlive())
			{
				if (TargetID == i)
					vRange.erase(vRange.begin() + getEnemy_ID_Pos(i));
				TargetID = -1;
				continue;
			}


			if (iter == mCollision.end())
				int a = 10;

			//	�̹� ��Ÿ� ���� �ִٸ�
			else if (FindEnemy_ID(i))
			{
				//	��Ÿ��� ����°�?
				if (!iter->second->EllipseCollision_Check(object->getCollision("SELF")))
				{
					vRange.erase(vRange.begin() + getEnemy_ID_Pos(i));
					if (TargetID == i)
						TargetID = -1;
				}
				continue;
			}

			//	������ SELF�� �ִ� ��Ÿ��� ���ϰ��ִٸ� , ����ִٸ�
			if (iter == mCollision.end())
				int a = 10;

			else if (iter->second->EllipseCollision_Check(object->getCollision("SELF")))
			{
				if ((iter = mCollision.find("MIN_RANGE")) != mCollision.end())
				{
					if (!iter->second->PointCollision_Check(object->getCharacterPos().x, object->getCharacterPos().y))
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

bool TaticDoll::FindEnemy_ID(SINT id)
{
	for (auto& it : vRange)
		if (it == id)
			return true;
	return false;
}

int TaticDoll::getEnemy_ID_Pos(SINT id)
{
	for (size_t i = 0; i < vRange.size(); ++i)
	{
		if (vRange[i] == id)
			return static_cast<int>(i);
	}
	return -1;
}

void TaticDoll::Set_Targetting()
{
	if (vRange.size() < 1)
		TargetID = -1;
	else
	{
		if (!FindEnemy_ID(TargetID))
			TargetID = -1;

		if (TargetID == -1)
			TargetID = vRange.at(0);
	}
}

void TaticDoll::Set_MinTargetting()
{
	iter_mCollision iter;

	//�ּ� ��Ÿ��� �����Ѵٸ�
	if ((iter = mCollision.find("MIN_RANGE")) != mCollision.end())
	{
		//��Ÿ� ���� ������ �Ǵ��Ѵ�.
		for (auto& it : vRange)
		{
			TaticDoll* object;

			switch (alianceType)
			{
			case ALIANCE_NONE:
				break;
			case ALIANCE_GRIFFON:
				object = BDATA->getObject(it);
				break;

			case ALIANCE_IRONBLOD:
			case ALIANCE_SCCOM:
			case ALIANCE_PEREDEUS:
				object = PLAYER->getIOPdoll_crntSquad(it);
				break;
			}

			if (iter->second->PointCollision_Check(object->getCharacterPos().x, object->getCharacterPos().y))
			{
				if (getEnemy_ID_Pos(it) != -1)
				{
					vRange.erase(vRange.begin() + getEnemy_ID_Pos(it));

					if (TargetID == it)
						TargetID = -1;
				}
			}
		}
	}
}

void TaticDoll::Set_Targetting_Angle()
{
	//	Target_ID�� �д� ����� ���°�����̴�.
	//	Ÿ���� �����ϰ�, �̵����� �ƴ϶��

	if (TargetID != -1 && !motion->isCurrent("move"))
	{
		TaticDoll* object;

		switch (alianceType)
		{
		case ALIANCE_NONE:
			break;
		case ALIANCE_GRIFFON:
			object = BDATA->getObject(TargetID);
			break;

		case ALIANCE_IRONBLOD:
		case ALIANCE_SCCOM:
		case ALIANCE_PEREDEUS:
			object = PLAYER->getIOPdoll_crntSquad(TargetID);
			break;
		}

		if (object != nullptr)
			Angle = getAngle(Pos.x, Pos.y, object->getCharacterPos().x, object->getCharacterPos().y);
	}
}

void TaticDoll::Set_Targetting_Other(SINT id)
{
	if (FindEnemy_ID(id))
		TargetID = id;
}

void TaticDoll::Character_GetDamage(const Status & st)
{
	/*
		����/ȸ�� ����

		���� = ���� / (ȸ�� + ����)
	*/

	//����� ����ġ [ 0.0 ~ 1.0 / ���� ���� ���߷��� 1.0�� �ٻ��Ѵ�.]
	double Accur = st.Accuracy / (curState.Avoid + st.Accuracy);
	double Acr_rnd = static_cast<double>(rand() % 101) / 100.0;

	//����
	if (Accur >= Acr_rnd)
	{
		//ġ��Ÿ
		if (rand() % 100 < st.CriticPoint)
		{
			float per = st.CriticAcl;
			per = per * 0.01f;

			float CriticDamage = static_cast<float>((float)st.AttackPoint * per);

			UINT totalDamge = st.AttackPoint + (UINT)CriticDamage;

			//HP�� ���δ�.
			if (curState.HitPoint.curr > totalDamge)
			{
				curState.HitPoint.curr -= totalDamge;
				_color.g = _color.b = 0.0F;
			}

			else
				curState.HitPoint.curr = 0;

			//DAMAGELOG->Create_Damage(cPos.x, dPos.y, totalDamge, true);
		}

		else
		{
			//HP�� ���δ�.
			if (curState.HitPoint.curr > st.AttackPoint)
			{
				curState.HitPoint.curr -= st.AttackPoint;
				_color.g = _color.b = 0.0F;
			}

			else
				curState.HitPoint.curr = 0;

			//DAMAGELOG->Create_Damage(cPos.x, dPos.y, st.AttackPoint);
		}


		//HP�� �� �������
		if (curState.HitPoint.curr <= 0)
			isAlive = false;	//�� �׾���
	}
	//else
		//DAMAGELOG->Create_Damage(cPos.x, dPos.y, -1, 0);
}

void TaticDoll::Render_VisualBar(D3DXVECTOR2 _pos, int _curHp, int _maxHp, D3DXVECTOR2 _size, ColorF _frontColor, ColorF _backColor)
{
	D3DXVECTOR2 pos = _pos;

	pos.x -= CameraPositionX;
	pos.y += CameraPositionY;

	D2DX->renderRect(pos.x, pos.y, _size.x, _size.y, _frontColor);
	D2DX->renderRect(pos.x, pos.y, (static_cast<FLOAT>(_curHp)) / (static_cast<FLOAT>(_maxHp)) * _size.x, _size.y, _frontColor, true);
	
}

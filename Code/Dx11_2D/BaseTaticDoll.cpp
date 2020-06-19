#include "stdafx.h"
#include "BaseTaticDoll.h"

template<typename T>
bool findAt(vector<T> _vec, T _value)
{
	for (auto& it : _vec)
		if (_value == it)
			return true;
	return false;
}

BaseTaticDoll::BaseTaticDoll()
	: motion(nullptr), _colorBuffer(nullptr), statusManager(nullptr)
{
	myID.All_ID = myID.SquadMem_ID = myID.Squad_ID = -1;
	alianceType = ALIANCE_NONE;

	if (_colorBuffer == nullptr)
		CreateConstantBuffer(&_colorBuffer, sizeof(D3DXCOLOR), &_color);
}
BaseTaticDoll::~BaseTaticDoll() {}
void BaseTaticDoll::LoadTray_SoundList(){}
void BaseTaticDoll::LoadTray_ImageList(){}
void BaseTaticDoll::render() {}
void BaseTaticDoll::reset() {}
void BaseTaticDoll::render_VisualBar() {}
void BaseTaticDoll::render_Motion()
{
	DeviceContext->UpdateSubresource(_colorBuffer, 0, NULL, _color, 0, 0);
	DeviceContext->PSSetConstantBuffers(2, 1, &_colorBuffer);
}
void BaseTaticDoll::Use_ActiveSkill() {}
void BaseTaticDoll::MotionUpdate() {}
void BaseTaticDoll::Update_DrawPos() {}
void BaseTaticDoll::Unit_CollitionCheck() {}

void BaseTaticDoll::AttachEquipment()
{
	curState = maxState;

	for (auto& it : mEquip)
	{
		if (it.second == nullptr) continue;

		auto& atchState = it.second->getState();

		curState.Accuracy += atchState.Accuracy;
		curState.AttackDelay -= (curState.AttackDelay * atchState.AttackDelay) * 0.01f;
		curState.AttackPoint += atchState.AttackPoint;
		curState.CriticPoint += atchState.CriticPoint;
	}

	//옵티컬일때 최소사거리를 주고, 사거리를 증가시킴
	if (mEquip.count(EPT_ACESORY))
	{
		if (mEquip.find(EPT_ACESORY)->second != nullptr)
		{
			if (mEquip.find(EPT_ACESORY)->second->getItemType() == EPC_OPTICAL)
			{
				auto& maxRange = mCollision.find("MAX_RANGE")->second;

				float l, s;
				l = axisMax_LongRad + (maxRange->getLongRad() * 0.2f);
				s = axisMax_ShortRad + (maxRange->getShortRad() * 0.2f);

				if (!mCollision.count("MIN_RANGE"))
					mCollision.insert(make_pair("MIN_RANGE",
						new EllipseBase(&Pos.x, &Pos.y, axisMin_LongRad, axisMin_ShortRad)));

				maxRange->ChangeLadius(l, s);
			}

			else
			{
				auto& maxRange = mCollision.find("MAX_RANGE")->second;
				maxRange->ChangeLadius(axisMax_LongRad, axisMax_ShortRad);

				//최소사거리가 존재한다면 -> 최소사거리를 없앰
				if (mCollision.count("MIN_RANGE"))
				{
					auto& it = (mCollision.find("MIN_RANGE")->second);
					it->Release_Ellipse();
					SAFE_DELETE(it);

					mCollision.erase(mCollision.find("MIN_RANGE"));
				}
			}

		}
	}

	//연장총열일때 사거리를 증가시킴
	if (mEquip.count(EPT_ACESORY2))
	{
		if (mEquip.find(EPT_ACESORY2)->second != nullptr)
		{
			if (mEquip.find(EPT_ACESORY2)->second->getItemType() == EPC_EXTBAR)
			{
				auto& maxRange = mCollision.find("MAX_RANGE")->second;

				float l, s;
				l = maxRange->getLongRad() + (axisMax_LongRad * 0.1f);
				s = maxRange->getShortRad() + (axisMax_ShortRad * 0.1f);

				maxRange->ChangeLadius(l, s);
			}

			else
			{
				bool sameCheck = false;
				if (mEquip.count(EPT_ACESORY))
				{
					if (mEquip.find(EPT_ACESORY)->second != nullptr)
					{
						if (mEquip.find(EPT_ACESORY)->second->getItemType() == EPC_OPTICAL)
							sameCheck = true;
					}
				}

				if (!sameCheck)
				{
					auto& maxRange = mCollision.find("MAX_RANGE")->second;
					maxRange->ChangeLadius(axisMax_LongRad, axisMax_ShortRad);
				}
			}
		}
	}
}

void BaseTaticDoll::update_Coltime()
{
	if (atkColTime > 0.0)
		atkColTime -= DELTA() * DeltaAcl;
	else
		atkColTime = 0.0;


	if (sklColTime > 0.0)
		sklColTime -= DELTA() * DeltaAcl;
	else
		sklColTime = 0.0;
}

HRESULT BaseTaticDoll::init()
{
	Flip = false;
	isAlive = true;

	sklColTime = 0.0f;
	atkColTime = 0.0f;

	_color = D3DXCOLOR(1, 1, 1, 1);

	return S_OK;
}

void BaseTaticDoll::release()
{
	SAFE_RELEASE(_colorBuffer);

	for (auto& iter : mCollision)
	{
		if (iter.second != nullptr)
		{
			iter.second->Release_Ellipse();
			SAFE_DELETE(iter.second);
		}
	}

	SAFE_DELETE(motion);
	SAFE_DELETE(statusManager);
}

void BaseTaticDoll::update()
{
	_color.r = _color.r < 1.0F ? _color.r + (DELTA() * DeltaAcl * 5.0f) : 1.0F;
	_color.g = _color.g < 1.0F ? _color.g + (DELTA() * DeltaAcl * 5.0f) : 1.0F;
	_color.b = _color.b < 1.0F ? _color.b + (DELTA() * DeltaAcl * 5.0f) : 1.0F;

	if (isAlive)
	{
		MoveClickPoint();
		Limit_CharacterPosition();

		IsEnemy_at();
		Set_Targetting_Angle();
	}
}

void BaseTaticDoll::render_Ellipse()
{

}

void BaseTaticDoll::Limit_CharacterPosition()
{
	/*if (Pos.y < CHARACTERPOS_LIM_Y)
	{
		Pos.y = CHARACTERPOS_LIM_Y;
	}*/
}

void BaseTaticDoll::StopMoving()
{
	if (motion->isCurrent("move"))
	{
		//sound->Stop_Sound("WALK");
		motion->changeMotion("wait", true, true, 0.125f);

		if (vPath.size() > 0)
			vPath.clear();
	}
}

void BaseTaticDoll::SetMovePoint()
{
	//마우스가 눌렸을때 호출되어, 마우스 포인터로 위치를 지정해준다.
	//SHIFT가 활성화 상태이면, 지정된 위치를 저장해준다.

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

			msPointX = static_cast<float>(g_ptMouse.x) + CameraPositionX;
			msPointY = static_cast<float>(g_ptMouse.y) - CameraPositionY;

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

void BaseTaticDoll::MoveClickPoint()
{
	if (motion->isCurrent("move"))
	{
		float Spd = 0.0f;
		float s_angle = sinf(Angle);
		float c_angle = cosf(Angle);

		if (!inWorld) {
			if (s_angle >= 0)
				Spd = moveSpd - (s_angle * 0.5f) * moveSpd;

			else
				Spd = moveSpd + (s_angle * 0.5f) * moveSpd;

			Spd *= (DELTA() * DeltaAcl);
		}
		else
			Spd = DELTA() * 150.0f;

		//삼각함수 계산
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

bool BaseTaticDoll::MovePoint(float * nx, float * ny, float x, float y)
{
	x, y, nx, ny;
	return false;
}

void BaseTaticDoll::MovePoint(float x, float y, bool world)
{
	msPointX = x;
	msPointY = y;

	inWorld = world;

	Angle = getAngle(Pos.x, Pos.y, msPointX, msPointY);

	if (moveAble)
		if (!motion->isCurrent("move"))
			motion->changeMotion("move", true, true, 0.125f);
}

void BaseTaticDoll::IsEnemy_at()
{
	/*
		기존 공격범위에 의한 공격
	*/

	iter_mCollision iter;

	size_t EnemyLoopSize = 0;							//	순회해야할 사이즈
	vector<TATICDOLL_ALIANCE_TYPE> alianceList;		//	동맹 리스트

	switch (alianceType)
	{
	case ALIANCE_NONE:
		break;
	case ALIANCE_GRIFFON:
		EnemyLoopSize = BDATA->getCurrUnits().size();
		alianceList = PLAYER->getAlianceList();
		break;

	case ALIANCE_IRONBLOD:
	case ALIANCE_SCCOM:
	case ALIANCE_PEREDEUS:
		EnemyLoopSize = PLAYER->getPlayerSquad(PLAYER->getCurrentSquad())->squadMember.size();
		break;
	}

	//	사정거리가 존재한다면,
	if ((iter = mCollision.find("MAX_RANGE")) != mCollision.end())
	{
		for (size_t i = 0; i < EnemyLoopSize; ++i)
		{
			BaseTaticDoll* object = nullptr;
			BaseTaticDoll* myAdr = this;
			iter = mCollision.find("MAX_RANGE");

			switch (alianceType)
			{
			case ALIANCE_NONE:
				break;
			case ALIANCE_GRIFFON:
				object = BDATA->getObject(i);
				if (object->alianceType == ALIANCE_GRIFFON)
				{
					if (TargetID == static_cast<signed int>(i))
						TargetID = -1;

					if (FindEnemy_ID(i))
						vRange.erase(vRange.begin() + getEnemy_ID_Pos(i));

					continue;
				}
				break;

			case ALIANCE_IRONBLOD:
			case ALIANCE_SCCOM:
			case ALIANCE_PEREDEUS:
				object = PLAYER->getIOPdoll_crntSquad(i);
				break;
			}

			if (object == myAdr)
			{
				TargetID = -1;
				continue;
			}

			//	살아있지않으면 패스
			if (!object->getAlive())
			{
				if (TargetID == static_cast<signed int>(i))
				{
					vRange.erase(vRange.begin() + getEnemy_ID_Pos(i));
					TargetID = -1;
				}
				continue;
			}

			if (findAt<TATICDOLL_ALIANCE_TYPE>(alianceList, object->alianceType))
			{
				if (TargetID == static_cast<signed int>(i))
				{
					vRange.erase(vRange.begin() + getEnemy_ID_Pos(i));
					TargetID = -1;
				}
				continue;
			}

			if (iter == mCollision.end()) {}

			//	이미 사거리 내에 있다면
			else if (FindEnemy_ID(i))
			{
				//	사거리를 벗어났는가?
				if (!iter->second->EllipseCollision_Check(object->getCollision("SELF")))
				{
					vRange.erase(vRange.begin() + getEnemy_ID_Pos(i));
					if (TargetID == static_cast<signed int>(i))
						TargetID = -1;
				}

				continue;
			}

			//	적군의 SELF와 최대 사거리가 접하고있다면 , 살아있다면
			if (iter == mCollision.end()) {}
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

bool BaseTaticDoll::FindEnemy_ID(SINT id)
{
	for (auto& it : vRange)
		if (it == id)
			return true;
	return false;
}

int BaseTaticDoll::getEnemy_ID_Pos(SINT id)
{
	for (size_t i = 0; i < vRange.size(); ++i)
	{
		if (vRange[i] == id)
			return static_cast<int>(i);
	}
	return -1;
}

void BaseTaticDoll::Set_Targetting()
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

void BaseTaticDoll::Set_MinTargetting()
{
	iter_mCollision iter;

	//최소 사거리가 존재한다면
	if ((iter = mCollision.find("MIN_RANGE")) != mCollision.end())
	{
		//사거리 내의 적으로 판단한다.
		for (auto& it : vRange)
		{
			BaseTaticDoll* object = nullptr;

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

void BaseTaticDoll::Set_Targetting_Angle()
{
	//	Target_ID는 분대 멤버중 몇번째인지이다.
	//	타겟이 존재하고, 이동중이 아니라면

	if (TargetID != -1 && !motion->isCurrent("move"))
	{
		BaseTaticDoll* object = nullptr;

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

void BaseTaticDoll::Set_Targetting_Other(SINT id)
{
	if (FindEnemy_ID(id))
		TargetID = id;
}

void BaseTaticDoll::Character_GetDamage(const Status & st)
{
	/*
		명중/회피 공식

		명중 = 명중 / (회피 + 명중)
	*/

	//재계산된 명중치 [ 0.0 ~ 1.0 / 가장 높은 명중률은 1.0에 근사한다.]
	double Accur = st.Accuracy / (curState.Avoid + st.Accuracy);
	double Acr_rnd = static_cast<double>(rand() % 101) / 100.0;

	//명중
	if (Accur >= Acr_rnd)
	{
		//	치명타
		if (rand() % 100 < st.CriticPoint)
		{
			double per = st.CriticAcl;
			per = per * 0.01;

			double CriticDamage = static_cast<double>((double)st.AttackPoint * per);

			UINT totalDamge = st.AttackPoint + (UINT)CriticDamage;

			if (curState.ArmorPoint.curr < 1)
			{
				//HP를 줄인다.
				if (static_cast<UINT>(curState.HitPoint.curr) > totalDamge)
				{
					totalDamge = totalDamge > static_cast<UINT>(curState.Armor) ? totalDamge - curState.Armor : 0;
					curState.HitPoint.curr -= totalDamge;

					_color.g = _color.b = 0.0F;
				}

				else
					curState.HitPoint.curr = 0;

				DAMAGE->Create_Damage(Pos.x, Pos.y - 50.0f, totalDamge, true);
			}

			else
			{
				if (st.ArmorPierce < 1)
				{
					totalDamge = 1;
					--curState.ArmorPoint.curr;
				}

				else
				{
					totalDamge = st.ArmorPierce;
					curState.ArmorPoint.curr -= st.ArmorPierce;
				}

				DAMAGE->Create_Damage(Pos.x, Pos.y - 50.0f, totalDamge, true, true);

			}


		}

		//	안치명타
		else
		{
			UINT totalDamge = st.AttackPoint;

			if (curState.ArmorPoint.curr < 1)
			{
				//HP를 줄인다.
				if (curState.HitPoint.curr > st.AttackPoint)
				{
					totalDamge = totalDamge > static_cast<UINT>(curState.Armor) ? totalDamge - static_cast<UINT>(curState.Armor) : 0;
					curState.HitPoint.curr -= totalDamge;
					_color.g = _color.b = 0.0F;
				}

				else
					curState.HitPoint.curr = 0;

				DAMAGE->Create_Damage(Pos.x, Pos.y - 50.0f, totalDamge);
			}

			else
			{
				if (st.ArmorPierce < 1)
				{
					totalDamge = 1;
					--curState.ArmorPoint.curr;
				}

				else
				{
					totalDamge = st.ArmorPierce;
					curState.ArmorPoint.curr -= st.ArmorPierce;
				}

				DAMAGE->Create_Damage(Pos.x, Pos.y - 50.0f, totalDamge, true, false);
			}


		}


		//HP가 다 닳았으면
		if (curState.HitPoint.curr <= 0)
			isAlive = false;	//앙 죽었띠
	}
	else
		DAMAGE->Create_Damage(Pos.x, Pos.y - 50.0f, -1);
}

void BaseTaticDoll::Render_VisualBar(VEC2 _pos, int _curHp, int _maxHp, VEC2 _size, ColorF _frontColor, ColorF _backColor)
{
	D3DXVECTOR2 pos = _pos;

	pos.x -= CameraPositionX;
	pos.y += CameraPositionY;

	D2D->renderRect(pos.x, pos.y, _size.x, _size.y, _frontColor);
	D2D->renderRect(pos.x, pos.y, (static_cast<FLOAT>(_curHp)) / (static_cast<FLOAT>(_maxHp)) * _size.x, _size.y, _frontColor, true);

}

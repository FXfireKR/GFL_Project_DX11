#include "stdafx.h"
#include "TaticDollBase.h"

TaticDollBase::TaticDollBase() : motion(nullptr)
{
}

TaticDollBase::~TaticDollBase()
{
}

void TaticDollBase::LoadTray_List()
{
}

HRESULT TaticDollBase::init()
{
	Flip = false;
	isAlive = true;

	SklColTime = 0.0f;
	AtkColTime = 0.0f;

	_color = D3DXCOLOR(1, 1, 1, 1);

	return S_OK;
}

void TaticDollBase::release()
{
	if (motion != nullptr) 
	{
		motion->release();
		SAFE_DEL(motion);
	}

	if (hpBar != nullptr)
		SAFE_DEL(hpBar);


	//if (sound != nullptr)
	//{
	//	sound->release_Manager();
	//	SAFE_DELETE(sound);
	//}

	for (auto& it : mCollision)
	{
		it.second->Release_Ellipse();
		SAFE_DEL(it.second);
	}
}

void TaticDollBase::update()
{
	_color.r = _color.r < 1.0F ? _color.r + (DELTA*5.0f) : 1.0F;
	_color.g = _color.g < 1.0F ? _color.g + (DELTA*5.0f) : 1.0F;
	_color.b = _color.b < 1.0F ? _color.b + (DELTA*5.0f) : 1.0F;
}

void TaticDollBase::render(){}

void TaticDollBase::render_VisualBar(){}

void TaticDollBase::render_Motion(){}

void TaticDollBase::render_Ellipse()
{
	Limit_CharacterPosition();

	for (auto& it : mCollision)
		it.second->Rend_Ellipse();
}

void TaticDollBase::Update_DrawPos()
{
}

void TaticDollBase::Unit_CollitionCheck()
{
}

void TaticDollBase::Limit_CharacterPosition()
{
	if (cPos.y < CHARACTERPOS_LIM_Y)
	{
		cPos.y = CHARACTERPOS_LIM_Y;

		if (motion->isCurrent("WALK"))	
			motion->Change_Motion("IDLE");
	}
}

void TaticDollBase::Character_GetDamage(const Status & st)
{
	/*
		����/ȸ�� ����

		���� = ���� / (ȸ�� + ����)
	*/

	//����� ����ġ [ 0.0 ~ 1.0 / ���� ���� ���߷��� 1.0�� �ٻ��Ѵ�.]
	double Accur = (double)(st.HitPoint / (double)(state.AvoidPoint + st.HitPoint));
	double Acr_rnd = (double)((double)(rand() % 101) / (double)100.0);

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
			if (state.HeatPoint.current > totalDamge)
			{
				state.HeatPoint.current -= totalDamge;
				_color.g = _color.b = 0.0F;
			}

			else
				state.HeatPoint.current = 0;

			//DAMAGELOG->Create_DamageFont(cPos.x, dPos.y, totalDamge, true);
		}

		else
		{
			//HP�� ���δ�.
			if (state.HeatPoint.current > st.AttackPoint)
			{
				state.HeatPoint.current -= st.AttackPoint;
				_color.g = _color.b = 0.0F;
			}

			else
				state.HeatPoint.current = 0;

			//DAMAGELOG->Create_DamageFont(cPos.x, dPos.y, st.AttackPoint);
		}


		//HP�� �� �������
		if (state.HeatPoint.current <= 0)
			isAlive = false;	//�� �׾���
	}
}

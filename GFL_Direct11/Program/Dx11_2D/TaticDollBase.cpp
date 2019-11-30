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
		명중/회피 공식

		명중 = 명중 / (회피 + 명중)
	*/

	//재계산된 명중치 [ 0.0 ~ 1.0 / 가장 높은 명중률은 1.0에 근사한다.]
	double Accur = (double)(st.HitPoint / (double)(state.AvoidPoint + st.HitPoint));
	double Acr_rnd = (double)((double)(rand() % 101) / (double)100.0);

	//명중
	if (Accur >= Acr_rnd)
	{
		//치명타
		if (rand() % 100 < st.CriticPoint)
		{
			float per = st.CriticAcl;
			per = per * 0.01f;

			float CriticDamage = static_cast<float>((float)st.AttackPoint * per);

			UINT totalDamge = st.AttackPoint + (UINT)CriticDamage;

			//HP를 줄인다.
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
			//HP를 줄인다.
			if (state.HeatPoint.current > st.AttackPoint)
			{
				state.HeatPoint.current -= st.AttackPoint;
				_color.g = _color.b = 0.0F;
			}

			else
				state.HeatPoint.current = 0;

			//DAMAGELOG->Create_DamageFont(cPos.x, dPos.y, st.AttackPoint);
		}


		//HP가 다 닳았으면
		if (state.HeatPoint.current <= 0)
			isAlive = false;	//앙 죽었띠
	}
}

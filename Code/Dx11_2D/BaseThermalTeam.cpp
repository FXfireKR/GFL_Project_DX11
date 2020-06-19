#include "stdafx.h"
#include "BaseThermalTeam.h"

BaseThermalTeam::BaseThermalTeam()
	: motion(nullptr), _colorBuffer(nullptr), statusManager(nullptr)
{
	myID.All_ID = myID.SquadMem_ID = myID.Squad_ID = -1;
	alianceType = ALIANCE_NONE;

	if (_colorBuffer == nullptr)
		CreateConstantBuffer(&_colorBuffer, sizeof(D3DXCOLOR), &_color);
}
BaseThermalTeam::~BaseThermalTeam(){}
void BaseThermalTeam::LoadTray_SoundList(){}
void BaseThermalTeam::LoadTray_ImageList(){}

HRESULT BaseThermalTeam::init()
{
	Flip = false;
	isAlive = true;

	atkColTime = 0.0f;

	_color = D3DXCOLOR(1, 1, 1, 1);

	return S_OK;
}

void BaseThermalTeam::release()
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

void BaseThermalTeam::update()
{
	_color.r = _color.r < 1.0F ? _color.r + (DELTA() * DeltaAcl * 5.0f) : 1.0F;
	_color.g = _color.g < 1.0F ? _color.g + (DELTA() * DeltaAcl * 5.0f) : 1.0F;
	_color.b = _color.b < 1.0F ? _color.b + (DELTA() * DeltaAcl * 5.0f) : 1.0F;

	if (isAlive)
	{
		MoveClickPoint();
	}
}

void BaseThermalTeam::render(){}
void BaseThermalTeam::reset(){}

void BaseThermalTeam::render_Motion()
{
}

void BaseThermalTeam::render_Ellipse()
{
}

void BaseThermalTeam::MotionUpdate()
{
}

void BaseThermalTeam::Update_DrawPos()
{
}

void BaseThermalTeam::StopMoving()
{
	if (motion->isCurrent("move"))
	{
		//sound->Stop_Sound("WALK");
		motion->changeMotion("wait", true, true, 0.125f);
	}
}

void BaseThermalTeam::SetMovePoint()
{
	//	마우스가 눌렸을때 호출되어, 마우스 포인터로 위치를 지정해준다.
	if (isAlive) {

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

void BaseThermalTeam::MoveClickPoint()
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

		Spd *= (DELTA() * DeltaAcl);

		//삼각함수 계산
		Pos.x = Pos.x + (Spd * c_angle);
		Pos.y = Pos.y - (Spd * s_angle);

		//sound->Play_Sound("WALK");

		if (Pos.x >= msPointX - (Spd * 0.5f) && Pos.x <= msPointX + (Spd * 0.5f))
			if (Pos.y >= msPointY - (Spd * 0.5f) && Pos.y <= msPointY + (Spd * 0.5f))
				motion->changeMotion("wait", true, true, 0.125f);
	}
}

bool BaseThermalTeam::MovePoint(float * nx, float * ny, float x, float y)
{
	x, y, nx, ny;
	return false;
}

void BaseThermalTeam::MovePoint(float x, float y)
{
	msPointX = x;
	msPointY = y;

	Angle = getAngle(Pos.x, Pos.y, msPointX, msPointY);

	if (moveAble)
		if (!motion->isCurrent("move"))
			motion->changeMotion("move", true, true, 0.125f);
}

void BaseThermalTeam::update_Coltime()
{
	if (atkColTime > 0.0)
		atkColTime -= DELTA() * DeltaAcl;
	else
		atkColTime = 0.0;
}

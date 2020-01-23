#include "stdafx.h"
#include "Effect.h"

Effect::Effect()
{
	Init_Effect();
}

Effect::~Effect()
{
}

HRESULT Effect::Init_Effect()
{
	fps = 0;
	FrameX = FrameY = 0;
	maxFrameX = maxFrameY = 0;
	count = 0;

	EffectEnd = true;
	render = false;

	return S_OK;
}

void Effect::Release_Effect()
{
}

HRESULT Effect::Create_Effect(string k, int maxX, int maxY, float _fps, FLOAT acl, FLOAT opc)
{
	maxFrameX = maxX;
	maxFrameY = maxY;

	key = k;

	ACL = acl;
	OPC = opc;

	fps = _fps;

	EffectEnd = false;
	render = false;

	return S_OK;
}

HRESULT Effect::Get_Rendorder(FLOAT x, FLOAT y)
{
	posX = x;
	posY = y;

	render = true;

	return S_OK;
}

void Effect::Update_Effect()
{
	if (render)
	{
		if (FrameX >= maxFrameX)
		{
			EffectEnd = true;
			render = false;
		}

		if (!EffectEnd)
		{
			//counter계수가 fps와 같아졌을경우 frame 상승
			if (count < fps)
				count += DELTA;

			else
			{
				count = 0.0f;
				++FrameX;
			}
		}
	}
}
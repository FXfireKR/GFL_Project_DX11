#include "stdafx.h"
#include "Effect.h"

Effect::Effect()
{
	resetEffect();
}

Effect::~Effect()
{
}

HRESULT Effect::Create_Effect(string _key, D3DXVECTOR2 _pos, D3DXVECTOR2 _perSize, POINT _maxFrame, float _fps, float _scl, float _opc)
{
	//	Reset
	resetEffect();

	minTexCoord.x = minTexCoord.y = 0.0f;
	maxTexCoord = perSize = _perSize;

	maxFrameX = _maxFrame.x;
	maxFrameY = _maxFrame.y;

	Trans = _pos;

	key = _key;

	Scale.x = _scl;
	Scale.y = _scl;

	OPC = _opc;

	fps = _fps;

	EffectEnd = false;
	render = true;

	return S_OK;
}

void Effect::update()
{
	if (render)
		updateEffect();

}

void Effect::updateEffect()
{
	if (FrameY >= maxFrameY)
	{
		EffectEnd = true;
		render = false;
	}

	if (!EffectEnd)
	{
		//	When count's value was same at fps, framecount up

		if (count < fps)
			count += DELTA;

		else
		{
			count = 0.0f;

			if (FrameX + 1 < maxFrameX)
			{
				++FrameX;
				minTexCoord.x = minTexCoord.x > 1.0f ? 1.0f : minTexCoord.x + perSize.x;
				maxTexCoord.x = maxTexCoord.x > 1.0f ? 1.0f : maxTexCoord.x + perSize.x;
			}

			else
			{		
				if (FrameY + 1 < maxFrameY)
				{
					++FrameY;
					FrameX = 0;

					minTexCoord.x = 0.0f;
					maxTexCoord.x = perSize.x;

					minTexCoord.y = minTexCoord.y > 1.0f ? 1.0f : minTexCoord.y + perSize.y;
					maxTexCoord.y = maxTexCoord.y > 1.0f ? 1.0f : maxTexCoord.y + perSize.y;
				}

				else
				{
					EffectEnd = true;
					render = false;
				}

			}
		}
	}	
}

void Effect::resetEffect()
{
	fps = 0;
	FrameX = FrameY = 0;
	maxFrameX = maxFrameY = 0;
	count = 0;

	EffectEnd = true;
	render = false;
}
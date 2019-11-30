#include "stdafx.h"
#include "Motion.h"

Motion::Motion() : img(nullptr)
{
	this->Init_Motion();
}

Motion::~Motion()
{
}

HRESULT Motion::Init_Motion()
{
	if(img == nullptr)
		img = new FrameImage("NULL");
	
	MaxFrame.x = MaxFrame.y = 0;
	Frame.x = Frame.y = 0;

	FramePerSec = 0.0f;
	counter = 0.0f;
	countGrav = 1.0f;

	isOnce = isReverse = isOver = false;

	fncPtr = nullptr;
	trgObj = nullptr;

	return S_OK;
}

void Motion::release()
{
	if (img != nullptr){
		img->release();
		SAFE_DEL(img);
	}
}

void Motion::Reset_Motion()
{
	Frame.x = isReverse ? MaxFrame.x - 1 : 0;
	Frame.y = 0;

	counter = 0.0f;
	countGrav = 1.0f;

	isOver = false;
	isOnce = false;
}

HRESULT Motion::Create_Motion(string key, FLOAT _pWidth, FLOAT _pHeight, FLOAT fps, bool rev)
{
	img->setTextureKey(key);

	auto Texture = IMAGEMANAGER->getImageInfo(key);
	MaxFrame.x = Texture->Width / (int)_pWidth;
	MaxFrame.y = Texture->Height / (int)_pHeight;

	Frame.x = Frame.y = 0;

	perWidth = _pWidth;
	perHeight = _pHeight;

	perWidthDec = perWidth / Texture->Width;
	perHeightDec = perHeight / Texture->Height;

	FramePerSec = fps;
	counter = 0;

	isReverse = rev;
	isOver = false;
	isOnce = false;

	Reset_Motion();

	return S_OK;
}

HRESULT Motion::RegistFunc(void * fncptr, void * obj)
{
	fncPtr = (MotionAct)fncptr;
	trgObj = obj;	//주소값만 보유함 ㅋㅋㅋ

	return S_OK;
}

void Motion::Update_Motion()
{
	if (counter < FramePerSec)
	{
		counter += DELTA * countGrav;
		if (isOnce)
			isOnce = false;
	}

	else
	{
		if (!isReverse)
		{
			//끝난 뒤에 다음 프레임이 재생될때
			if (isOver && Frame.x == 0)
				isOver = false;

			++Frame.x;

			if (Frame.x >= MaxFrame.x)
			{
				Frame.x = 0;
				isOver = true;
				isOnce = true;
			}
		}

		else
		{
			//끝난 뒤에 다음 프레임이 재생될때
			if (isOver && Frame.x == MaxFrame.x - 1)
				isOver = false;

			if (Frame.x != 0)
				--Frame.x;

			if (Frame.x == 0)
			{
				Frame.x = MaxFrame.x - 1;
				isOver = true;
				isOnce = true;
			}
			else


				counter = 0;	
		}

		counter = 0.0f;
		isOnce = true;
	}

	if (fncPtr != nullptr)
		fncPtr(trgObj);	//함수포인터 콜
}

void Motion::Pause_Motion()
{
	counter = 0.0f;
}

void Motion::Render_Motion(FLOAT x, FLOAT y)
{
	if (img != nullptr) {
		img->setTrans(D3DXVECTOR2(x, y));

		float curX = perWidthDec * Frame.x;
		float curY = perHeightDec * Frame.y;
		float endX = perWidthDec * (Frame.x + 1);
		float endY = perHeightDec * (Frame.y + 1);

		if (endX > 1.0f)
			endX = 1;

		if (endY > 1.0f)
			endY = 1;

		img->render(curX, endX, curY, endY);
	}
}

void Motion::Render_Motion(FLOAT x, FLOAT y, FLOAT sx, FLOAT sy)
{
	if (img != nullptr) {
		img->setTrans(D3DXVECTOR2(x, y));
		img->setScale(D3DXVECTOR2(sx, sy));
		img->render(0, 1, 0, 1);
	}
}

void Motion::Render_Motion(FLOAT x, FLOAT y, FLOAT sx, FLOAT sy, FLOAT alpha)
{
	if (img != nullptr) {
		img->setTrans(D3DXVECTOR2(x, y));
		img->setScale(D3DXVECTOR2(sx, sy));
		img->setAlpha(alpha);
		img->render(0, 1, 0, 1);
	}
}

void Motion::Render_Motion(D3DXVECTOR2 t, D3DXVECTOR2 s, FLOAT alpha, FLOAT dgr)
{
	if (img != nullptr) {
		img->setTrans(t);
		img->setScale(s);
		img->setAlpha(alpha);

		float curX = perWidthDec * Frame.x;
		float curY = perHeightDec * Frame.y;
		float endX = perWidthDec * (Frame.x + 1);
		float endY = perHeightDec * (Frame.y + 1);

		if (endX > 1.0f)
			endX = 1;

		if (endY > 1.0f)
			endY = 1;	

		// Change Dgree 
		if ((dgr < 90.0f) || (dgr > 270.0f && dgr < 360.0f))
			img->render(curX, endX, curY, endY);

		else
			img->render(endX, curX, curY, endY);
	}
}

void Motion::Render_Motion(D3DXVECTOR2 t, D3DXVECTOR2 s, D3DXCOLOR color, FLOAT dgr)
{
	if (img != nullptr) {
		img->setTrans(t);
		img->setScale(s);
		img->setColor(color);

		float curX = perWidthDec * Frame.x;
		float curY = perHeightDec * Frame.y;
		float endX = perWidthDec * (Frame.x + 1);
		float endY = perHeightDec * (Frame.y + 1);

		if (endX > 1.0f)
			endX = 1;

		if (endY > 1.0f)
			endY = 1;

		// Change Dgree 
		if ((dgr < 90.0f) || (dgr > 270.0f && dgr < 360.0f))
			img->render(curX, endX, curY, endY);

		else
			img->render(endX, curX, curY, endY);
	}
}

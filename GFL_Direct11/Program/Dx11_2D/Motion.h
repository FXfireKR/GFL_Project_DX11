#pragma once
#include "FrameImage.h"

typedef void(*MotionAct)(void* obj);

class Motion
{
private:
	FrameImage* img;

	FMPT MaxFrame;
	FMPT Frame;

	FLOAT FramePerSec;
	FLOAT counter;
	FLOAT countGrav;

	FLOAT perWidth;
	FLOAT perHeight;

	FLOAT perWidthDec;
	FLOAT perHeightDec;

	bool isReverse;
	bool isOver;
	bool isOnce;		//1Frame당 1번씩 활성화

	MotionAct fncPtr;
	void* trgObj;

public:
	Motion();
	~Motion();

	HRESULT Init_Motion();
	void release();
	void Reset_Motion();

	HRESULT Create_Motion(string key, FLOAT _pWidth, FLOAT _pHeight, FLOAT fps, bool rev = false);
	HRESULT RegistFunc(void* fncptr, void* obj);

	void Update_Motion();
	void Pause_Motion();

	void Render_Motion(__in FLOAT x, __in FLOAT y);
	void Render_Motion(__in FLOAT x, __in FLOAT y, __in FLOAT sx, __in FLOAT sy);
	void Render_Motion(__in FLOAT x, __in FLOAT y, __in FLOAT sx, __in FLOAT sy, __in FLOAT alpha);
	void Render_Motion(__in D3DXVECTOR2 t, __in D3DXVECTOR2 s, __in FLOAT alpha, __in FLOAT dgr);
	void Render_Motion(__in D3DXVECTOR2 t, __in D3DXVECTOR2 s, __in D3DXCOLOR color, __in FLOAT dgr);


public:
	inline FMPT getFrame() const { return Frame; }
	inline UINT getFrameX() const { return Frame.x; }
	inline UINT getFrameY() const { return Frame.y; }

	inline void setFrame(UINT x, UINT y) { Frame.x = x; Frame.y = y; }
	inline void setFrameX(UINT x) { Frame.x = x; }
	inline void setFrameY(UINT y) { Frame.y = y; }

	inline UINT getCounter() const { return counter; }

	inline FLOAT getFramePerSec() const { return FramePerSec; }
	inline void setFramePerSec(FLOAT fps) { FramePerSec = fps; }

	inline bool getIsOver() { return isOver; }
	inline bool getIsReverse() { return isReverse; }
	inline bool getIsOnce() { return isOnce; }

	inline void setCountGrav(FLOAT g) { countGrav = g; }

};
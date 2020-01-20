#pragma once
#include "singleton.h"

/*
		본 클래스는 Direct2D에서 지원하는 내용들을 사용하여, 다각형, 타원, 직선들을 랜더링합니다.

		

*/

class Direct2DManager : public singleton<Direct2DManager>
{
private:
	DXGI_SWAP_CHAIN_DESC swapDesc;

public:
	Direct2DManager();
	~Direct2DManager();

private:
	void Begin();
	void End();

public:

	void init();

	void renderLine(FLOAT x, FLOAT y, FLOAT x2, FLOAT y2);
	void renderLine(FLOAT x, FLOAT y, FLOAT x2, FLOAT y2, ColorF c);

	void renderRect(FLOAT x, FLOAT y, FLOAT width, FLOAT height);
	void renderRect(FLOAT x, FLOAT y, FLOAT width, FLOAT height, ColorF c, bool _fill = false);
	void renderRect(D2D_RECT_F rt, ColorF c, bool _fill = false);

	void renderRectCenter(FLOAT x, FLOAT y, FLOAT width, FLOAT height);
	void renderRectCenter(FLOAT x, FLOAT y, FLOAT width, FLOAT height, ColorF c);

	void renderEllipse(FLOAT x, FLOAT y, FLOAT longRad, FLOAT shortRad, ColorF c = ColorF(0, 0, 0));
};
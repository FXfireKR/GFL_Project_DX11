#include "stdafx.h"
#include "VisualBar.h"

VisualBar::VisualBar()
{
}

VisualBar::~VisualBar()
{
}

HRESULT VisualBar::Create_VisualBar(void * current, void * max, int wid, int hei)
{
	currentValue = current;
	maxValue = max;

	Width = wid;
	Height = hei;

	return S_OK;
}

void VisualBar::Render_VisualBar(FLOAT x, FLOAT y, ColorF c)
{
	x -= BDATA->getVirtualPos().x;
	y -= BDATA->getVirtualPos().y;

	DRAWMANAGER->renderRect(x, y, Width, Height, c);
	DRAWMANAGER->renderRect(x, y, *((FLOAT*)currentValue) / *((FLOAT*)maxValue) * Width, Height, c, true);

	//D2D1_RECT_F r;
	//r.left = x;
	//r.top = y;
	//r.right = Width + x;
	//r.bottom = Height + y;

	//ID2D1SolidColorBrush* b;
	//Rendtrg->CreateSolidColorBrush(c, &b);

	//Rendtrg->DrawRectangle(r, b);

	//D2D1_RECT_F r2;
	//r2.left = x;
	//r2.top = y;
	//r2.right = *((FLOAT*)currentValue) / *((FLOAT*)maxValue) * Width + x;
	//r2.bottom = Height + y;

	//ID2D1SolidColorBrush* b2;
	//Rendtrg->CreateSolidColorBrush(c, &b2);
	//Rendtrg->FillRectangle(r2, b2);

	//BackBar
	//DRAW->render_Rect(x - BDATA->getVirtualPos().x, y - BDATA->getVirtualPos().y, Width, Height);

	//FrontBar
	//DRAW->render_Rect(x - BDATA->getVirtualPos().x, y - BDATA->getVirtualPos().y, *((FLOAT*)currentValue) / *((FLOAT*)maxValue) * Width, Height);
}
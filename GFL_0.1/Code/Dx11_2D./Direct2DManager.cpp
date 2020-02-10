#include "stdafx.h"
#include "Direct2DManager.h"

Direct2DManager::Direct2DManager()
{
}

Direct2DManager::~Direct2DManager()
{
}

void Direct2DManager::Begin()
{	
	d2Rtg->BeginDraw();
}

void Direct2DManager::End()
{
	HRESULT hr = d2Rtg->EndDraw();
}

void Direct2DManager::init()
{
	HRESULT hr = SwapChain->GetDesc(&swapDesc);
}

void Direct2DManager::renderLine(FLOAT x, FLOAT y, FLOAT x2, FLOAT y2)
{
	D2D1_POINT_2F p1;
	p1.x = x;
	p1.y = y;

	D2D1_POINT_2F p2;
	p2.x = x2;
	p2.y = y2;

	ID2D1SolidColorBrush* b;
	d2Rtg->CreateSolidColorBrush(ColorF(0.0F, 0.0F, 0.0F), &b);

	ID2D1StrokeStyle* s;
	d2dFactory->CreateStrokeStyle(
		D2D1::StrokeStyleProperties(
			D2D1_CAP_STYLE_FLAT,
			D2D1_CAP_STYLE_FLAT,
			D2D1_CAP_STYLE_ROUND,
			D2D1_LINE_JOIN_MITER,
			10.0f,
			D2D1_DASH_STYLE_DOT)
		, nullptr, 0, &s);

	int a = s->GetDashesCount();

	d2Rtg->DrawLine(p1, p2, b, 4.0F, s);
}

void Direct2DManager::renderLine(FLOAT x, FLOAT y, FLOAT x2, FLOAT y2, ColorF c)
{
	D2D1_POINT_2F p1;
	p1.x = x;
	p1.y = y;

	D2D1_POINT_2F p2;
	p2.x = x2;
	p2.y = y2;

	ID2D1SolidColorBrush* b;
	d2Rtg->CreateSolidColorBrush(c, &b);
	b->SetOpacity(1.0F);

	Begin();
	{
		d2Rtg->DrawLine(p1, p2, b);
	}
	End();
}

void Direct2DManager::renderRect(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
	D2D1_RECT_F r;
	r.left = x;
	r.top = y;
	r.right = width + x;
	r.bottom = height + y;

	ID2D1SolidColorBrush* b;
	d2Rtg->CreateSolidColorBrush(ColorF(0.0F, 0.0F, 0.0F), &b);

	d2Rtg->DrawRectangle(r, b);
}

void Direct2DManager::renderRect(FLOAT x, FLOAT y, FLOAT width, FLOAT height, ColorF c, bool _fill)
{
	D2D1_RECT_F r;
	r.left = x;
	r.top = y;
	r.right = width + x;
	r.bottom = height + y;

	ID2D1SolidColorBrush* b;
	d2Rtg->CreateSolidColorBrush(c, &b);

	Begin();
	{
		if(_fill)
			d2Rtg->FillRectangle(&r, b);
		d2Rtg->DrawRectangle(r, b);
	}
	End();
}

void Direct2DManager::renderRect(D2D_RECT_F rt, ColorF c, bool _fill)
{
	ID2D1SolidColorBrush* b;
	d2Rtg->CreateSolidColorBrush(c, &b);

	Begin();
	{
		if (_fill)
			d2Rtg->FillRectangle(&rt, b);
		d2Rtg->DrawRectangle(rt, b);
	}
	End();
}

void Direct2DManager::renderRectCenter(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
	D2D1_RECT_F r;
	ID2D1SolidColorBrush* b;

	r.left = x - width;
	r.top = y - height;
	r.right = x + width;
	r.bottom = y + height;

	d2Rtg->CreateSolidColorBrush(ColorF(0.0F, 0.0F, 0.0F), &b);

	Begin();
	{
		d2Rtg->DrawRectangle(r, b);
	}
	End();
}

void Direct2DManager::renderRectCenter(FLOAT x, FLOAT y, FLOAT width, FLOAT height, ColorF c)
{
	D2D1_RECT_F r;
	ID2D1SolidColorBrush* b;

	r.left = x - width;
	r.top = y - height;
	r.right = x + width;
	r.bottom = y + height;
	
	d2Rtg->CreateSolidColorBrush(c, &b);

	Begin();
	{
		d2Rtg->DrawRectangle(r, b);
	}
	End();
}

void Direct2DManager::renderEllipse(FLOAT x, FLOAT y, FLOAT longRad, FLOAT shortRad, ColorF c)
{
	D2D1_ELLIPSE eb;
	eb.point.x = x;
	eb.point.y = y;
	eb.radiusX = longRad;
	eb.radiusY = shortRad;

	ID2D1SolidColorBrush* b;
	d2Rtg->CreateSolidColorBrush(c, &b);

	Begin();
	{
		d2Rtg->DrawEllipse(eb, b);
	}
	End();
}

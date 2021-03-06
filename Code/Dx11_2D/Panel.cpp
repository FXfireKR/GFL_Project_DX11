#include "stdafx.h"
#include "Panel.h"

int Panel::panelId = 0;

Panel::Panel(void)
{
}

Panel::~Panel(void)
{
}

HRESULT Panel::init()
{
	++panelId;
	return S_OK;
}

void Panel::release()
{
	LinkedId.clear();
}

HRESULT Panel::Create(float x, float y)
{
	this->init();

	panelEnum = PANEL_CLASS_NONE;
	alience = ALIANCE_NONE;

	pos.x = x;
	pos.y = y;

	change = false;

	rc = RectMakeCenter(pos.x, pos.y, PANEL_SIZE_X / 2, PANEL_SIZE_Y / 2);

	return S_OK;
}

HRESULT Panel::Create(D3DXVECTOR2 _position)
{
	this->init();

	panelEnum = PANEL_CLASS_NONE;
	alience = ALIANCE_NONE;

	pos = _position;

	change = false;

	rc = RectMakeCenter(pos.x, pos.y, PANEL_SIZE_X / 2, PANEL_SIZE_Y / 2);

	return S_OK;
}

void Panel::update()
{
	if (change)
	{
		change = false;
		alience = change_alie;
	}

}

void Panel::LineRender()
{

}

void Panel::setPlagData(BattlePlagData _plagData)
{
	plagData = _plagData;
}

BattlePlagData Panel::getPlagData()
{
	return plagData;
}

void Panel::render()
{
	//D2D->renderRect(rc.left - CameraPositionX, rc.top + CameraPositionY,
	//	(rc.right - CameraPositionX) - (rc.left - CameraPositionX),
	//	(rc.bottom + CameraPositionY) - (rc.top + CameraPositionY), ColorF(1, 0, 0));

	D3DXCOLOR _color = PANEL_COLOR_NONE;
	D3DXVECTOR3 panelPos = D3DXVECTOR3(rc.left + PANEL_SIZE_X_HALF, rc.top + PANEL_SIZE_Y_HALF, 0);

	switch (alience)
	{
	case ALIANCE_GRIFFON:
		_color = PANEL_COLOR_GRF;
		break;

	case ALIANCE_IRONBLOD:
		_color = PANEL_COLOR_SNV;
		break;

	case ALIANCE_PEREDEUS:
		_color = PANEL_COLOR_PRD;
		break;

	case ALIANCE_SCCOM:
		_color = PANEL_COLOR_SSO;
		break;
	}

	switch (panelEnum)
	{
	case PANEL_CLASS_NONE:
		DRAW->render("Panel_Normal", PANEL_NORMAL_SIZE, panelPos, _color);
		break;

	case PANEL_CLASS_HELIPORT:
		DRAW->render("Panel_Heli", PANEL_HELI_SIZE, panelPos, _color);
		break;

	case PANEL_CLASS_HQ:
		DRAW->render("Panel_Hq", PANEL_HQ_SIZE, panelPos, _color);
		break;
	}
}

bool Panel::FindLinkedPanel(int findnum)
{
	for (pLinkedId = LinkedId.begin(); pLinkedId != LinkedId.end(); ++pLinkedId)
	{
		if (*pLinkedId == findnum)
			return true;
	}

	return false;

}
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

HRESULT Panel::Create(int x, int y)
{
	this->init();

	panelEnum = NONE;
	alience = ALIANCE_NONE;

	pos.x = x;
	pos.y = y;

	change = false;

	rc = RectMakeCenter(pos.x, pos.y, PANELSIZE_X, PANELSIZE_Y);

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
	for (pLinkedId = LinkedId.begin(); pLinkedId != LinkedId.end(); ++pLinkedId)
	{
		//int x = MAP->pManager->findPanel(*pLinkedId)->getPanelPos().x;
		//int y = MAP->pManager->findPanel(*pLinkedId)->getPanelPos().y;

		//D2DX->renderLine(pos.x, pos.y, x, y, ColorF(1, 0, 0));
	}
}

void Panel::render()
{
	D2DX->renderRect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, ColorF(1, 0, 0));

	switch (alience)
	{
	case ALIANCE_NONE:

		switch (panelEnum)
		{
		case NONE:
			DRAW->render("N_Normal", DV2(50, 50), DV2(rc.left - 10, rc.top - 10));
			break;

		case HELIPORT:
			DRAW->render("N_Heliport", DV2(50, 50), DV2(rc.left - 18.75f, rc.top - 18.75f));
			break;

		case HQ:
			DRAW->render("N_HQ", DV2(50, 50), DV2(rc.left - 18.75f, rc.top - 18.75f));
			break;
		};

		break;

	case ALIANCE_GRIFFON:

		switch (panelEnum)
		{
		case NONE:
			DRAW->render("N_Normal", DV2(50, 50), DV2(rc.left - 10, rc.top - 10), DCR(0.1, 0.75, 0.8, 1));
			break;

		case HELIPORT:
			DRAW->render("N_Heliport", DV2(50, 50), DV2(rc.left - 18.75f, rc.top - 18.75f), DCR(0.1, 0.75, 0.8, 1));
			break;

		case HQ:
			DRAW->render("N_HQ", DV2(50, 50), DV2(rc.left - 18.75f, rc.top - 18.75f), DCR(0.1, 0.75, 0.8, 1));
			break;
		};

		break;

	case ALIANCE_IRONBLOD:

		switch (panelEnum)
		{
		case NONE:
			DRAW->render("N_Normal", DV2(50, 50), DV2(rc.left - 10, rc.top - 10), DCR(0.8, 0, 0, 1));
			break;

		case HELIPORT:
			DRAW->render("N_Heliport", DV2(50, 50), DV2(rc.left - 18.75f, rc.top - 18.75f), DCR(0.8, 0, 0, 1));
			break;

		case HQ:
			DRAW->render("N_HQ", DV2(50, 50), DV2(rc.left - 18.75f, rc.top - 18.75f), DCR(0.8, 0, 0, 1));
			break;
		};

		break;
	};

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
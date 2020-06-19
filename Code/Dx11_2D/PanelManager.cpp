#include "stdafx.h"
#include "PanelManager.h"

PanelManager::PanelManager(void)
{
}

PanelManager::~PanelManager(void)
{
}

HRESULT PanelManager::init()
{
	Select_Panel_id = -1;
	return S_OK;
}

int PanelManager::Create_Panel(int x, int y)
{
	Panel* p = new Panel;

	if (tempPanel != this->getTotalPanelNum())
	{
		int cnt = 1;

		for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
		{
			if (miPanel->first != cnt)
			{
				Panel::panelId = cnt - 1;
				break;
			}
			else
				++cnt;
		}
	}

	else
	{
		tempPanel = this->getTotalPanelNum();
		if (tempPanel != Panel::panelId)
			Panel::panelId = tempPanel;
	}

	p->Create(static_cast<float>(x), static_cast<float>(y));

	mPanel.insert(make_pair(p->getPanelId(), p));

	//cout << "Create Panel :: " << x << " , " << y << endl;
	//cout << "[Panel Id : " << p->getPanelId() << " ]\n\n" << endl;
	//cout << this->getTotalPanelNum() << "  " << tempPanel << endl;

	Select_Panel_id = p->getPanelId();

	return mPanel.size();
}

void PanelManager::Release_Panel()
{
	for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
	{
		(*miPanel).second->release();
		SAFE_DELETE(miPanel->second);
	}

	mPanel.clear();
}

void PanelManager::Update_Panel()
{
	//this->Search_SelPanel();

	for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
		(*miPanel).second->update();

}

void PanelManager::Render_Panel()
{
	Render_PanelLink();

	for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
		(*miPanel).second->render();
}

void PanelManager::Render_PanelLink()
{
	for (size_t i = 0; i < allLink.size(); ++i)
	{
		Panel* startPanel = mPanel[allLink[i].id1];
		Panel* endPanel = mPanel[allLink[i].id2];

		float dist = getDistance(startPanel->getPanelPos().x, startPanel->getPanelPos().y,
			endPanel->getPanelPos().x, endPanel->getPanelPos().y);

		int distTotalNum = static_cast<int>(dist / 40.0f) + 1;

		D3DXVECTOR2 position;
		position.x = startPanel->getPanelPos().x;
		position.y = startPanel->getPanelPos().y;

		float angle = 0.0f;

		for (distTotalNum; distTotalNum > 0; --distTotalNum)
		{
			angle = getAngle(position.x, position.y, endPanel->getPanelPos().x, endPanel->getPanelPos().y);

			DRAW->render("WAY", D3DXVECTOR2(35, 35), 
				position, D3DXCOLOR(1, 1, 1, 1), D3DXVECTOR3(0, 0, DGR(angle)));

			position.x += cosf(angle) * 40.0f;
			position.y -= sinf(angle) * 40.0f;
		}
	}
}

void PanelManager::Delete_Panel(int id)
{
	if (mPanel.find(id) != mPanel.end())
	{
		miPanel = mPanel.find(id);
		UnLink_Panel(id);
		miPanel->second->release();
		SAFE_DELETE(miPanel->second);
		mPanel.erase(miPanel);

		tempPanel = Panel::panelId;
	}
}


void PanelManager::UnLink_Panel(int id)
{
	for (int i = 0; i < miPanel->second->getLinkedPanelNum(); ++i)
	{
		int linkId = miPanel->second->getLinkedId(i);

		mPanel.find(linkId)->second->deleteLinkedId(id);
	}
}

void PanelManager::Search_SelPanel()
{
	//if (!POPENT->getisVisible())
	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
		{
			POINT vMous = g_ptMouse;
			vMous.x += static_cast<LONG>(CameraPositionX);
			vMous.y -= static_cast<LONG>(CameraPositionY);

			const RECT* ptTemp = miPanel->second->pcgetPanelRect();
			if (PtInRect(ptTemp, vMous))
			{
				Select_Panel_id = miPanel->first;
				//cout << "Select Panel ID : " << Select_Panel_id << endl;
				//cout << "Panel Type : " << miPanel->second->getPanelEnum() << endl;
				//cout << "[ Connet Pannel ]" << endl;

				for (int i = 0; i < 5; ++i)
				{
					if (miPanel->second->getLinkedId(i) != -1) {}
						//cout << "Linked P :" << miPanel->second->getLinkedId(i) << endl;
					else
						i = 5;
				}

				//cout << "\n\n" << endl;

				break;
			}
			else
				Select_Panel_id = -1;
		}
	}

}

void PanelManager::Search_SelPanel(int keyboard)
{
	//if (!POPENT->getisVisible())
	if (KEYMANAGER->isKeyDown(keyboard))
	{
		for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
		{
			POINT vMous = g_ptMouse;
			vMous.x += static_cast<LONG>(CameraPositionX);
			vMous.y -= static_cast<LONG>(CameraPositionY);

			const RECT* ptTemp = miPanel->second->pcgetPanelRect();
			if (PtInRect(ptTemp, vMous))
			{
				Select_Panel_id = miPanel->first;

				for (int i = 0; i < 5; ++i)
				{
					if (miPanel->second->getLinkedId(i) != -1) {}
					//cout << "Linked P :" << miPanel->second->getLinkedId(i) << endl;
					else
						i = 5;
				}

				//cout << "\n\n" << endl;

				break;
			}
			else
				Select_Panel_id = -1;
		}
	}
}

void PanelManager::Search_SelectPanel()
{
	for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
	{
		POINT vMous = g_ptMouse;
		vMous.x += static_cast<LONG>(CameraPositionX);
		vMous.y -= static_cast<LONG>(CameraPositionY);

		const RECT* ptTemp = miPanel->second->pcgetPanelRect();
		if (PtInRect(ptTemp, vMous))
		{
			Select_Panel_id = miPanel->first;

			for (int i = 0; i < 5; ++i)
			{
				if (miPanel->second->getLinkedId(i) != -1) {}
				//cout << "Linked P :" << miPanel->second->getLinkedId(i) << endl;
				else
					i = 5;
			}

			//cout << "\n\n" << endl;

			break;
		}
		else
			Select_Panel_id = -1;
	}
}

int PanelManager::Search_intfacePanel()
{
	for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
	{
		const RECT* ptTemp = miPanel->second->pcgetPanelRect();
		if (PtInRect(ptTemp, g_ptMouse))
		{
			//cout << miPanel->first << " is Selected" << endl;
			return miPanel->first;
		}
	}

	//cout << "Not Selected" << endl;
	return -1;
}

void PanelManager::Select_Panel_Classify(PANEL_CLASSIFY pc)
{
	mPanel.find(Select_Panel_id)->second->setPanelEnum(pc);
}

void PanelManager::Select_Panel_Alience(TATICDOLL_ALIANCE_TYPE ac)
{
	mPanel.find(Select_Panel_id)->second->setPanelAlience(ac);
}

bool PanelManager::Panel_Link(int id1, int id2)
{
	if (id1 != id2)
	{
		mPanel.find(id1)->second->setLinkedId(id2);
		mPanel.find(id2)->second->setLinkedId(id1);
		return true;
	}
	else
		return false;
}

void PanelManager::SavePanel_Data(char* filename)
{
}

void PanelManager::LoadPanel_Data(string path)
{
}

int PanelManager::Check_ActionPoint_For()
{
	int cnt = 0;

	for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
	{
		if (miPanel->second->getPanelAlience() == ALIANCE_GRIFFON)
		{
			if (miPanel->second->getPanelEnum() == PANEL_CLASS_HQ || miPanel->second->getPanelEnum() == PANEL_CLASS_HELIPORT)
				++cnt;
		}
	}

	return cnt;

}

void PanelManager::Check_AroundPanel(TATICDOLL_ALIANCE_TYPE ac)
{
	for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
	{
		bool al = false;

		for (int i = 0; i < miPanel->second->getLinkedPanelNum(); ++i)
		{
			int linkP = miPanel->second->getLinkedId(i);

			//1. 만약 주변 타일중에 미점령구역이있는지 판단
			if (mPanel.find(linkP)->second->getPanelAlience() == ALIANCE_NONE)
			{
				//cout << "Panel : " << miPanel->first << endl;
				//cout << "There is NONE Tile...\n" << endl;
				al = true;
				break;
			}

			//2. 미점령구역이 없다면 자신과 다른 구역의 점령세력 판단
			else if (mPanel.find(linkP)->second->getPanelAlience() == miPanel->second->getPanelAlience())
			{
				//cout << "Panel : " << miPanel->first << endl;
				//cout << "There is Other Alience of Panel..\n" << endl;
				al = true;
				break;
			}
		}

		int linkP = miPanel->second->getLinkedId(0);

		for (int i = 0; i < miPanel->second->getLinkedPanelNum(); ++i)
		{
			int olinkP = miPanel->second->getLinkedId(i);

			if (mPanel.find(olinkP)->second->getPanelAlience() != mPanel.find(linkP)->second->getPanelAlience())
			{
				al = true;
				break;
			}

		}

		if (!al)
		{
			if (ac == mPanel.find(linkP)->second->getPanelAlience())
			{
				miPanel->second->setPanelChanged(true);
				miPanel->second->setAroundAlience(mPanel.find(linkP)->second->getPanelAlience());
			}
		}

	}
}
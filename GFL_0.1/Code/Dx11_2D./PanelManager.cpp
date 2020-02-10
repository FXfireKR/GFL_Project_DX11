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

	p->Create(x, y);

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
		SAFE_DEL(miPanel->second);
	}

	mPanel.clear();
}

void PanelManager::Update_Panel()
{

	this->Search_SelPanel();

	for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
		(*miPanel).second->update();

}

void PanelManager::Render_Panel()
{

	for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
		(*miPanel).second->LineRender();

	for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
		(*miPanel).second->render();


}

void PanelManager::Delete_Panel(int id)
{
	if (mPanel.find(id) != mPanel.end())
	{
		miPanel = mPanel.find(id);
		UnLink_Panel(id);
		miPanel->second->release();
		SAFE_DEL(miPanel->second);
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
			if (PtInRect(&(miPanel->second->getPanelRect()), g_ptMouse))
			{
				Select_Panel_id = miPanel->first;
				//cout << "Select Panel ID : " << Select_Panel_id << endl;
				//cout << "Panel Type : " << miPanel->second->getPanelEnum() << endl;
				//cout << "[ Connet Pannel ]" << endl;

				for (int i = 0; i < 5; ++i)
				{
					if (miPanel->second->getLinkedId(i) != -1)
						cout << "Linked P :" << miPanel->second->getLinkedId(i) << endl;
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

int PanelManager::Search_intfacePanel()
{
	for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
	{
		if (PtInRect(&(miPanel->second->getPanelRect()), g_ptMouse))
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
	PanelSave ps;

	FILE *p_file = fopen("TestMap.txt", "w");

	for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
	{
		ps.alience = miPanel->second->getPanelAlience();
		ps.change = miPanel->second->getPanelChanged();

		ps.LinkedNum = miPanel->second->getLinkedPanelNum();

		for (int i = 0; i < ps.LinkedNum; ++i)
			ps.Linked[i] = miPanel->second->getLinkedId(i);

		ps.panelEnum = miPanel->second->getPanelEnum();
		ps.panelId = miPanel->first;
		ps.pos = miPanel->second->getPanelPos();
		ps.rc = miPanel->second->getPanelRect();


		if (NULL != p_file)
			fwrite(&ps, 1, sizeof(PanelSave), p_file);

		ZeroMemory(&ps, sizeof(PanelSave));
	}

	fclose(p_file);

	cout << "Save Complete" << endl;

}

void PanelManager::LoadPanel_Data(string path)
{
	PanelSave ps;
	Panel* p;

	FILE *p_file = fopen("../../_TextTable/TestMap.txt", "r");

	while (!feof(p_file))
	{
		if (NULL != p_file)
			fread(&ps, sizeof(PanelSave), 1, p_file);

		p = new Panel;

		for (int i = 0; i < ps.LinkedNum; ++i)
			p->setLinkedId(ps.Linked[i]);

		p->setPanelAlience(ps.alience);
		p->setPanelChanged(ps.change);
		p->setPanelEnum(ps.panelEnum);
		p->setPanelId(ps.panelId);
		p->setPanelPos(ps.pos);
		p->setPanelRect(ps.rc);

		mPanel.insert(make_pair(p->getPanelId(), p));

	}

	SAFE_DEL(p);

	fclose(p_file);

	cout << "File Load Complete" << endl;


}

int PanelManager::Check_ActionPoint_For()
{
	int cnt = 0;

	for (miPanel = mPanel.begin(); miPanel != mPanel.end(); ++miPanel)
	{
		if (miPanel->second->getPanelAlience() == ALIANCE_GRIFFON)
		{
			if (miPanel->second->getPanelEnum() == HQ || miPanel->second->getPanelEnum() == HELIPORT)
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

			//1. ���� �ֺ� Ÿ���߿� �����ɱ������ִ��� �Ǵ�
			if (mPanel.find(linkP)->second->getPanelAlience() == ALIANCE_NONE)
			{
				//cout << "Panel : " << miPanel->first << endl;
				//cout << "There is NONE Tile...\n" << endl;
				al = true;
				break;
			}

			//2. �����ɱ����� ���ٸ� �ڽŰ� �ٸ� ������ ���ɼ��� �Ǵ�
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
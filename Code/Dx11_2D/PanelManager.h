#pragma once
#include "Panel.h"

struct tagPanelLink
{
	int id1, id2;
	tagPanelLink() {}
	tagPanelLink(int _i1, int _i2) : id1(_i1), id2(_i2) {}
};

struct PanelSave
{
	RECT rc;
	POINT pos;

	PANEL_CLASSIFY panelEnum;	//�г��� ����
	TATICDOLL_ALIANCE_TYPE alience;	//�г��� ���⼼��

	int panelId;

	bool change;

	int Linked[PANEL_LINK_LIMIT];
	int LinkedNum;
};

class PanelManager
{
private:
	map<int, Panel*> mPanel;
	map<int, Panel*>::iterator miPanel;

	int Select_Panel_id;	//-1�� �̼���
	int tempPanel;

	vector<tagPanelLink> allLink;

public:
	PanelManager(void);
	~PanelManager(void);

	HRESULT init();

	int Create_Panel(int x, int y);
	void Release_Panel();
	void Update_Panel();
	void Render_Panel();
	void Render_PanelLink();

	void Search_SelPanel();
	void Search_SelPanel(int keyboard);
	void Search_SelectPanel();
	int Search_intfacePanel();
	void Select_Panel_Classify(PANEL_CLASSIFY pc);
	void Select_Panel_Alience(TATICDOLL_ALIANCE_TYPE ac);
	void Delete_Panel(int id);
	void UnLink_Panel(int id);

	bool Panel_Link(int id1, int id2);

	void SavePanel_Data(char* filename);
	void LoadPanel_Data(string path);

	int Check_ActionPoint_For();
	void Check_AroundPanel(TATICDOLL_ALIANCE_TYPE ac);	//�ֺ����ɿ���


public:
	inline int getTotalPanelNum() { return mPanel.size(); }

	inline int getSelectPID() { return Select_Panel_id; }
	inline void setSelectPID() { Select_Panel_id = -1; }	//	@ ��� : ���� �Լ��� ��ü�Ұ�
	inline void resetSelectPID() { Select_Panel_id = -1; }	//	@ ���� : -1�� �ʱ�ȭ�Ϸ��� �̰��� ���

	inline int getSelectPanelId() { if (this->findPanel(Select_Panel_id) != NULL)return Select_Panel_id; else return -1; }

	inline Panel* findPanel(int id) { 
		if (mPanel.find(id) != mPanel.end())
			return mPanel.find(id)->second; 
		else 
			return NULL; 
	}

	inline vector<tagPanelLink>& getAllLink() { return allLink; }
};
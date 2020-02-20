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

	PANEL_CLASSIFY panelEnum;	//패널의 종류
	TATICDOLL_ALIANCE_TYPE alience;	//패널의 영향세력

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

	int Select_Panel_id;	//-1이 미선택
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
	int Search_intfacePanel();
	void Select_Panel_Classify(PANEL_CLASSIFY pc);
	void Select_Panel_Alience(TATICDOLL_ALIANCE_TYPE ac);
	void Delete_Panel(int id);
	void UnLink_Panel(int id);

	bool Panel_Link(int id1, int id2);

	void SavePanel_Data(char* filename);
	void LoadPanel_Data(string path);

	int Check_ActionPoint_For();
	void Check_AroundPanel(TATICDOLL_ALIANCE_TYPE ac);	//주변점령여부


public:
	inline int getTotalPanelNum() { return mPanel.size(); }

	inline int getSelectPID() { return Select_Panel_id; }
	inline void setSelectPID() { Select_Panel_id = -1; }

	inline int getSelectPanelId() { if (this->findPanel(Select_Panel_id) != NULL)return Select_Panel_id; else return -1; }

	inline Panel* findPanel(int id) { if (mPanel.find(id) != mPanel.end())return mPanel.find(id)->second; else return NULL; }

	inline vector<tagPanelLink>& getAllLink() { return allLink; }
};
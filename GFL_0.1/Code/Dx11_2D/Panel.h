#pragma once

#define PANELSIZE_X		50
#define PANELSIZE_Y		50

#define PANEL_LINK_LIMIT	10

enum PANEL_CLASSIFY
{
	NONE = 0,
	HELIPORT,
	HQ
};

class Panel
{
private:
	RECT rc;
	POINT pos;

	PANEL_CLASSIFY panelEnum;		//패널의 종류
	TATICDOLL_ALIANCE_TYPE alience;		//패널의 영향세력
	TATICDOLL_ALIANCE_TYPE change_alie;	//변화할 세력

	bool change;

	//Linked Panel
	vector<int> LinkedId;
	vector<int>::iterator pLinkedId;


public:
	Panel(void);
	~Panel(void);

	static int panelId;

	//base Initializing
	HRESULT init();
	void release();

	HRESULT Create(int x, int y);
	void update();
	void render();
	void LineRender();

	bool FindLinkedPanel(int findnum);

public:
	inline int getLinkedPanelNum() { return LinkedId.size(); }

	inline int getLinkedId(int num) { if (LinkedId.size() > num) return LinkedId[num]; else return -1; }
	inline bool setLinkedId(int num)
	{
		bool ist = false;
		for (pLinkedId = LinkedId.begin(); pLinkedId != LinkedId.end(); ++pLinkedId)
			if (*pLinkedId == num)
			{
				ist = true;
				break;
			}

		if (!ist)
		{
			LinkedId.push_back(num);
			return false;	//같은게 없다
		}
		else
			return true;	//같은게 있다

	}

	inline void deleteLinkedId(int num)
	{
		for (pLinkedId = LinkedId.begin(); pLinkedId != LinkedId.end(); ++pLinkedId)
			if (*pLinkedId == num)
			{
				LinkedId.erase(pLinkedId);
				break;
			}

	}


	inline bool getPanelChanged() { return change; }
	inline void setPanelChanged(bool c) { change = c; }

	inline POINT getPanelPos() { return pos; }
	inline void setPanelPos(POINT p) { pos = p; }

	inline int getPanelId() { return panelId; }
	inline void setPanelId(int id) { panelId = id; }

	inline RECT getPanelRect() { return rc; }
	inline void setPanelRect(RECT r) { rc = r; }

	inline void setPanelEnum(PANEL_CLASSIFY pc) { panelEnum = pc; }
	inline PANEL_CLASSIFY getPanelEnum() { return panelEnum; }

	inline void setPanelAlience(TATICDOLL_ALIANCE_TYPE ac) { alience = ac; }
	inline TATICDOLL_ALIANCE_TYPE getPanelAlience() { return alience; }

	inline void setAroundAlience(TATICDOLL_ALIANCE_TYPE ac) { change_alie = ac; }

};
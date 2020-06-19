#pragma once

#define PANEL_LINK_LIMIT	10


enum PANEL_CLASSIFY
{
	PANEL_CLASS_NONE = 0,
	PANEL_CLASS_HELIPORT,
	PANEL_CLASS_HQ
};

struct tagArea
{
	D3DXVECTOR2 AreaCenter;
	D3DXVECTOR2 AreaRaius;
};

struct BattlePlagData
{
	BYTE battlePlag;					//	���� ����

	BYTE protectObjectNumber;			//	��ȣ/���� �ؾ��ϴ� ������Ʈ ����
	vector<tagArea> protectArea;		//	��ȣ/���� �ؾ��ϴ� ������Ʈ ��� ����

	BattlePlagData() : battlePlag(1 << 1), protectObjectNumber(0) {}
};

class Panel
{
private:
	const FLOAT	PANEL_SIZE_X = 60.0f;
	const FLOAT	PANEL_SIZE_Y = 60.0f;
	const FLOAT PANEL_SIZE_X_HALF = 30.0f;
	const FLOAT PANEL_SIZE_Y_HALF = 30.0f;

	const D3DXCOLOR PANEL_COLOR_NONE = D3DXCOLOR(1, 1, 1, 1);
	const D3DXCOLOR PANEL_COLOR_GRF = D3DXCOLOR(0.1f, 0.8f, 1, 1);
	const D3DXCOLOR PANEL_COLOR_SNV = D3DXCOLOR(0.9f, 0, 0, 1);
	const D3DXCOLOR PANEL_COLOR_PRD = D3DXCOLOR(1, 1, 1, 1);
	const D3DXCOLOR PANEL_COLOR_SSO = D3DXCOLOR(1, 1, 1, 1);

	const D3DXVECTOR2 PANEL_NORMAL_SIZE = D3DXVECTOR2(PANEL_SIZE_X, PANEL_SIZE_Y);
	const D3DXVECTOR2 PANEL_HELI_SIZE = D3DXVECTOR2(PANEL_SIZE_X + 10, PANEL_SIZE_Y + 10);
	const D3DXVECTOR2 PANEL_HQ_SIZE = D3DXVECTOR2(PANEL_SIZE_X + 15, PANEL_SIZE_Y + 15);

private:
	RECT rc;
	D3DXVECTOR2 pos;

	PANEL_CLASSIFY panelEnum;		//�г��� ����
	TATICDOLL_ALIANCE_TYPE alience;		//�г��� ���⼼��
	TATICDOLL_ALIANCE_TYPE change_alie;	//��ȭ�� ����

	string battleAtlas;				//	���� ��׶��� �̹���

	BattlePlagData plagData;		//	�÷��� ������

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

	HRESULT Create(float x, float y);
	HRESULT Create(D3DXVECTOR2 _position);
	void update();
	void render();
	void LineRender();

	void setPlagData(BattlePlagData _plagData);
	BattlePlagData getPlagData();
	bool FindLinkedPanel(int findnum);

public:
	inline string getBattleAtlasKey() { return battleAtlas; }
	inline void setBattleAtlasKey(string key) { battleAtlas = key; }

	inline const vector<int>& getLinkedList() { return LinkedId; }

	inline void reserveLinkedList(size_t _size) { LinkedId.reserve(_size); }

	inline int getLinkedPanelNum() { return LinkedId.size(); }

	inline int getLinkedId(int num) { if (static_cast<int>(LinkedId.size()) > num) return LinkedId[num]; else return -1; }
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
			return false;	//������ ����
		}
		else
			return true;	//������ �ִ�

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

	inline D3DXVECTOR2 getPanelPos() { return pos; }
	inline void setPanelPos(D3DXVECTOR2 p) { pos = p; }

	inline int getPanelId() { return panelId; }
	inline void setPanelId(int id) { panelId = id; }

	inline const RECT& getPanelRect() { return rc; }
	inline const RECT* pcgetPanelRect() { return &rc; }
	inline RECT* pgetPanelRect() { return &rc; }
	inline RECT& rgetPanelRect() { return rc; }
	inline void setPanelRect(RECT r) { rc = r; }

	inline void setPanelEnum(PANEL_CLASSIFY pc) { panelEnum = pc; }
	inline PANEL_CLASSIFY getPanelEnum() { return panelEnum; }

	inline void setPanelAlience(TATICDOLL_ALIANCE_TYPE ac) { alience = ac; }
	inline TATICDOLL_ALIANCE_TYPE getPanelAlience() { return alience; }

	inline void setAroundAlience(TATICDOLL_ALIANCE_TYPE ac) { change_alie = ac; }

};
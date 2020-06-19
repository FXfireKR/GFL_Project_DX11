#pragma once
#include "Scene.hpp"

class TutorialScene : public Scene
{
private:
	//	Instad of #define ->
	const float		ZORDER_REFRESH_TIME = 0.2f;

	const SINT		PLAYER_TARGET_SQUAD = 1;

	const int		TATICDOOL_NONE = 0x0000;			//	0
	const int		TATICDOOL_CONTROLL = 0x0001;		//	1
	const int		TATICDOOL_MOVE = 0x0010;			//	16
	const int		TATICDOOL_SKILL = 0x0100;			//	256
	const int		TATICDOOL_CHANGE_TARGET = 0x1000;	//	4096

	const LPCWSTR	TUTORIAL_TEXT_FONT = L"�������";
	const float		TUTORIAL_TEXT_WIDTH = 750.0f;
	const float		TUTORIAL_TEXT_HEIGHT = 100.0f;
	const float		TUTORIAL_BOX_POS_X = 300.0f;
	const float		TUTORIAL_BOX_POS_Y = WINSIZEY - 200.0f;

	//	Ʃ�丮�� �� Conversation�� �ൿ �÷��׿� ����Ʈ
	struct TutorialConvExtra
	{
		int convIndex;
		int plagCode;
		string Quest;
	};

	struct TutorialSpawn
	{
		int convIndex;
		int spawnNum;
		vector<D3DXVECTOR2> coord;
	};

private:
	int			curTacdollState;		//	���� ���������� ���� �÷���
	bool		lockCameraMove;			//	ī�޶� �����̱� ��Ȱ��ȭ
	bool		lockConv;				//	��ȭâ ��Ȱ��ȭ

	BaseTaticDoll*	curTacDoll;

	string		bkKey;
	string		curSound;
	RECT		rctxt;

	int textCount;
	float opc;

	bool SerifOv;
	bool Next;
	bool Initialize;

	vector<Convers>				vCon;
	vector<TutorialConvExtra>	vQuest;
	vector<TutorialSpawn>		vSpawn;

	string		Quest;
	bool		questComplete;


	string nextScene;		//	���� Scene Key
	string nextReadFile;	//	���� �о�鿩�� �� ����

	vector<POINT>					vRendList;
	map<FLOAT, map<FLOAT, POINT>>	mRendList;
	vector<LoadResourceData>		vLoadList;

	vector<SINT>					vIDSave;

public:
	TutorialScene();
	~TutorialScene();

	// Scene��(��) ���� ��ӵ�
	virtual void init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

private:
	void questUpdate();
	void unLoad();

	void scriptUpdate();
	void scriptRender();
	void readScript();

	void ZOrder_Sort();
};
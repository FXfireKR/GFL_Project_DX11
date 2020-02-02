#pragma once
#include "Scene.h"

class TutorialScene : public Scene
{
private:
	//	Instad of #define ->
	const float		ZORDER_REFRESH_TIME = 0.2f;

	const int TATICDOOL_CONTROLL		= 0x0001;
	const int TATICDOOL_MOVE			= 0x0010;
	const int TATICDOOL_SKILL			= 0x0100;
	const int TATICDOOL_CHANGE_TARGET	= 0x1000;

	const LPCWSTR TUTORIAL_TEXT_FONT = L"�������";
	const float TUTORIAL_TEXT_WIDTH = 750.0f;
	const float TUTORIAL_TEXT_HEIGHT = 100.0f;
	const float TUTORIAL_BOX_POS_X = 300.0f;
	const float TUTORIAL_BOX_POS_Y = WINSIZEY - 200.0f;

private:
	int			curTacdollState;		//	���� ���������� ���� �÷���
	bool		lockCameraMove;			//	ī�޶� �����̱� ��Ȱ��ȭ
	bool		lockConv;				//	��ȭâ ��Ȱ��ȭ

	string		bkKey;
	string		curSound;
	RECT		rctxt;

	int textCount;
	float opc;

	bool SerifOv;
	bool Next;
	bool Initialize;

	vector<Convers> vCon;

	string nextScene;		//	���� Scene Key
	string nextReadFile;	//	���� �о�鿩�� �� ����

	vector<POINT>					vRendList;
	map<FLOAT, map<FLOAT, POINT>>	mRendList;

public:
	TutorialScene();
	~TutorialScene();

	// Scene��(��) ���� ��ӵ�
	virtual void init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

private:
	void scriptUpdate();
	void scriptRender();
	void readScript();

	void ZOrder_Sort();
};
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

	const LPCWSTR TUTORIAL_TEXT_FONT = L"나눔고딕";
	const float TUTORIAL_TEXT_WIDTH = 750.0f;
	const float TUTORIAL_TEXT_HEIGHT = 100.0f;
	const float TUTORIAL_BOX_POS_X = 300.0f;
	const float TUTORIAL_BOX_POS_Y = WINSIZEY - 200.0f;

private:
	int			curTacdollState;		//	현재 전술인형의 상태 플래그
	bool		lockCameraMove;			//	카메라 움직이기 비활성화
	bool		lockConv;				//	대화창 비활성화

	string		bkKey;
	string		curSound;
	RECT		rctxt;

	int textCount;
	float opc;

	bool SerifOv;
	bool Next;
	bool Initialize;

	vector<Convers> vCon;

	string nextScene;		//	다음 Scene Key
	string nextReadFile;	//	다음 읽어들여야 할 내용

	vector<POINT>					vRendList;
	map<FLOAT, map<FLOAT, POINT>>	mRendList;

public:
	TutorialScene();
	~TutorialScene();

	// Scene을(를) 통해 상속됨
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
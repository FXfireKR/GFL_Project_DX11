#pragma once
#include "Scene.h"

#define TEXT_FONT		L"나눔고딕"

#define TEXTBOX_WIDTH	750.0F
#define TEXTBOX_HEIGH	100.0F

#define TEXTBOX_POSX	300.0F
#define TEXTBOX_POSY	WINSIZEY - 200.0F

#define TEXT_SPEAKER_ACL	0.7F


class StoryScene : public Scene
{
private:
	string bkKey;
	string curSound;
	RECT rctxt;

	int textCount;
	float opc;

	bool SerifOv;
	bool Next;
	bool Initialize;

	vector<Convers> vCon;

	string nextScene;		//	다음 Scene Key
	string nextReadFile;	//	다음 읽어들여야 할 내용
	vector<LoadResourceData>		vLoadList;

public:
	StoryScene();
	~StoryScene();


	// Scene을(를) 통해 상속됨
	void init() override;
	void release() override;
	void update() override;
	void render() override;
	void unLoad();
	void ReadScript();
};
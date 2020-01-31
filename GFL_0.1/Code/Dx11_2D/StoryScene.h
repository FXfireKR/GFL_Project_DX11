#pragma once
#include "Scene.h"

#define TEXT_FONT		L"나눔고딕"

#define TEXTBOX_WIDTH	750.0F
#define TEXTBOX_HEIGH	100.0F

#define TEXTBOX_POSX	300.0F
#define TEXTBOX_POSY	WINSIZEY - 200.0F

#define TEXT_SPEAKER_ACL	0.7F

//대화문 출력시 필요한 것들을 저장해둠
struct Convers
{
	string text, SpeakName;	//대화내용, 화자이름
	string bgmKey;			//배경음악 파일이름
	string bkKey;			//배경이미지 파일이름
	string curSound;		//현재 재생되는 BGM

	int Speaker;
	vector<string> vImageKey;
};


class StoryScene : public Scene
{
private:
	HANDLE hFile;
	DWORD dwRead;

	string bkKey;
	string curSound;
	RECT rctxt;

	int textCount;
	float opc;

	bool SerifOv;
	bool Next;
	bool Initialize;

	vector<Convers> vCon;

public:
	StoryScene();
	~StoryScene();


	// Scene을(를) 통해 상속됨
	void init() override;
	void release() override;
	void update() override;
	void render() override;
	void ReadScript();
};
#pragma once
#include "Scene.h"
#include "Image.h"

#define BGM_BASEROOT	"../SoundData/Bgm/"
#define BGI_BASEROOT	"../RemakeResource/"

#define TEXT_FONT		L"�������"

#define TEXTBOX_WIDTH	750.0F
#define TEXTBOX_HEIGH	400.0F

#define TEXTBOX_POSX	300.0F
#define TEXTBOX_POSY	WINSIZEY - 200.0F

#define TEXT_SPEAKER_ACL	0.7F

//��ȭ�� ��½� �ʿ��� �͵��� �����ص�
struct Convers
{
	string text, SpeakName;	//��ȭ����, ȭ���̸�
	string bgmKey;			//������� �����̸�
	string bkKey;			//����̹��� �����̸�
	string curSound;		//���� ����Ǵ� BGM

	int Speaker;
	vector<string> vImageKey;
};

class StoryScene : public Scene
{
private:
	HANDLE hFile;
	DWORD dwRead;
	SoundManager* sound;

	string bkKey;
	string curSound;
	RECT rctxt;

	Image* img;

	int textCount;
	float opc;

	bool SerifOv;
	bool Next;
	bool Initialize;

	vector<Convers> vCon;

public:
	StoryScene();
	~StoryScene();

	// Scene��(��) ���� ��ӵ�
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;

	void ReadScript();
};
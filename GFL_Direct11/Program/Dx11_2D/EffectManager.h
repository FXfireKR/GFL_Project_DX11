#pragma once

#include "Effect.h"
#include "FrameImage.h"
#include "singleton.h"

#define MAX_EFFECT_RENDER	40		//�ѹ��� �ִ� 40���� �������Ҽ��ִ�


class EffectManager : public singleton<EffectManager>
{
private:
	struct FrameEffectInfo
	{
		POINT perSize;			//
		FPNT perSizeDec;
		FrameImage* img;
	};

private:
	vector<Effect*> vEffect;			//�̸����� �������� ������ ������.
	map<string, FrameEffectInfo*> mImage;		//�̹����� �Ҵ��� �������ִ´�.

public:
	EffectManager();
	~EffectManager();

	HRESULT Init_EffectManager();
	void Release_EffectManager();

	//������������ ����Ʈ�� ���Ǵ°͵��� �ε��ؿ´�.
	HRESULT Create_Sprite(string key, int Width, int Heigh);

	HRESULT Create_(string key, FLOAT posx, FLOAT posy, float _fps, FLOAT acl = 1.0F, FLOAT opc = 1.0F);

	void update();
	void render();
};
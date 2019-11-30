#pragma once

#include "Effect.h"
#include "FrameImage.h"
#include "singleton.h"

#define MAX_EFFECT_RENDER	40		//한번에 최대 40개를 랜더링할수있다


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
	vector<Effect*> vEffect;			//이리지를 랜더링할 정보를 가진다.
	map<string, FrameEffectInfo*> mImage;		//이미지를 할당해 가지고있는다.

public:
	EffectManager();
	~EffectManager();

	HRESULT Init_EffectManager();
	void Release_EffectManager();

	//전투시작전에 이펙트로 사용되는것들을 로드해온다.
	HRESULT Create_Sprite(string key, int Width, int Heigh);

	HRESULT Create_(string key, FLOAT posx, FLOAT posy, float _fps, FLOAT acl = 1.0F, FLOAT opc = 1.0F);

	void update();
	void render();
};
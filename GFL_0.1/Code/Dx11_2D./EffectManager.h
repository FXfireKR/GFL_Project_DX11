#pragma once

#include "Effect.h"
#include "FrameImage.h"
#include "memorypool.h"
#include "singleton.h"

class EffectManager : public singleton<EffectManager>
{
private:
	const UINT MAX_EFFECT_RENDER = 40;
	FrameImage* img;						//	�������ɶ� ���Ǵ� �̹���


private:
	memorypool<Effect>* Effects;

public:
	EffectManager();
	~EffectManager();

	HRESULT Init_EffectManager();
	void Release_EffectManager();

	//������������ ����Ʈ�� ���Ǵ°͵��� �ε��ؿ´�.
	HRESULT Create_Sprite(string key, int Width, int Heigh);

	HRESULT createEffect(string _key, D3DXVECTOR2 _pos, float _fps, float _scl = 1.0f, float _opc = 1.0f, bool _flip = false);
	HRESULT Create_(string key, FLOAT posx, FLOAT posy, float _fps, FLOAT acl = 1.0F, FLOAT opc = 1.0F, bool _flip = false);

	void update();
	void render();
};
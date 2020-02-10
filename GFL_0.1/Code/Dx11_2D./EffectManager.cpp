#include "stdafx.h"
#include "EffectManager.h"

EffectManager::EffectManager()
	: Effects(nullptr), img(nullptr)
{
	if (Effects == nullptr)
		Effects = new memorypool<Effect>(MAX_EFFECT_RENDER);

	if (img == nullptr)
		img = new FrameImage;
}

EffectManager::~EffectManager()
{
	SAFE_DELETE(Effects);
	SAFE_DELETE(img);
}

HRESULT EffectManager::Init_EffectManager()
{
	//LOADMANAGER->Add_LoadTray("EXPLO1", "../../_Textures/explo.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	//LOADMANAGER->Add_LoadTray("CLOUD", "../../_Textures/shotCloud.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	return S_OK;
}

void EffectManager::Release_EffectManager()
{
}

HRESULT EffectManager::Create_Sprite(string key, int Width, int Heigh)
{
	return S_OK;
}

HRESULT EffectManager::createEffect(string _key, D3DXVECTOR2 _pos, float _fps, float _scl, float _opc, bool _flip)
{
	ImageResource* loadImage = nullptr;
	if ((loadImage = IMAGEMAP->getImageInfo(_key)) != nullptr)
	{
		Effect* _new = Effects->pickUpPool();
		_new->Create_Effect(_key, _pos, loadImage->perImageSize, loadImage->Frame, _fps, _scl, _opc);
		Effects->create(_new);

		return S_OK;
	}
	return E_FAIL;
}

HRESULT EffectManager::Create_(string key, FLOAT posx, FLOAT posy, float _fps, FLOAT acl, FLOAT opc, bool _flip)
{
	return S_OK;
}

void EffectManager::update()
{
	for (size_t i = Effects->getUsage().size(); i > 0; --i)
	{
		Effect* Iter = Effects->getUsage()[i - 1];
		if (!Iter->isRender()) continue;

		Iter->update();

		if (Iter->isEffectEnd())
			Effects->release(i - 1);
	}
}

void EffectManager::render()
{
	for (size_t i = Effects->getUsage().size(); i > 0; --i)
	{
		Effect* Iter = Effects->getUsage()[i - 1];
		if (!Iter->isRender()) continue;

		img->render(Iter->getKey(), Iter->getEffectScale(), Iter->getEffectTrans(), Iter->getMinTexcoord(), Iter->getMaxTexcoord());
	}
}
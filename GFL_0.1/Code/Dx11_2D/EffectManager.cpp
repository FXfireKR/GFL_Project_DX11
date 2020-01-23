#include "stdafx.h"
#include "EffectManager.h"

EffectManager::EffectManager()
	: Effects(nullptr), img(nullptr)
{
	if (Effects == nullptr)
		Effects = new memorypool<Effect*>(MAX_EFFECT_RENDER);

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
	if (IMAGEMAP->getImageInfo(key) != nullptr)
	{

	}

	if (!mImage.count(key))
	{
		FrameEffectInfo* info = new FrameEffectInfo;
		info->img = new FrameImage(key);
		info->perSize.x = Width;
		info->perSize.y = Heigh;
		info->perSizeDec.x = (float)Width / info->img->getImageSize().x;
		info->perSizeDec.y = (float)Heigh / info->img->getImageSize().y;

		mImage.insert(make_pair(key, info));

		return S_OK;
	}

	return E_FAIL;
}

HRESULT EffectManager::Create_(string key, FLOAT posx, FLOAT posy, float _fps, FLOAT acl, FLOAT opc, bool _flip)
{
	if (mImage.count(key))
	{
		auto it = mImage.find(key)->second;
		Effect* temp = new Effect();
		temp->Init_Effect();

		POINT max;
		max.x = it->img->getImageSize().x / it->perSize.x;
		max.y = it->img->getImageSize().y / it->perSize.y;

		temp->Create_Effect(key, max.x, max.y, _fps, acl, opc);
		temp->Get_Rendorder(posx, posy);
		temp->setFlip(_flip);

		//it->img->setTrans(posX, posY);

		vEffect.push_back(temp);

		return S_OK;
	}
}

void EffectManager::update()
{
	for (auto& it : vEffect)
	{
		if (!it->isRender())continue;

		it->Update_Effect();
	}

}

void EffectManager::render()
{
	for (auto& it : vEffect)
	{
		if (it->isRender())
		{
			auto& eft = mImage.find(it->getKey())->second->img;
			auto& eftInfo = mImage.find(it->getKey())->second;

			float curX = eftInfo->perSizeDec.x * it->getFrameX();
			float curY = eftInfo->perSizeDec.y * it->getFrameY();
			float endX = eftInfo->perSizeDec.x * (it->getFrameX() + 1);
			float endY = eftInfo->perSizeDec.y * (it->getFrameY() + 1);

			if (endX > 1.0f)
				endX = 1;

			if (endY > 1.0f)
				endY = 1;

			eft->render(D3DXVECTOR2(it->getEffectAcl() * eftInfo->perSize.x, it->getEffectAcl() * eftInfo->perSize.x), D3DXVECTOR3(0, 0, it->isFlip() ? 180.0F : 0),
				D3DXVECTOR2(it->getEffectPositionX(), it->getEffectPositionY()), curX, endX, curY, endY);
		}
	}
}
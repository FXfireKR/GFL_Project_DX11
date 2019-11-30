#include "stdafx.h"
#include "EffectManager.h"

EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
}

HRESULT EffectManager::Init_EffectManager()
{
	LOADMANAGER->Add_LoadTray("EXPLO1", "../../_Textures/explo.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);


	return S_OK;
}

void EffectManager::Release_EffectManager()
{
	//전투 종료하면 모든 동적할당 이미지 이펙트를 해제해주고, 정리한다.

	for (auto& it : mImage)
	{
		if (it.second != nullptr)
		{
			if (it.second->img != nullptr)
			{
				it.second->img->release();
				SAFE_DEL(it.second->img);
			}
			SAFE_DEL(it.second);
		}
	}
	mImage.clear();

	for (auto& it : vEffect)
	{
		it->Release_Effect();
		SAFE_DEL(it);
	}

	vEffect.clear();
}

HRESULT EffectManager::Create_Sprite(string key, int Width, int Heigh)
{
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

HRESULT EffectManager::Create_(string key, FLOAT posx, FLOAT posy, float _fps, FLOAT acl, FLOAT opc)
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

		//it->img->setTrans(posX, posY);

		vEffect.push_back(temp);

		return S_OK;
	}
}

void EffectManager::update()
{
	for (auto& it : vEffect)
	{
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

			eft->render(D3DXVECTOR2(it->getEffectAcl() * eftInfo->perSize.x, it->getEffectAcl() * eftInfo->perSize.x), D3DXVECTOR3(0, 0, 0),
				D3DXVECTOR2(it->getEffectPositionX(), it->getEffectPositionY()), curX, endX, curY, endY);
		}
	}
}
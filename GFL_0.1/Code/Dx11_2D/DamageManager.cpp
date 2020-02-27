#include "stdafx.h"
#include "DamageManager.h"

DamageManager::DamageManager()
{
}

DamageManager::~DamageManager()
{
}

void DamageManager::loadImageList()
{
	LOADMANAGER->Add_LoadTray("0", "../../_Assets/Texture2D/Font/0.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("1", "../../_Assets/Texture2D/Font/1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("2", "../../_Assets/Texture2D/Font/2.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("3", "../../_Assets/Texture2D/Font/3.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("4", "../../_Assets/Texture2D/Font/4.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("5", "../../_Assets/Texture2D/Font/5.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("6", "../../_Assets/Texture2D/Font/6.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("7", "../../_Assets/Texture2D/Font/7.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("8", "../../_Assets/Texture2D/Font/8.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("9", "../../_Assets/Texture2D/Font/9.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOADMANAGER->Add_LoadTray("0a", "../../_Assets/Texture2D/Font/0a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("1a", "../../_Assets/Texture2D/Font/1a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("2a", "../../_Assets/Texture2D/Font/2a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("3a", "../../_Assets/Texture2D/Font/3a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("4a", "../../_Assets/Texture2D/Font/4a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("5a", "../../_Assets/Texture2D/Font/5a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("6a", "../../_Assets/Texture2D/Font/6a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("7a", "../../_Assets/Texture2D/Font/7a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("8a", "../../_Assets/Texture2D/Font/8a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("9a", "../../_Assets/Texture2D/Font/9a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOADMANAGER->Add_LoadTray("miss", "../../_Assets/Texture2D/Font/MISS.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("amo", "../../_Assets/Texture2D/Font/Armo.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("bamo", "../../_Assets/Texture2D/Font/bArmo.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
}

void DamageManager::AllocateMemory()
{
	if (DamageLogger.size() < 1)
	{
		for (int i = 0; i < DAMAGELOGGER_NUM_LIM; ++i)
		{
			DamageLogger.push_back(new tagLogger);
			DamageLogger[i]->isRender = false;
		}
	}
}

void DamageManager::release()
{
	for (auto& iter : DamageLogger)
		SAFE_DEL(iter);
	DamageLogger.clear();
}

void DamageManager::Create_Damage(float x, float y, int _damage, bool _isCiritic)
{
	for (auto& iter : DamageLogger)
	{
		if (iter->isRender)continue;
		else
		{
			iter->alpha = 1.0F;
			iter->damage = _damage < 0 ? "miss" : to_string(_damage);
			iter->isCritical = _isCiritic;
			iter->isRender = true;
			iter->isArmor = false;
			iter->pos.x = x + (rand() % 65) - .0;
			iter->pos.y = y - 40.0f;
			break;
		}
	}
}

void DamageManager::Create_Damage(float x, float y, int _damage, bool _isArmor, bool _isCiritic)
{
	for (auto& iter : DamageLogger)
	{
		if (iter->isRender)continue;
		else
		{
			iter->alpha = 1.0F;
			iter->damage = _damage < 0 ? "miss" : to_string(_damage);
			iter->isCritical = _isCiritic;
			iter->isArmor = _isArmor;
			iter->isRender = true;
			iter->pos.x = x + (rand() % 65) - 30;
			iter->pos.y = y - 40.0f;
			break;
		}
	}
}

void DamageManager::update()
{
	for (auto& iter : DamageLogger)
	{
		if (!iter->isRender) continue;
		else
		{
			iter->pos.y -= DELTA * DAMAGELOGGER_UPSCALE;
			iter->alpha -= DELTA;

			if (iter->alpha < DELTA)
				iter->isRender = false;
		}
	}
}

void DamageManager::render()
{
	for (auto& iter : DamageLogger)
	{
		if (!iter->isRender) continue;
		else
		{
			if (iter->isArmor)
				DRAW->render("amo", DV2(15, 15), DV2(iter->pos.x - 25, iter->pos.y), DCR(1, 1, 1, iter->alpha));

			if (iter->damage.find("miss") == string::npos)
			{
				for (int i = 0; i < iter->damage.size(); ++i)
				{
					string key;
					key.append(iter->damage, i, 1);

					if (!iter->isCritical)
						key.append("a");

					DRAW->render(key, DV2(15, 15), DV2(iter->pos.x + (i * 22), iter->pos.y), DCR(1, 1, 1, iter->alpha));
				}
			}

			else
			{				
				DRAW->render(iter->damage, DV2(32, 10), DV2(iter->pos.x, iter->pos.y), DCR(1, 1, 1, iter->alpha));
			}
		}
	}
}

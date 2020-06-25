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
	LOAD->Add_LoadTray("0", "Texture2D/Font/0.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("1", "Texture2D/Font/1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("2", "Texture2D/Font/2.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("3", "Texture2D/Font/3.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("4", "Texture2D/Font/4.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("5", "Texture2D/Font/5.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("6", "Texture2D/Font/6.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("7", "Texture2D/Font/7.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("8", "Texture2D/Font/8.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("9", "Texture2D/Font/9.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	
	LOAD->Add_LoadTray("0a", "Texture2D/Font/0a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("1a", "Texture2D/Font/1a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("2a", "Texture2D/Font/2a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("3a", "Texture2D/Font/3a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("4a", "Texture2D/Font/4a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("5a", "Texture2D/Font/5a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("6a", "Texture2D/Font/6a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("7a", "Texture2D/Font/7a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("8a", "Texture2D/Font/8a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("9a", "Texture2D/Font/9a.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	
	LOAD->Add_LoadTray("miss", "Texture2D/Font/MISS.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("amo", "Texture2D/Font/Armo.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("bamo", "Texture2D/Font/bArmo.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
}

void DamageManager::AllocateMemory()
{
	if (DamageLogger.size() < 1){
		for (int i = 0; i < DAMAGELOGGER_NUM_LIM; ++i){
			DamageLogger.push_back(new DamageLog);
			DamageLogger[i]->isRender = false;
		}
	}
}

void DamageManager::release()
{
	for (auto& iter : DamageLogger)
		SAFE_DELETE(iter);
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
			iter->pos.x = x + static_cast<float>((rand() % 65) - 10);
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
			iter->pos.y -= DELTA() * DAMAGELOGGER_UPSCALE;
			iter->alpha -= DELTA();

			if (iter->alpha < DELTA())
				iter->isRender = false;
		}
	}
}

void DamageManager::render()
{
	for (auto& iter : DamageLogger)
	{
		if (!iter->isRender) continue;
		else{
			if (iter->isArmor)
				DRAW->render("amo", VEC2(30, 30), VEC2(iter->pos.x - 25, iter->pos.y), COLR(1, 1, 1, iter->alpha));

			if (iter->damage.find("miss") == string::npos){
				for (size_t i = 0; i < iter->damage.size(); ++i){
					string key;
					key.append(iter->damage, i, 1);

					if (!iter->isCritical)
						key.append("a");

					DRAW->render(key, VEC2(30, 30), VEC2(iter->pos.x + (i * 22), iter->pos.y), COLR(1, 1, 1, iter->alpha));
				}
			}

			else
				DRAW->render(iter->damage, VEC2(64, 20), VEC2(iter->pos.x, iter->pos.y), COLR(1, 1, 1, iter->alpha));			
		}
	}
}

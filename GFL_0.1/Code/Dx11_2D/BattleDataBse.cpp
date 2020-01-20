#include "stdafx.h"
#include "BattleDataBse.h"

BattleDataBse::BattleDataBse()
	: player(nullptr)
{
}

BattleDataBse::~BattleDataBse()
{
	this->release();
}

HRESULT BattleDataBse::init()
{
	player = new PlayerData;
	player->init();


	EngageEnemySquad = 1;

	squadSNV = new SquadManager<TaticDoll*>;

	ARCHITECT* _new = new ARCHITECT;
	_new->init();
	vecBattleObjects.push_back(_new);

	return S_OK;
}

void BattleDataBse::release()
{
	//release에서 자동으로 소멸시켜준다
	if (squadSNV != nullptr)
	{
		squadSNV->Release();
		delete squadSNV;
		squadSNV = nullptr;
	}//mSNVsquad.clear();

	SAFE_DELETE(player);

	for (auto& it : vecBattleObjects)
	{
		it->release();
		SAFE_DELETE(it);
	}
}
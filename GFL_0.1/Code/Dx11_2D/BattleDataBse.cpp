#include "stdafx.h"
#include "BattleDataBse.h"

BattleDataBse::BattleDataBse()
	: player(nullptr)
{
	player = new PlayerData;
	player->init();

	EngageEnemySquad = 1;

	squadSNV = new SquadManager<TaticDoll*>;
}

BattleDataBse::~BattleDataBse()
{
	this->release();
}

HRESULT BattleDataBse::init()
{
	//ARCHITECT* _new = new ARCHITECT;
	//_new->init();
	//vecBattleObjects.push_back(_new);

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

void BattleDataBse::insertObject_AtSquad(SINT id)
{
	//	데이터 삭제
	deleteAll();

	for (auto& taticDoll : squadSNV->Call_Squad(id)->mSquad)	
		vecBattleObjects.push_back(taticDoll.second);
}

void BattleDataBse::insertObject(TaticDoll * _pointer)
{
	vecBattleObjects.push_back(_pointer);
}

void BattleDataBse::deleteObject(const TaticDoll * _pointer)
{
	vector<TaticDoll*>::iterator iter;

	for (iter = vecBattleObjects.begin(); iter != vecBattleObjects.end(); ++iter)
	{
		if (*iter == _pointer)
		{
			(*iter)->release();
			SAFE_DELETE(*iter);
			vecBattleObjects.erase(iter);
			break;
		}
	}
}

void BattleDataBse::deleteObject(size_t at)
{
	if (vecBattleObjects.size() < at)
	{
		vector<TaticDoll*>::iterator iter = vecBattleObjects.begin() + at;
		(*iter)->release();
		SAFE_DELETE(*iter);
		vecBattleObjects.erase(iter);
	}
}

void BattleDataBse::deleteAll()
{
	for (auto& it : vecBattleObjects)
	{
		it->release();
		SAFE_DELETE(it);
	}
	vecBattleObjects.clear();
}

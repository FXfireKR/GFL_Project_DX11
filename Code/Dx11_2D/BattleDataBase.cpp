#include "stdafx.h"
#include "BattleDataBase.h"

BattleDataBase::BattleDataBase()
	: player(nullptr), squadSNV(nullptr)
{
	if (player == nullptr) {
		player = new PlayerData;
		player->init();
	}

	EngageEnemySquad = 1;

	if (squadSNV == nullptr)
		squadSNV = new SquadManager;
}

BattleDataBase::~BattleDataBase()
{
	this->release();
}

HRESULT BattleDataBase::init()
{
	return S_OK;
}

void BattleDataBase::release()
{
	SAFE_DELETE(squadSNV);
	SAFE_DELETE(player);

	//for (auto& it : vecBattleObjects){
	//	it->release();
	//	SAFE_DELETE(it);
	//}
}

void BattleDataBase::insertObject_AtSquad(SINT id)
{
	//	데이터 삭제
	deleteAll();

	for (auto& taticDoll : squadSNV->callSquad(id)->squadMember)
		vecBattleObjects.push_back(taticDoll.second);
}

void BattleDataBase::insertObject(BaseTaticDoll * _pointer)
{
	vecBattleObjects.push_back(_pointer);
}

void BattleDataBase::deleteObject(const BaseTaticDoll * _pointer)
{
	vector<BaseTaticDoll*>::iterator iter;

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

void BattleDataBase::deleteObject(size_t at)
{
	if (vecBattleObjects.size() < at)
	{
		vector<BaseTaticDoll*>::iterator iter = vecBattleObjects.begin() + at;
		(*iter)->release();
		SAFE_DELETE(*iter);
		vecBattleObjects.erase(iter);
	}
}

void BattleDataBase::deleteAll()
{
	for (auto& it : vecBattleObjects){
		squadSNV->emitSquadMember(it->getID()->Squad_ID, 0);
		it->release();
		SAFE_DELETE(it);
	}
	vecBattleObjects.clear();
}

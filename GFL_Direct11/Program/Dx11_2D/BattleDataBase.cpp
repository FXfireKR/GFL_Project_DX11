#include "stdafx.h"
#include "BattleDataBase.h"
#include "SNVtacDollType.h"

BattleDataBase::BattleDataBase()
{
}

BattleDataBase::~BattleDataBase()
{
}

HRESULT BattleDataBase::init()
{
	vPos.x = vPos.y = 0;
	EngageEnemySquad = 1;

	squadSNV = new SquadManager<SNVtacDoll*>;

	return S_OK;
}

void BattleDataBase::release()
{

	//release에서 자동으로 소멸시켜준다
	if (squadSNV != nullptr)
	{
		squadSNV->Release();
		delete squadSNV;
		squadSNV = nullptr;
	}

	//mSNVsquad.clear();
}

void BattleDataBase::VirtualPos_Limit(int y)
{
	//너비와 높이를 고려해서 가상좌표의 한계점을 걸어준다.
	//생각해보면 Y축만 고정되면 되는거아니냐 ㅡㅡ?

	int limit_Height = y;

	if (vPos.y >= limit_Height)
		vPos.y = limit_Height;

	else if (vPos.y < 0)
		vPos.y = 0;
}
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

	//release���� �ڵ����� �Ҹ�����ش�
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
	//�ʺ�� ���̸� ����ؼ� ������ǥ�� �Ѱ����� �ɾ��ش�.
	//�����غ��� Y�ุ �����Ǹ� �Ǵ°žƴϳ� �Ѥ�?

	int limit_Height = y;

	if (vPos.y >= limit_Height)
		vPos.y = limit_Height;

	else if (vPos.y < 0)
		vPos.y = 0;
}
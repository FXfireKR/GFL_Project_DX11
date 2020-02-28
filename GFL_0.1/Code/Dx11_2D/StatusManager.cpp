#include "stdafx.h"
#include "StatusManager.h"

StatusManager::~StatusManager()
{
}

void StatusManager::create(Status state, double conTime)
{
	buffStatus newbuff;
	newbuff.state = state;
	newbuff.continueTimer = conTime;

	vBuffLsit.push_back(newbuff);
}

void StatusManager::update(double delta)
{
	//	스테이터스 복귀
	StatusInput(mainStatus, maxStatus);

	//	장비 스테이터스 적용
	for (auto& it : *mEquip)
	{
		if (it.second == nullptr) continue;

		auto& atchState = it.second->getState();

		mainStatus->Accuracy += atchState.Accuracy;
		mainStatus->AttackDelay -= (mainStatus->AttackDelay * atchState.AttackDelay) * 0.01f;
		mainStatus->AttackPoint += atchState.AttackPoint;
		mainStatus->CriticPoint += atchState.CriticPoint;
	}

	for (int i = vBuffLsit.size() - 1; i > -1; --i)
	{
		vBuffLsit[i].update_Buff(delta);

		if (vBuffLsit[i].isOver)
			vBuffLsit.erase(vBuffLsit.begin() + i);

		else
		{
			mainStatus->Accuracy += vBuffLsit[i].state.Accuracy;
			mainStatus->AimDelay += vBuffLsit[i].state.AimDelay;
			mainStatus->AttackDelay += vBuffLsit[i].state.AttackDelay;
			mainStatus->AttackPoint += vBuffLsit[i].state.AttackPoint;
			mainStatus->Avoid += vBuffLsit[i].state.Avoid;
			mainStatus->CriticAcl += vBuffLsit[i].state.CriticAcl;
			mainStatus->CriticPoint += vBuffLsit[i].state.CriticPoint;
		}
	}
}

void StatusManager::allClear()
{
	//	모든 버프 제거 및 원상태 복귀
	vBuffLsit.clear();
	StatusInput(mainStatus, maxStatus);
}
#include "stdafx.h"
#include "BuffManager.h"

BuffManager::~BuffManager()
{
	vBuffLsit.reserve(MAX_BUFF_SIZE);
}

void BuffManager::create(Status state, double conTime)
{
	buffStatus newbuff;
	newbuff.state = state;
	newbuff.continueTimer = conTime;

	vBuffLsit.push_back(newbuff);
}

void BuffManager::update(double delta)
{
	StatusInput(mainStatus, maxStatus);

	for (int i = vBuffLsit.size() - 1; i > -1; --i)
	{
		vBuffLsit[i].update_Buff(delta);

		if (vBuffLsit[i].isOver)
			vBuffLsit.erase(vBuffLsit.begin() + i);

		else
		{
			ImGui::Text("LeftBuffTime : %.4f", vBuffLsit[i].continueTimer);

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
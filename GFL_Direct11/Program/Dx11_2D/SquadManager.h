#pragma once
#include "stdafx.h"

template <typename T>
struct tagSquad
{
	SPNT TotalHP;
	SPNT Mre;
	SPNT Bullet;

	UINT nowNodeID;
	UINT nextNodeID;

	UINT squadLeaderID;
	T squadLeader;

	FPNT pos;

	float Angle;

	bool isSelect;
	bool AllDead;

	vector<UINT> vNextNode;
	map<UINT, T> mSquad;
};

template <typename T>
class SquadManager
{
private:
	map<UINT, tagSquad<T>*> mManager;

public:
	//생성자
	SquadManager(int SquadNumber = 3)	//최소 3개의 빈 분대를 창설한다.
	{
		for (int i = 0; i < SquadNumber; ++i)
		{
			tagSquad<T>* temp = new tagSquad<T>;
			temp->squadLeader = nullptr;
			temp->squadLeaderID = 0;
			mManager.insert(make_pair(i + 1, temp));
		}
	};

	//소멸자
	~SquadManager()
	{
		for (auto& it : mManager)
		{
			if (it.second != nullptr)
			{
				delete it.second;
				it.second = nullptr;
			}
		}
	};

	void Insert_SquadMember(UINT squadID, void* mem)
	{
		if (mManager.count(squadID))
		{
			bool sameID = false;

			T temp = (T)mem;

			if (temp->getID()->SquadMem_ID == 0)	//소속이 없다는 뜻이다.
			{
				//동일한 시드 찾기
				for (auto& it : mManager.find(squadID)->second->mSquad)
				{
					if (it.second->getID()->All_ID == temp->getID()->All_ID)
					{
						sameID = true;
						break;
					}
				}

				if (!sameID)
				{
					mManager.find(squadID)->second->mSquad.insert(make_pair(mManager.find(squadID)->second->mSquad.size() + 1, temp));
					temp->getID()->SquadMem_ID = mManager.find(squadID)->second->mSquad.size();
					temp->getID()->Squad_ID = squadID;

					if (mManager.find(squadID)->second->squadLeaderID == 0)
						mManager.find(squadID)->second->squadLeaderID = 1;
				}
			}
		}
	};

	void Emit_SquadMember(UINT squadID, UINT memID)
	{
		if (mManager.count(squadID))
		{
			//SID분대의 MID를 0, 0으로 변동시킨다.
			auto it = mManager.find(squadID);

			int count = it->second->mSquad.begin()->first;
			int oldSize = it->second->mSquad.size();

			if (it->second->mSquad.count(memID))
			{
				auto it2 = it->second->mSquad.find(memID);
				it->second->mSquad.find(memID)->second->getID()->Squad_ID = 0;
				it->second->mSquad.find(memID)->second->getID()->SquadMem_ID = 0;

				it->second->mSquad.erase(it->second->mSquad.find(memID));

				if (it->second->squadLeaderID == memID)
				{
					if (it->second->mSquad.size() < 1)		//남은 분대원이 없다면
						it->second->squadLeaderID = 0;
				}
			}

			if (it->second->mSquad.size() > 0)
			{
				for (int i = 0; i < oldSize;)
				{
					if (it->second->mSquad.count(count + i))
					{
						auto iter = it->second->mSquad.find(count + i);

						if (iter->first == count)
						{
							++i;
							continue;
						}

						int id = iter->first;

						if (!it->second->mSquad.count(id - 1))
						{
							mManager.find(squadID)->second->mSquad.insert(make_pair(id - 1, it->second->mSquad.find(id)->second));
							it->second->mSquad.find(id)->second->getID()->SquadMem_ID = id - 1;
							it->second->mSquad.erase(it->second->mSquad.find(id));
							i = count;
						}
						else
							++i;
					}
					else
						++i;

				}



			}
		}
	};

	void Release()
	{
		for (auto& it : mManager)
		{
			for (auto& it2 : it.second->mSquad)
			{
				if (it2.second != nullptr)
				{
					it2.second->release();
					delete it2.second;
					it2.second = nullptr;
				}
			}

			if (it.second != nullptr)
			{
				delete it.second;
				it.second = nullptr;
			}
		}
	}

	T Call_SquadMember(UINT squadID, UINT memID)
	{
		if (mManager.count(squadID))
		{
			auto it = mManager.find(squadID)->second;

			if (it->mSquad.count(memID))
				return it->mSquad.find(memID)->second;

		}

		return nullptr;
	}

	tagSquad<T>* Call_Squad(UINT squadID) { return mManager.count(squadID) ? mManager.find(squadID)->second : nullptr; }



public:
	inline int getTotalSquadSize() { return mManager.size(); }
	inline int getSquadMemberSize(unsigned int squadID) { return mManager.count(squadID) ? mManager.find(squadID).size() : 0; }
};
#include "stdafx.h"
#include "SquadManager.h"

SquadManager::SquadManager(int SquadNumber)
{
	for (int i = 0; i < SquadNumber; ++i) {
		SquadData* newData = new SquadData;
		newData->squadLeader = nullptr;
		newData->squadLeaderID = -1;
		mManager.insert(make_pair(i + 1, newData));
	}
}

SquadManager::~SquadManager()
{
	for (auto& it : mManager){
		if (it.second != nullptr)
		{
			for (auto& member : it.second->squadMember) {
				member.second->release();
				SAFE_DELETE(member.second);
			}
			delete it.second;
			it.second = nullptr;
		}
	}
}

bool SquadManager::insertSquadMember(SINT squadID, void * mem)
{
	if ((miManager = mManager.find(squadID)) != mManager.end()){
		bool sameID = false;
		BaseTaticDoll* curDoll = (BaseTaticDoll*)mem;
		DOGTAG_ID* curId = curDoll->getID();

		//	소속이 없다는 뜻이다.
		if (curId->SquadMem_ID == -1) {

			//	동일한 시드 찾기
			for (auto& it : miManager->second->squadMember) {
				if (it.second->getID()->All_ID == curDoll->getID()->All_ID) {
					sameID = true;
					break;
				}
			}

			if (!sameID) {
				SINT id = miManager->second->squadMember.size();
				miManager->second->squadMember.insert(make_pair(id, curDoll));
				curId->SquadMem_ID = id;
				curId->Squad_ID = squadID;

				if (miManager->second->squadLeaderID == -1)
					miManager->second->squadLeaderID = 0;
				return true;
			}
		}
		else
			return false;
	}
	else
		return false;
	return false;
}

void SquadManager::emitSquadMember(SINT squadID, SINT memID)
{
	//	is Valid [ squadID ] ?
	if ((miManager = mManager.find(squadID)) != mManager.end()) 
	{
		unordered_map<UINT, BaseTaticDoll*>& curSquad = miManager->second->squadMember;
		if ((iterMember = curSquad.find(memID)) != curSquad.end()) {

			int count = curSquad.begin()->first;
			size_t oldSize = curSquad.size();

			iterMember->second->getID()->Squad_ID = -1;
			iterMember->second->getID()->SquadMem_ID = -1;

			curSquad.erase(iterMember);

			if (miManager->second->squadLeaderID == memID)
				if (curSquad.size() < 1)
					miManager->second->squadLeaderID = -1;	
				
			if (curSquad.size() > 0) {
				for (size_t i = 0; i < oldSize;) 
				{
					ubIter memIterator;

					if ((memIterator = curSquad.find(count + i)) != curSquad.end())
					{
						if (static_cast<int>(memIterator->first) == count) {
							++i;
							continue;
						}

						UINT id = memIterator->first;

						if (!curSquad.count(id - 1))
						{
							curSquad.insert(make_pair(id - 1, curSquad.find(id)->second));
							curSquad.find(id)->second->getID()->SquadMem_ID = id - 1;
							curSquad.erase(curSquad.find(id));
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
	}
}

void SquadManager::release()
{
	for (auto& it : mManager) 
	{
		for (auto& it2 : it.second->squadMember) {
			if (it2.second != nullptr) {
				it2.second->release();
				delete it2.second;
				it2.second = nullptr;
			}
		}

		if (it.second != nullptr) {
			delete it.second;
			it.second = nullptr;
		}
	}
}

BaseTaticDoll * SquadManager::callSquadMember(SINT squadID, SINT memID)
{
	if ((miManager = mManager.find(squadID)) != mManager.end()) {
		unordered_map<UINT, BaseTaticDoll*>& curSquad = miManager->second->squadMember;
		if ((iterMember = curSquad.find(memID)) != curSquad.end())
			return iterMember->second;
	}
	return nullptr;
}

SquadData * SquadManager::callSquad(SINT squadID)
{
	return (miManager = mManager.find(squadID)) != mManager.end() ? miManager->second : nullptr;
}

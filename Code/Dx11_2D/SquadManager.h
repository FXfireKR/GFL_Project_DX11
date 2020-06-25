#pragma once
#include "BaseTaticDoll.h"

typedef struct tagSquad
{
	//	Leader & member Data
	BaseTaticDoll*										squadLeader;
	unordered_map<UINT, BaseTaticDoll*>					squadMember;

	bool				isSelect;
	bool				AllDead;

	//	worldMap Node ID Data
	UINT				nowNodeID;
	UINT				nextNodeID;
	vector<UINT>		vNextNode;

	SINT				squadLeaderID;

	//	squad leader's angle value
	float				rAngle;
	float				dAngle;

	//	squad battle state
	STATE<UINT>			TotalHP;
	STATE<UINT>			Mre;
	STATE<UINT>			Bullet;

	//	squad world position
	D3DXVECTOR2			pos;

	//	squadAliance type
	TATICDOLL_ALIANCE_TYPE	squadAliance;

	//	initialize function
	tagSquad() {
		AllDead = false;
		isSelect = false;
		nowNodeID = 0;
		nextNodeID = 0;
		pos.x = pos.y = 0.0f;
	}
}SquadData;

class SquadManager
{
public:
	SquadManager(int SquadNumber = 3);		//	최소 3개의 빈 분대를 창설한다.
	~SquadManager();						//	deleter

	bool insertSquadMember(SINT squadID, void* mem, bool sameCheck = true);
	void emitSquadMember(SINT squadID, SINT memID);
	void release();
	BaseTaticDoll* callSquadMember(SINT squadID, SINT memID);
	SquadData* callSquad(SINT squadID);

public:
	inline TATICDOLL_ALIANCE_TYPE getAliance_at(SINT squadId) const {
		return mManager.find(squadId)->second->squadAliance;
	}

	inline int getTotalSquadSize() {
		return static_cast<int>(mManager.size());
	}

	inline const bool getValidSquad(SINT squadID) {
		return static_cast<bool>(mManager.count(squadID));
	}

	inline int getSquadMemberSize(unsigned int squadID) {
		return mManager.count(squadID) ? mManager.find(squadID)->second->squadMember.size() : 0;
	}

private:
	unordered_map<SINT, SquadData*>					mManager;
	unordered_map<SINT, SquadData*>::iterator		miManager;

	typedef unordered_map<UINT, BaseTaticDoll*>::iterator ubIter;
	ubIter	iterMember;
};
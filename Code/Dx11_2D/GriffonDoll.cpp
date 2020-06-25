#include "stdafx.h"
#include "GriffonDoll.h"

GriffonDoll::GriffonDoll()
	: IOPsquad(nullptr)
{
	if (IOPsquad == nullptr)
		IOPsquad = new SquadManager;
}

GriffonDoll::~GriffonDoll()
{
	this->release();
}

HRESULT GriffonDoll::init()
{
	return S_OK;
}

void GriffonDoll::release()
{
	for (auto& it : mAllDoll) {
		if (it.second == nullptr || it.second == NULL) continue;
	
		this->exitSquadTacDoll(it.second->getID()->Squad_ID, it.second->getID()->SquadMem_ID);

		it.second->release();
		SAFE_DELETE(it.second);
	}
	mAllDoll.clear();

	if (IOPsquad != nullptr) {
		IOPsquad->release();
		SAFE_DELETE(IOPsquad);
	}
}

HRESULT GriffonDoll::createIOPtacDoll(GRFDOLL_NAME name)
{
	static SINT seed = 0;

	switch (name)
	{
	case GRF_NONE:
		break;

	case GRF_AK12:
	{
		//AK12* _new = new AK12;
		//_new->getID()->All_ID = seed;
		//mAllDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
	break;

	case GRF_9A91:
	{
		nAnOne* _new = new nAnOne;
		_new->getID()->All_ID = seed;
		mAllDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
	break;

	case GRF_PKP:
	{
		Pkp* _new = new Pkp;
		_new->getID()->All_ID = seed;
		mAllDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
	break;

	case GRF_AR15:
	{
		Ar15* _new = new Ar15;
		_new->getID()->All_ID = seed;
		mAllDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
	break;

	case GRF_M4SOP:
	{
		M4sopmod* _new = new M4sopmod;
		_new->getID()->All_ID = seed;
		mAllDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
	break;

	case GRF_NTW20:
	{
		Ntw20* _new = new Ntw20;
		_new->getID()->All_ID = seed;
		mAllDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
	break;

	case GRF_AN94:
	{
		//AN94* _new = new AN94;
		//_new->getID()->All_ID = seed;
		//mAllDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
	break;

	case GRF_KSG:
	{
		Ksg* _new = new Ksg;
		_new->getID()->All_ID = seed;
		mAllDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
	break;

	default:
		assert(false);
		break;
	}

	++seed;

	return S_OK;
}

HRESULT GriffonDoll::insertSquadTacDoll(SINT squadID, SINT allID)
{
	if ((dollIter = mAllDoll.find(allID)) != mAllDoll.end() 
		&& IOPsquad->callSquad(squadID) != nullptr)
	{
		if (IOPsquad->insertSquadMember(squadID, dollIter->second))
		{
			//	분대장이 비어있으면 0번째 친구를 분대장으로 임명한다.
			appointLeaderTacDoll(squadID, IOPsquad->callSquad(squadID)->squadLeaderID);
			return S_OK;
		}
	}
	return E_FAIL;
}

HRESULT GriffonDoll::exitSquadTacDoll(SINT squadID, SINT memID)
{
	if (IOPsquad->getValidSquad(squadID)) {
		IOPsquad->emitSquadMember(squadID, memID);
		appointLeaderTacDoll(squadID, IOPsquad->callSquad(squadID)->squadLeaderID);
	}
	return S_OK;
}

HRESULT GriffonDoll::appointLeaderTacDoll(SINT squadID, SINT memID)
{
	IOPsquad->callSquad(squadID)->squadLeaderID = memID;
	IOPsquad->callSquad(squadID)->squadLeader = IOPsquad->callSquadMember(squadID, memID);
	return S_OK;
}

HRESULT GriffonDoll::createIOPtherTeam(GRFDOLL_NAME name)
{
	static SINT seed = 0;

	switch (name)
	{
	case GRF_BGM71:
	{
		Bgm71* _new = new Bgm71;
		_new->getID()->All_ID = seed;
		mAllThermalTeam.insert(make_pair(_new->getID()->All_ID, _new));
	}
	break;

	default:
		assert(false);
		break;
	}

	++seed;

	return S_OK;
}

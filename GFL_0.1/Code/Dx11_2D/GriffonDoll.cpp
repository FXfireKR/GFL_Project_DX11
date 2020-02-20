#include "stdafx.h"
#include "GriffonDoll.h"

GriffonDoll::GriffonDoll()
{
}

GriffonDoll::~GriffonDoll()
{
}

HRESULT GriffonDoll::init()
{
	return S_OK;
}

void GriffonDoll::release()
{
	for (auto& it : mAlltacDoll)
	{
		it.second->release();
		SAFE_DEL(it.second);
	}

	mAlltacDoll.clear();
}

HRESULT GriffonDoll::Create_IOPtacDoll(GRFDOLL_NAME name)
{
	static SINT seed = 0;

	switch (name)
	{
	case GRF_NONE:
		break;

	case GRF_AK12:
	{
		AK12* _new = new AK12;
		_new->getID()->All_ID = seed;
		mAlltacDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
		break;

	case GRF_9A91:
	{
		GuA91* _new = new GuA91;
		_new->getID()->All_ID = seed;
		mAlltacDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
		break;

	case GRF_PKP:
	{
		pkp* _new = new pkp;
		_new->getID()->All_ID = seed;
		mAlltacDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
		break;

	case GRF_AR15:
	{
		AR15* _new = new AR15;
		_new->getID()->All_ID = seed;
		mAlltacDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
		break;

	case GRF_M4SOP:
	{
		M4SopMod* _new = new M4SopMod;
		_new->getID()->All_ID = seed;
		mAlltacDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
	break;

	case GRF_NTW20:
	{
		Ntw20* _new = new Ntw20;
		_new->getID()->All_ID = seed;
		mAlltacDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
	break;

	case GRF_AN94:
	{
		AN94* _new = new AN94;
		_new->getID()->All_ID = seed;
		mAlltacDoll.insert(make_pair(_new->getID()->All_ID, _new));
	}
	break;

	default:
		break;
	}

	++seed;

	return S_OK;
}

HRESULT GriffonDoll::InsertSquad_IOPtacDoll(SINT squadId, SINT allid)
{
	if (mAlltacDoll.count(allid) && IOPsquad.Call_Squad(squadId) != nullptr)
	{
		auto it = mAlltacDoll.find(allid);

		IOPsquad.Insert_SquadMember(squadId, it->second);

		//분대장이 비어있으면 0번째 친구를 분대장으로 임명한다.
		AppointLeader_IOPsquad(squadId, IOPsquad.Call_Squad(squadId)->squadLeaderID);
		return S_OK;
	}

	return E_FAIL;
}

HRESULT GriffonDoll::ExitSquad_IOPtacDoll(SINT squadId, SINT memID)
{
	IOPsquad.Emit_SquadMember(squadId, memID);
	AppointLeader_IOPsquad(squadId, IOPsquad.Call_Squad(squadId)->squadLeaderID);

	return S_OK;
}

HRESULT GriffonDoll::AppointLeader_IOPsquad(SINT sid, SINT mid)
{
	IOPsquad.Call_Squad(sid)->squadLeaderID = mid;
	IOPsquad.Call_Squad(sid)->squadLeader = IOPsquad.Call_SquadMember(sid, mid);
	return S_OK;
}

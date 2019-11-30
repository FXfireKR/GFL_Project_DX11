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


	/*for (auto& it : mAllsuportDoll)
	{
		it.second->release();
		SAFE_DELETE(it.second);
	}

	mAllsuportDoll.clear();*/
}

HRESULT GriffonDoll::Create_IOPtacDoll(IOPDOLL_NAME name)
{
	switch (name)
	{
	case IOP_M4A1MOD:
	{
		//M4A1MOD* buf = new M4A1MOD;
		//mAlltacDoll.insert(make_pair(buf->getID()->All_ID, buf));
	}
	break;

	case IOP_AR15MOD:
	{
		//AR15MOD* buf = new AR15MOD;
		//mAlltacDoll.insert(make_pair(buf->getID()->All_ID, buf));
	}
	break;

	case IOP_M1891MOD:
	{
		//M1891MOD* buf = new M1891MOD;
		//mAlltacDoll.insert(make_pair(buf->getID()->All_ID, buf));
	}
	break;

	case IOP_NTW20:
	{
		NTW20* buf = new NTW20;
		mAlltacDoll.insert(make_pair(buf->getID()->All_ID, buf));
	}
	break;

	case IOP_AA12:
	{
		//AA12* buf = new AA12;
		//mAlltacDoll.insert(make_pair(buf->getID()->All_ID, buf));
	}
	break;

	case IOP_UMP45MOD:
	{
		//UMP45MOD* buf = new UMP45MOD;
		//mAlltacDoll.insert(make_pair(buf->getID()->All_ID, buf));
	}
	break;

	case IOP_PKP:
	{
		//PKP* buf = new PKP;
		//mAlltacDoll.insert(make_pair(buf->getID()->All_ID, buf));
	}
	break;

	case IOP_AK12:
	{
		//AK12* buf = new AK12;
		//mAlltacDoll.insert(make_pair(buf->getID()->All_ID, buf));
	}
	break;

	case IOP_THUNDER:
	{
		//THUNDER50* buf = new THUNDER50;
		//mAlltacDoll.insert(make_pair(buf->getID()->All_ID, buf));
	}
	break;

	case IOP_FNFAL:
	{
		FNFAL* buf = new FNFAL;
		mAlltacDoll.insert(make_pair(buf->getID()->All_ID, buf));
	}
	break;

	case IOP_ARCHITECT:
	{
		ARCHITECT* buf = new ARCHITECT;
		mAlltacDoll.insert(make_pair(buf->getID()->All_ID, buf));
	}
	break;

	}

	return S_OK;
}

HRESULT GriffonDoll::InsertSquad_IOPtacDoll(UINT squadId, UINT allid)
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

HRESULT GriffonDoll::ExitSquad_IOPtacDoll(UINT squadId, UINT memID)
{
	IOPsquad.Emit_SquadMember(squadId, memID);
	AppointLeader_IOPsquad(squadId, IOPsquad.Call_Squad(squadId)->squadLeaderID);

	return S_OK;
}

HRESULT GriffonDoll::AppointLeader_IOPsquad(UINT sid, UINT mid)
{
	IOPsquad.Call_Squad(sid)->squadLeaderID = mid;
	IOPsquad.Call_Squad(sid)->squadLeader = IOPsquad.Call_SquadMember(sid, mid);
	return S_OK;
}

//HRESULT GriffonDoll::Create_IOPsptDoll(IOP_SUPPORTDOLL_TYPE type)
//{
//	if (!mAllsuportDoll.count(type))
//	{
//		switch (type)
//		{
//		case IST_BGM_71:
//		{
//			BGM_71* temp = new BGM_71;
//			temp->init();
//			mAllsuportDoll.insert(make_pair(type, temp));
//		}
//		break;
//
//		case IST_AGS_30:
//			break;
//
//		case IST_2B14:
//			break;
//
//		case IST_M2:
//			break;
//
//		case IST_AT4:
//			break;
//		}
//
//		return S_OK;
//	}
//
//	return E_FAIL;
//}

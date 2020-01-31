#include "stdafx.h"
#include "PlayerData.h"

PlayerData::PlayerData()
	:curFocusDol(nullptr), cameraFocus(false)
{
}

PlayerData::~PlayerData()
{
	this->release();
}

HRESULT PlayerData::init()
{
	tacDoll = new GriffonDoll;
	tacDoll->init();

	////부착물 최대 40개 보유제한
	//for (int i = 0; i < 40; ++i)
	//{
	//	EquipBase temp;
	//	mPlayerEquip.insert(make_pair(temp.getWeaponID(), temp));
	//}

	//EquipIter = mPlayerEquip.begin();


	//전체 할당
	//tacDoll->Create_IOPtacDoll(IOP_AA12);
	//tacDoll->Create_IOPtacDoll(IOP_AK12);
	//tacDoll->Create_IOPtacDoll(IOP_THUNDER);
	//tacDoll->Create_IOPtacDoll(IOP_UMP45MOD);

	//tacDoll->Create_IOPtacDoll(IOP_AR15MOD);
	//tacDoll->Create_IOPtacDoll(IOP_PKP);
	//tacDoll->Create_IOPtacDoll(IOP_M4A1MOD);

	//tacDoll->Create_IOPsptDoll(IST_BGM_71);

	playerHighLight = maxPlayerHighLight = 0.0f;
	PlayerSlow = false;

	//현재 정보 저장
	currentSquad = 1;
	//curntSuportDoll = IST_BGM_71;

	sPos.x = sPos.y = 0;
	ePos.x = ePos.y = 0;

	multiSelectMod = false;
	RemUnit_Toggle = false;

	return S_OK;
}

void PlayerData::release()
{
	if (tacDoll != nullptr) {
		tacDoll->release();
		SAFE_DEL(tacDoll);
	}
}

void PlayerData::update()
{
	AutoSetting_CameraFocus();
	Change_FocusTacDoll();

	//멀티 선택일경우
	if (vMultiSelect.size() > 0)
	{
		mPos.x = 0.0f;
		mPos.y = 0.0f;

		for (auto& it : vMultiSelect)
		{
			mPos.x += this->getIOPdoll_crntSquad(it)->getCharacterPos().x;
			mPos.y += this->getIOPdoll_crntSquad(it)->getCharacterPos().y;
		}

		mPos.x /= vMultiSelect.size();
		mPos.y /= vMultiSelect.size();

	}

	//화력소대가 안불렸다면
	/*if (tacDoll->getSupportDoll(curntSuportDoll) != nullptr)
	{
		if (!tacDoll->getSupportDoll(curntSuportDoll)->isCalling())
		{
			for (auto& it : tacDoll->getSquad(currentSquad)->mSquad)
				it.second->update();
		}
	}
	else*/
	if (PlayerSlow)
	{
		if (playerHighLight < DELTA)
		{
			for (auto& it : tacDoll->getSquad(currentSquad)->mSquad)
				it.second->update();
			playerHighLight = maxPlayerHighLight;
		}
		else
			playerHighLight -= DELTA;
	}

	else
	{
		if (playerHighLight > 0.0f)
			playerHighLight = 0;

		for (auto& it : tacDoll->getSquad(currentSquad)->mSquad)
			it.second->update();
	}

	//if (tacDoll->getSupportDoll(curntSuportDoll) != nullptr)
	//	tacDoll->getSupportDoll(curntSuportDoll)->update();
}

void PlayerData::render()
{
	if (multiSelectMod)
	{
		D2DX->renderRect(sPos.x - CameraPositionX, sPos.y + CameraPositionY, 
			(ePos.x - sPos.x), (ePos.y - sPos.y), ColorF(0, 255, 0));
	}
}

void PlayerData::Change_FocusTacDoll()
{
	//5~0 번 까지 유닛 지정 선택
	if (KEYMANAGER->isKeyDown(VK_LCONTROL))
		RemUnit_Toggle = true;

	//NumberPad Action
	{
		if (KEYMANAGER->isKeyDown('1'))
			RemUnit_Toggle ? SaveRemUnit(1) : LoadRemUnit(1);

		else if (KEYMANAGER->isKeyDown('2'))
			RemUnit_Toggle ? SaveRemUnit(2) : LoadRemUnit(2);

		else if (KEYMANAGER->isKeyDown('3'))
			RemUnit_Toggle ? SaveRemUnit(3) : LoadRemUnit(3);

		else if (KEYMANAGER->isKeyDown('4'))
			RemUnit_Toggle ? SaveRemUnit(4) : LoadRemUnit(4);

		else if (KEYMANAGER->isKeyDown('5'))
			RemUnit_Toggle ? SaveRemUnit(5) : LoadRemUnit(5);

		else if (KEYMANAGER->isKeyDown('6'))
			RemUnit_Toggle ? SaveRemUnit(6) : LoadRemUnit(6);

		else if (KEYMANAGER->isKeyDown('7'))
			RemUnit_Toggle ? SaveRemUnit(7) : LoadRemUnit(7);

		else if (KEYMANAGER->isKeyDown('8'))
			RemUnit_Toggle ? SaveRemUnit(8) : LoadRemUnit(8);

		else if (KEYMANAGER->isKeyDown('9'))
			RemUnit_Toggle ? SaveRemUnit(9) : LoadRemUnit(9);

		else if (KEYMANAGER->isKeyDown('0'))
			RemUnit_Toggle ? SaveRemUnit(0) : LoadRemUnit(0);
	}

	if (KEYMANAGER->isKeyUp(VK_LCONTROL))
		RemUnit_Toggle = false;

	//포커싱
	if (KEYMANAGER->isKeyUp(VK_TAB))
	{
		if (vMultiSelect.size() < 1)
		{
			if (curFocusDol != nullptr)
				curFocusDol->revFocus();

			//if (curFocusDol != nullptr)
			//	curFocusDol->revSelect();

			curFocusDol = nullptr;
			cameraFocus = false;
		}

		else
		{
			for (auto& it : vMultiSelect)
				this->getIOPdoll_crntSquad(it)->revSelect();
			vMultiSelect.clear();
		}
	}


	//좌클릭시
	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		////화력지원소대 포커싱일경우
		//if (getIOPsptDoll_crntFocus() != nullptr)
		//{
		//	//화력지원부대가 호출되어있지는 않는다면
		//	if (!getIOPsptDoll_crntFocus()->isCalling())
		//	{
		//		//시작지점을 정한다.

		if (vMultiSelect.size() > 0)
		{
			for (auto& it : vMultiSelect)
				this->getIOPdoll_crntSquad(it)->revSelect();
			vMultiSelect.clear();
		}

		else
		{
			if (curFocusDol != 0)
				curFocusDol->revFocus();

			//if (focusTacdolID != 0)
			//	curFocusDol->revSelect();

			curFocusDol = nullptr;
			cameraFocus = false;
		}

		sPos.x = g_ptMouse.x + CameraPositionX;
		sPos.y = g_ptMouse.y - CameraPositionY;

		multiSelectMod = true;
		//	}

		//	//폭탄 투하위치 지정
		//	else
		//		getIOPsptDoll_crntFocus()->setTargetPosition();
		//}

		//else
		{
			//if (getIOPdoll_crntSquadFocus() != nullptr)
			//	getIOPdoll_crntSquadFocus()->SetMovePoint();

		}
	}


	//다중 대상 지정중
	if (KEYMANAGER->isKeyStayDown(VK_LBUTTON))
	{
		ePos.x = g_ptMouse.x + CameraPositionX;
		ePos.y = g_ptMouse.y - CameraPositionY;
	}


	//다중 대상 지정 완료
	if (KEYMANAGER->isKeyUp(VK_LBUTTON))
	{
		if (multiSelectMod)
		{
			ePos.x = static_cast<LONG>(g_ptMouse.x + CameraPositionX);
			ePos.y = static_cast<LONG>(g_ptMouse.y - CameraPositionY);

			if (sPos.x > ePos.x)
			{
				int temp = sPos.x;
				sPos.x = ePos.x;
				ePos.x = temp;
			}

			if (sPos.y > ePos.y)
			{
				int temp = sPos.y;
				sPos.y = ePos.y;
				ePos.y = temp;
			}

			RECT rc = RectMake(sPos.x, sPos.y, ePos.x - sPos.x, ePos.y - sPos.y);

			for (auto& it : tacDoll->getSquad(currentSquad)->mSquad)
			{
				if (!it.second->getAlive())continue;

				POINT cp;
				cp.x = static_cast<LONG>(it.second->getCharacterPos().x);
				cp.y = static_cast<LONG>(it.second->getCharacterPos().y);

				if (PtInRect(&rc, cp))
				{
					vMultiSelect.push_back(it.first);
					it.second->revSelect();
				}
			}

			//단일 인형만을 선택했을경우
			if (vMultiSelect.size() == 1)
			{
				getIOPdoll_crntSquad(*vMultiSelect.begin())->revSelect();
				Change_FocusTacDoll_Func(*vMultiSelect.begin());
				vMultiSelect.clear();
			}

			mPos.x = mPos.y = 0.0f;

			multiSelectMod = false;
		}
	}


	//이동, 공격대상 변경
	if (KEYMANAGER->isKeyUp(VK_RBUTTON))
	{
		if (!Targetting_Other())
		{
			if (vMultiSelect.size() < 1)
			{
				//포커싱된 인형이 존재한다면
				if (getIOPdoll_crntSquadFocus() != nullptr)
					getIOPdoll_crntSquadFocus()->SetMovePoint();
			}

			//다중 선택일 경우에
			else
			{
				for (auto& it : vMultiSelect)
				{
					auto& me = (*getIOPdoll_crntSquad(it));

					trgPos.x = g_ptMouse.x - (mPos.x - me.getCharacterPos().x) + CameraPositionX;
					trgPos.y = g_ptMouse.y - (mPos.y - me.getCharacterPos().y) - CameraPositionY;

					me.MovePoint(trgPos.x, trgPos.y);
				}
			}
		}
	}


	//정지
	if (KEYMANAGER->isKeyUp('S'))
	{
		if (vMultiSelect.size() < 1) {
			if (this->getIOPdoll_crntSquadFocus() != nullptr)
				if (this->getIOPdoll_crntSquadFocus()->getAlive())
					this->getIOPdoll_crntSquadFocus()->StopMoving();
		}

		else
		{
			for (auto& it : vMultiSelect)
			{
				auto& me = (*getIOPdoll_crntSquad(it));
				me.StopMoving();
			}
		}
	}


	//스킬 사용
	if (KEYMANAGER->isKeyUp('E'))
	{
		if (vMultiSelect.size() < 1)
			if (this->getIOPdoll_crntSquadFocus() != nullptr)
				if (this->getIOPdoll_crntSquadFocus()->getAlive())
					this->getIOPdoll_crntSquadFocus()->Use_ActiveSkill();
	}
}

void PlayerData::Change_FocusTacDoll_Func(int memID)
{
	TaticDoll* calledDoll;

	if ((calledDoll = this->getIOPdoll_crntSquad(memID)) != nullptr)
	{
		//	1회 누를시 선택, 2번누를시 포커스

		//	현재 포커싱중인 전술인형과 다른경우
		if (curFocusDol != calledDoll)
		{
			if (curFocusDol != nullptr)
				curFocusDol->revFocus();

			cameraFocus = false;
			curFocusDol = calledDoll;

			//Select FocusTacDoll
			curFocusDol->revSelect();
		}

		//	현재 포커싱중인 전술인형과 동일한 경우
		else
			cameraFocus = cameraFocus ? false : true;
	}
}

void PlayerData::SaveRemUnit(int padID)
{
	//단일선택인경우
	if (vMultiSelect.size() < 1)
	{
		//선택이 되어있을경우
		if (this->getIOPdoll_crntSquadFocus() != nullptr)
		{
			vector<UINT> temp;
			temp.push_back(getIOPdoll_crntSquadFocus()->getID()->SquadMem_ID);

			if (!mRemUnit.count(padID))
				mRemUnit.insert(make_pair(padID, temp));

			else
				mRemUnit.find(padID)->second.swap(temp);
		}
	}

	//다중선택일경우
	else
	{
		vector<UINT> temp;
		temp = vMultiSelect;

		if (!mRemUnit.count(padID))
			mRemUnit.insert(make_pair(padID, temp));

		else
			mRemUnit.find(padID)->second.swap(temp);
	}
}

void PlayerData::LoadRemUnit(int padID)
{
	//해당 부분이 존재하고있을 경우
	if (mRemUnit.count(padID))
	{
		auto& it = *(mRemUnit.find(padID));

		//단일 유닛인 경우 -> 해당유닛으로 포커싱 변동
		if (it.second.size() == 1)
		{
			if (vMultiSelect.size() > 0)
			{
				for (auto& it : vMultiSelect)
					this->getIOPdoll_crntSquad(it)->revSelect();
				vMultiSelect.clear();
			}

			Change_FocusTacDoll_Func(*(it.second.begin()));
		}

		//다중 유닛인경우
		else if (it.second.size() > 1)
		{
			if (vMultiSelect.size() < 1)
			{
				if (curFocusDol != nullptr)
					curFocusDol->revFocus();

				//if (focusTacdolID != 0)
				//	curFocusDol->revSelect();

				curFocusDol = nullptr;
				cameraFocus = false;
			}

			else
			{
				for (auto& it : vMultiSelect)
					this->getIOPdoll_crntSquad(it)->revSelect();
				vMultiSelect.clear();
			}

			vMultiSelect = it.second;

			for (auto& it2 : vMultiSelect)
				getIOPdoll_crntSquad(it2)->revSelect();

		}

	}
}

void PlayerData::AutoSetting_CameraFocus()
{
	if (cameraFocus)
	{
		CameraPositionX = curFocusDol->getCharacterPos().x - (WINSIZEX * 0.5f);
		CameraPositionY = (curFocusDol->getCharacterPos().y - (WINSIZEY * 0.5f))* -1.0f;
	}

	//	Limit Camera
	BTLMAP->update();
}

bool PlayerData::Targetting_Other()
{
	//if (getIOPdoll_crntSquadFocus() != nullptr)
	//{
	//	for (auto& it : BDATA->getEnemySquad(BDATA->getEngageSquadID())->mSquad)
	//	{
	//		//조건
	//		if (!PtInRect(&(it.second->getCharacterRect()), _ptMouse))continue;
	//		getIOPdoll_crntSquadFocus()->Set_Targetting_Other(it.first);

	//		return true;
	//	}

	//	return false;
	//}
	return false;
}

void PlayerData::test_create()
{
	tacDoll->Create_IOPtacDoll(GRF_AR15);
	tacDoll->Create_IOPtacDoll(GRF_AK12);
	
	
	//tacDoll->Create_IOPtacDoll(GRF_PKP);

}

void PlayerData::test_setting()
{
	//tacDoll->getAllTacDoll().at(0)->init();
	tacDoll->InsertSquad_IOPtacDoll(1, 0);

	//tacDoll->getAllTacDoll().at(1)->init();
	tacDoll->InsertSquad_IOPtacDoll(1, 1);
}

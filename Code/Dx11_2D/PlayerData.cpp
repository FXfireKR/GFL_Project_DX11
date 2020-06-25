#include "stdafx.h"
#include "PlayerData.h"

PlayerData::PlayerData()
	: curFocusDol(nullptr), cameraFocus(false), tacDoll(nullptr)
{
}

PlayerData::~PlayerData()
{
	this->release();
}

void PlayerData::init()
{
	tacDoll = new GriffonDoll;
	tacDoll->init();

	//	부착물 로드
	for (auto& it : EQUIP->getEquipInfoBase())
		mPlayerEquip.insert(make_pair(it.second->getString().name, EquipContainer(it.second)));

	EquipIter = mPlayerEquip.begin();

	//전체 할당

	playerHighLight = maxPlayerHighLight = 0.0f;
	PlayerSlow = false;

	//현재 정보 저장
	currentSquad = 1;
	//curntSuportDoll = IST_BGM_71;

	sPos.x = sPos.y = 0;
	ePos.x = ePos.y = 0;

	multiSelectMod = false;
	RemUnit_Toggle = false;
	lockTechKey = false;
}

void PlayerData::reset()
{
	cameraFocus = false;
	curFocusDol = nullptr;

	sPos.x = sPos.y = 0;
	ePos.x = ePos.y = 0;

	multiSelectMod = false;
	RemUnit_Toggle = false;

	mRemUnit.clear();
}

void PlayerData::release()
{
	SAFE_DELETE(tacDoll);
}

void PlayerData::update()
{
	autoSettingCameraFocus();
	changeFocusDoll();

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
}

void PlayerData::render()
{
	if (multiSelectMod) {
		D2D->renderRect(static_cast<float>(sPos.x) - CameraPositionX, static_cast<float>(sPos.y) + CameraPositionY,
			static_cast<float>(ePos.x - sPos.x), static_cast<float>(ePos.y - sPos.y), ColorF(0, 255, 0));
	}
}

void PlayerData::changeFocusDoll()
{
	if (lockTechKey == false) {
		//	0 ~ 9 번 까지 유닛 지정 선택
		if (KEYMANAGER->isKeyDown(VK_LCONTROL))
			RemUnit_Toggle = true;

		//	NumberPad Action
		{
			if (KEYMANAGER->isKeyDown('1'))
				RemUnit_Toggle ? saveRemUnit(1) : loadRemUnit(1);

			else if (KEYMANAGER->isKeyDown('2'))
				RemUnit_Toggle ? saveRemUnit(2) : loadRemUnit(2);

			else if (KEYMANAGER->isKeyDown('3'))
				RemUnit_Toggle ? saveRemUnit(3) : loadRemUnit(3);

			else if (KEYMANAGER->isKeyDown('4'))
				RemUnit_Toggle ? saveRemUnit(4) : loadRemUnit(4);

			else if (KEYMANAGER->isKeyDown('5'))
				RemUnit_Toggle ? saveRemUnit(5) : loadRemUnit(5);

			else if (KEYMANAGER->isKeyDown('6'))
				RemUnit_Toggle ? saveRemUnit(6) : loadRemUnit(6);

			else if (KEYMANAGER->isKeyDown('7'))
				RemUnit_Toggle ? saveRemUnit(7) : loadRemUnit(7);

			else if (KEYMANAGER->isKeyDown('8'))
				RemUnit_Toggle ? saveRemUnit(8) : loadRemUnit(8);

			else if (KEYMANAGER->isKeyDown('9'))
				RemUnit_Toggle ? saveRemUnit(9) : loadRemUnit(9);

			else if (KEYMANAGER->isKeyDown('0'))
				RemUnit_Toggle ? saveRemUnit(0) : loadRemUnit(0);
		}

		//	선택 종료
		if (KEYMANAGER->isKeyUp(VK_LCONTROL))
			RemUnit_Toggle = false;

		//	Change Focusing
		if (KEYMANAGER->isKeyUp(VK_TAB))
		{
			if (vMultiSelect.size() < 1)
			{
				if (curFocusDol != nullptr) {
					if (curFocusDol->isFocus())
						curFocusDol->revFocus();

					if (curFocusDol->isSelect())
						curFocusDol->revSelect();

					curFocusDol = nullptr;
					cameraFocus = false;
				}
			}

			else
			{
				for (auto& it : vMultiSelect)
					this->getIOPdoll_crntSquad(it)->revSelect();
				vMultiSelect.clear();
			}
		}
	}

	//	좌클릭시
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
			if (curFocusDol != nullptr)
			{
				if (curFocusDol->isFocus())
					curFocusDol->revFocus();

				if (curFocusDol->isSelect())
					curFocusDol->revSelect();
			}

			curFocusDol = nullptr;
			cameraFocus = false;
		}

		sPos.x = g_ptMouse.x + static_cast<LONG>(CameraPositionX);
		sPos.y = g_ptMouse.y - static_cast<LONG>(CameraPositionY);

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

	//	다중 대상 지정중
	if (KEYMANAGER->isKeyStayDown(VK_LBUTTON))
	{
		ePos.x = g_ptMouse.x + static_cast<LONG>(CameraPositionX);
		ePos.y = g_ptMouse.y - static_cast<LONG>(CameraPositionY);
	}

	//	다중 대상 지정 완료
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

			for (auto& it : tacDoll->getSquad(currentSquad)->squadMember)
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
				changeFocusDoll(*vMultiSelect.begin());
				vMultiSelect.clear();
			}

			mPos.x = mPos.y = 0.0f;

			multiSelectMod = false;
		}
	}


	//	이동, 공격대상 변경
	if (KEYMANAGER->isKeyUp(VK_RBUTTON))
	{
		if (!targettingOther())
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


	//	정지
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


	//	스킬 사용
	if (KEYMANAGER->isKeyUp('E'))
	{
		if (curFocusDol != nullptr)
			if (curFocusDol->getAlive())
				curFocusDol->Use_ActiveSkill();

		/*if (vMultiSelect.size() < 1)
			if (this->getIOPdoll_crntSquadFocus() != nullptr)
				if (this->getIOPdoll_crntSquadFocus()->getAlive())
					this->getIOPdoll_crntSquadFocus()->Use_ActiveSkill();*/
	}
}

void PlayerData::changeFocusDoll(SINT memID)
{
	BaseTaticDoll* calledDoll;

	if ((calledDoll = this->getIOPdoll_crntSquad(memID)) != nullptr)
	{
		//	1회 누를시 선택, 2번누를시 포커스

		//	현재 포커싱중인 전술인형과 다른경우
		if (curFocusDol != calledDoll)
		{
			if (curFocusDol != nullptr) {

				if(curFocusDol->isFocus())
					curFocusDol->revFocus();
				if (curFocusDol->isSelect())
					curFocusDol->revSelect();
			}

			cameraFocus = false;
			curFocusDol = calledDoll;

			//Select FocusTacDoll
			if (!curFocusDol->isSelect())
				curFocusDol->revSelect();
		}

		//	현재 포커싱중인 전술인형과 동일한 경우
		else
			cameraFocus = cameraFocus ? false : true;
	}
}

void PlayerData::saveRemUnit(int padID)
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

void PlayerData::loadRemUnit(int padID)
{
	//해당 부분이 존재하고있을 경우
	if ((miRemUnit = mRemUnit.find(padID)) != mRemUnit.end())
	{
		//	단일 유닛인 경우 -> 해당유닛으로 포커싱 변동
		if (miRemUnit->second.size() == 1)
		{
			if (vMultiSelect.size() > 0)
			{
				for (auto& it : vMultiSelect) {
					if (this->getIOPdoll_crntSquad(it)->isSelect())
						this->getIOPdoll_crntSquad(it)->revSelect();
				}
				vMultiSelect.clear();
			}

			changeFocusDoll(*(miRemUnit->second.begin()));
		}

		//	다중 유닛인경우
		else if (miRemUnit->second.size() > 1)
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
				for (auto& it : vMultiSelect) {
					if (this->getIOPdoll_crntSquad(it)->isSelect())
						this->getIOPdoll_crntSquad(it)->revSelect();
				}
				vMultiSelect.clear();
			}

			vMultiSelect = miRemUnit->second;

			for (auto& it2 : vMultiSelect) {
				if (!getIOPdoll_crntSquad(it2)->isSelect())
					getIOPdoll_crntSquad(it2)->revSelect();
			}
		}

	}
}

void PlayerData::autoSettingCameraFocus()
{
	if (cameraFocus)
	{
		CameraPositionX = curFocusDol->getCharacterPos().x - (WINSIZEX * 0.5f);
		CameraPositionY = (curFocusDol->getCharacterPos().y - (WINSIZEY * 0.5f))* -1.0f;
	}

	//	Limit Camera
	//BTLMAP->update();
}

bool PlayerData::targettingOther()
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

bool PlayerData::isAliance(TATICDOLL_ALIANCE_TYPE _type)
{
	for (auto& it : vAlianceList)
		if (it == _type)
			return true;
	return false;
}

void PlayerData::insertDollToSquad(SINT allID, SINT squadID)
{
	//	전체 멤버가 있다면
	if (tacDoll->getAllDolls().count(allID))
		tacDoll->insertSquadTacDoll(squadID, allID);
}

void PlayerData::deleteDollToSquad(SINT squadID, SINT memID)
{
	//	해당 스쿼드에 멤버가 존재한다면
	if (tacDoll->getSquad(squadID)->squadMember.count(memID))
		tacDoll->exitSquadTacDoll(squadID, memID);
}

void PlayerData::testCreate()
{
	tacDoll->createIOPtacDoll(GRF_NTW20);
	tacDoll->createIOPtacDoll(GRF_KSG);
	//tacDoll->createIOPtacDoll(GRF_9A91);
	//tacDoll->createIOPtacDoll(GRF_AR15);
	tacDoll->createIOPtacDoll(GRF_PKP);
	tacDoll->createIOPtacDoll(GRF_M4SOP);


	//tacDoll->createIOPtherTeam(GRF_BGM71);
}

void PlayerData::testSetting()
{
}

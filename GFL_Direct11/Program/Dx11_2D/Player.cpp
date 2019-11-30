#include "stdafx.h"
#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

HRESULT Player::init()
{
	tacDoll = new GriffonDoll;
	tacDoll->init();

	//������ �ִ� 40�� ��������
	//for (int i = 0; i < 40; ++i)
	//{
	//	EquipBase temp;
	//	mPlayerEquip.insert(make_pair(temp.getWeaponID(), temp));
	//}

	//EquipIter = mPlayerEquip.begin();

	//EquipIter->second = EQUIP->getEquipInfo("EOT 516");
	//++EquipIter;

	//EquipIter->second = EQUIP->getEquipInfo("16LAB_MARS");
	//++EquipIter;

	//EquipIter->second = EQUIP->getEquipInfo("VFL6-24X56");
	//++EquipIter;

	//EquipIter->second = EQUIP->getEquipInfo("VFL16-48X80");
	//++EquipIter;

	//EquipIter->second = EQUIP->getEquipInfo("APCR���ź");
	//++EquipIter;

	//EquipIter->second = EQUIP->getEquipInfo("Mk169ö��ź");
	//++EquipIter;

	//EquipIter->second = EQUIP->getEquipInfo("JHP���ź");
	//++EquipIter;

	//EquipIter->second = EQUIP->getEquipInfo("Mk169��ȭö��ź");
	//++EquipIter;

	//EquipIter->second = EQUIP->getEquipInfo("AMP_COMPM4");
	//++EquipIter;

	//EquipIter->second = EQUIP->getEquipInfo("16LAB�����ѿ�");
	//++EquipIter;

	//EquipIter->second = EQUIP->getEquipInfo("AC16������");

	//OwnEquipNumber = 11;



	//��ü �Ҵ�
	//tacDoll->Create_IOPtacDoll(IOP_AA12);
	//tacDoll->Create_IOPtacDoll(IOP_AK12);
	//tacDoll->Create_IOPtacDoll(IOP_THUNDER);
	//tacDoll->Create_IOPtacDoll(IOP_UMP45MOD);

	//tacDoll->Create_IOPtacDoll(IOP_AR15MOD);
	//tacDoll->Create_IOPtacDoll(IOP_PKP);
	//tacDoll->Create_IOPtacDoll(IOP_M4A1MOD);

	//tacDoll->Create_IOPsptDoll(IST_BGM_71);

	//���� ���� ����
	currentSquad = 1;
	focusTacdolID = 0;
	cFocusTacDollID = 0;
	//curntSuportDoll = IST_BGM_71;

	sPos.x = sPos.y = 0;
	ePos.x = ePos.y = 0;

	multiSelectMod = false;
	RemUnit_Toggle = false;

	return S_OK;
}

void Player::release()
{
	tacDoll->release();
	SAFE_DEL(tacDoll);
}

void Player::update()
{
	Change_FocusTacDoll();

	//��Ƽ �����ϰ��
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

	//ȭ�¼Ҵ밡 �Ⱥҷȴٸ�
	/*if (tacDoll->getSupportDoll(curntSuportDoll) != nullptr)
	{
		if (!tacDoll->getSupportDoll(curntSuportDoll)->isCalling())
		{
			for (auto& it : tacDoll->getSquad(currentSquad)->mSquad)
				it.second->update();
		}
	}
	else*/
		for (auto& it : tacDoll->getSquad(currentSquad)->mSquad)
			it.second->update();


	//if (tacDoll->getSupportDoll(curntSuportDoll) != nullptr)
	//	tacDoll->getSupportDoll(curntSuportDoll)->update();
}

void Player::render()
{
	if (multiSelectMod)
	{
		DRAWMANAGER->renderRect(sPos.x - BDATA->getVirtualPos().x, sPos.y - BDATA->getVirtualPos().y, (ePos.x - sPos.x), (ePos.y - sPos.y), ColorF(0, 255, 0));
	}

	//char str[256];
	//sprintf(str, "Middle Point : %.2f  %.2f", mPos.x, mPos.y);
	//TEXT->Create_TextField("TEST", L"����ü", str, 20, DWRITE_FONT_WEIGHT_BOLD);
	//TEXT->TextRender("TEST", 0.0F, 150.0F, { 255.0F,0.0F,0.0F });

	//sprintf(str, "Target Point : %.2f  %.2f", trgPos.x, trgPos.y);
	//TEXT->Create_TextField("TEST", L"����ü", str, 20, DWRITE_FONT_WEIGHT_BOLD);
	//TEXT->TextRender("TEST", 0.0F, 180.0F, { 255.0F,0.0F,0.0F });

	//if (tacDoll->getSupportDoll(curntSuportDoll) != nullptr)
	//	tacDoll->getSupportDoll(curntSuportDoll)->render();

	//for (auto& it : tacDoll->getSquad(currentSquad)->mSquad)
	//	it.second->render();
}

void Player::Change_FocusTacDoll()
{
	//5~0 �� ���� ���� ���� ����
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

	//��Ŀ��
	if (KEYMANAGER->isKeyUp(VK_TAB))
	{
		if (vMultiSelect.size() < 1)
		{
			if (cFocusTacDollID != 0)
				this->getIOPdoll_crntSquad(cFocusTacDollID)->revFocus();

			if (focusTacdolID != 0)
				this->getIOPdoll_crntSquad(focusTacdolID)->revSelect();

			focusTacdolID = 0;
			cFocusTacDollID = 0;
		}

		else
		{
			for (auto& it : vMultiSelect)
				this->getIOPdoll_crntSquad(it)->revSelect();
			vMultiSelect.clear();
		}
	}


	//��Ŭ����
	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		////ȭ�������Ҵ� ��Ŀ���ϰ��
		//if (getIOPsptDoll_crntFocus() != nullptr)
		//{
		//	//ȭ�������δ밡 ȣ��Ǿ������� �ʴ´ٸ�
		//	if (!getIOPsptDoll_crntFocus()->isCalling())
		//	{
		//		//���������� ���Ѵ�.
				if (vMultiSelect.size() > 0)
				{
					for (auto& it : vMultiSelect)
						this->getIOPdoll_crntSquad(it)->revSelect();
					vMultiSelect.clear();
				}

				else
				{
					if (cFocusTacDollID != 0)
						this->getIOPdoll_crntSquad(cFocusTacDollID)->revFocus();

					if (focusTacdolID != 0)
						this->getIOPdoll_crntSquad(focusTacdolID)->revSelect();

					focusTacdolID = 0;
					cFocusTacDollID = 0;
				}

				sPos.x = _ptMouse.x + BDATA->getVirtualPos().x;
				sPos.y = _ptMouse.y + BDATA->getVirtualPos().y;

				multiSelectMod = true;
		//	}

		//	//��ź ������ġ ����
		//	else
		//		getIOPsptDoll_crntFocus()->setTargetPosition();
		//}

		//else
		{
			//if (getIOPdoll_crntSquadFocus() != nullptr)
			//	getIOPdoll_crntSquadFocus()->SetMovePoint();

		}
	}


	//���� ��� ������
	if (KEYMANAGER->isKeyStayDown(VK_LBUTTON))
	{
		ePos.x = _ptMouse.x + VPOS->x;
		ePos.y = _ptMouse.y + VPOS->y;
	}


	//���� ��� ���� �Ϸ�
	if (KEYMANAGER->isKeyUp(VK_LBUTTON))
	{
		if (multiSelectMod)
		{
			ePos.x = _ptMouse.x + VPOS->x;
			ePos.y = _ptMouse.y + VPOS->y;

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
				cp.x = it.second->getCharacterPos().x;
				cp.y = it.second->getCharacterPos().y;

				if (PtInRect(&rc, cp))
				{
					vMultiSelect.push_back(it.first);
					it.second->revSelect();
				}
			}

			//���� �������� �����������
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


	//�̵�, ���ݴ�� ����
	if (KEYMANAGER->isKeyUp(VK_RBUTTON))
	{
		if (!Targetting_Other())
		{
			if (vMultiSelect.size() < 1)
			{
				//��Ŀ�̵� ������ �����Ѵٸ�
				if (getIOPdoll_crntSquadFocus() != nullptr)
					getIOPdoll_crntSquadFocus()->SetMovePoint();
			}

			//���� ������ ��쿡
			else
			{
				for (auto& it : vMultiSelect)
				{
					auto& me = (*getIOPdoll_crntSquad(it));

					trgPos.x = _ptMouse.x - (mPos.x - me.getCharacterPos().x) + VPOS->x;
					trgPos.y = _ptMouse.y - (mPos.y - me.getCharacterPos().y) + VPOS->y;

					me.MovePoint(trgPos.x, trgPos.y);
				}
			}
		}
	}


	//����
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


	//��ų ���
	if (KEYMANAGER->isKeyUp('E'))
	{
		if (vMultiSelect.size() < 1)
			if (this->getIOPdoll_crntSquadFocus() != nullptr)
				if (this->getIOPdoll_crntSquadFocus()->getAlive())
					this->getIOPdoll_crntSquadFocus()->Use_ActiveSkill();
	}
}

void Player::Change_FocusTacDoll_Func(int memID)
{
	if (this->getIOPdoll_crntSquad(memID) != nullptr)
	{
		//1ȸ ������ ����, 2�������� ��Ŀ��
		if (focusTacdolID != memID)
		{
			if (cFocusTacDollID != 0)
				this->getIOPdoll_crntSquad(cFocusTacDollID)->revFocus();

			if (focusTacdolID != 0)
				this->getIOPdoll_crntSquad(focusTacdolID)->revSelect();

			cFocusTacDollID = 0;
			focusTacdolID = memID;

			//Select FocusTacDoll
			this->getIOPdoll_crntSquad(focusTacdolID)->revSelect();
		}

		else
		{
			if (cFocusTacDollID == memID)
			{
				this->getIOPdoll_crntSquad(cFocusTacDollID)->revFocus();
				cFocusTacDollID = 0;
			}

			else
			{
				cFocusTacDollID = memID;
				this->getIOPdoll_crntSquad(cFocusTacDollID)->revFocus();
			}

		}
	}
}

void Player::SaveRemUnit(int padID)
{
	//���ϼ����ΰ��
	if (vMultiSelect.size() < 1)
	{
		//������ �Ǿ��������
		//if (this->getIOPsptDoll_crntFocus() != nullptr)
		{
			vector<UINT> temp;
			temp.push_back(getIOPdoll_crntSquadFocus()->getID()->SquadMem_ID);

			if (!mRemUnit.count(padID))
				mRemUnit.insert(make_pair(padID, temp));

			else
				mRemUnit.find(padID)->second.swap(temp);
		}
	}

	//���߼����ϰ��
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

void Player::LoadRemUnit(int padID)
{
	//�ش� �κ��� �����ϰ����� ���
	if (mRemUnit.count(padID))
	{
		auto& it = *(mRemUnit.find(padID));

		//���� ������ ��� -> �ش��������� ��Ŀ�� ����
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

		//���� �����ΰ��
		else if (it.second.size() > 1)
		{
			if (vMultiSelect.size() < 1)
			{
				if (cFocusTacDollID != 0)
					this->getIOPdoll_crntSquad(cFocusTacDollID)->revFocus();

				if (focusTacdolID != 0)
					this->getIOPdoll_crntSquad(focusTacdolID)->revSelect();

				focusTacdolID = 0;
				cFocusTacDollID = 0;
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

void Player::AutoSetting_CameraFocus()
{

}

bool Player::Targetting_Other()
{
	if (getIOPdoll_crntSquadFocus() != nullptr)
	{
		for (auto& it : BDATA->getEnemySquad(BDATA->getEngageSquadID())->mSquad)
		{
			//����
			if (!PtInRect(&(it.second->getCharacterRect()), _ptMouse))continue;
			getIOPdoll_crntSquadFocus()->Set_Targetting_Other(it.first);

			return true;
		}

		return false;
	}
}

void Player::testFuc()
{
	tacDoll->Create_IOPtacDoll(IOP_FNFAL);
	tacDoll->Create_IOPtacDoll(IOP_ARCHITECT);
}

void Player::testinst()
{
	tacDoll->getAllTacDoll().at(0)->init();
	tacDoll->InsertSquad_IOPtacDoll(1, 0);

	tacDoll->getAllTacDoll().at(1)->init();
	tacDoll->InsertSquad_IOPtacDoll(1, 1);
}

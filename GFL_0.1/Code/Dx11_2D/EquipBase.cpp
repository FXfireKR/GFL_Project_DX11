#include "stdafx.h"
#include "EquipBase.h"

BYTE EquipBase::seed = 0;

EquipBase::EquipBase()
{
	img_key = "";
}

EquipBase::~EquipBase()
{
}

HRESULT EquipBase::init(string key, WeaponString lstr, EQUIPTOTAL_TYPE etype)
{
	img_key = key;

	str = lstr;

	Tokenize_AttachAble();		//	���� ���θ� �и��� ����
	Tokenize_Spec();			//	������ �и��� ����.

	itemType = etype;
	equipType = (EQUIPTYPE)(((int)etype / 10) * 10);

	equipID = seed;
	++seed;

	return S_OK;
}

void EquipBase::render(FLOAT x, FLOAT y, FLOAT acl, FLOAT opc)
{
	/*float wid = (DRAW->getBitmap(img_key)->GetSize().width*acl) / 2.0f;
	float hei = (DRAW->getBitmap(img_key)->GetSize().height*acl) / 2.0f;

	DRAW->render(img_key, x - wid, y - hei, acl, opc);*/

	float wid = (IMAGEMAP->getImageInfo(img_key)->Width * acl) / 2.0f;
	float hei = (IMAGEMAP->getImageInfo(img_key)->Height * acl) / 2.0f;

	DRAW->render(img_key, DV2(wid, hei), DV2(x, y), DCR(1, 1, 1, opc));
}

void EquipBase::render(D3DXVECTOR2 _trans, D3DXVECTOR2 _scale, float _alpha)
{
	/*float wid = (IMAGEMAP->getImageInfo()->Width * _scale.x) / 2.0f;
	float hei = (DRAW->getBitmap(img_key)->GetSize().height*acl) / 2.0f;*/

	DRAW->render(img_key, _scale, _trans);
}

bool EquipBase::isAttachAble(TATICDOLL_WEAPON_TYPE twt)
{
	BYTE attachPlag;

	if (twt != TWT_NONE)
	{
		switch (twt)
		{
		case TWT_AR:
			attachPlag = ATTACH_AR;
			break;
		case TWT_RF:
			attachPlag = ATTACH_RF;
			break;
		case TWT_SR:
			attachPlag = ATTACH_SR;
			break;
		case TWT_MG:
			attachPlag = ATTACH_MG;
			break;
		case TWT_SG:
			attachPlag = ATTACH_SG;
			break;
		case TWT_HG:
			attachPlag = ATTACH_HG;
			break;
		case TWT_SMG:
			attachPlag = ATTACH_SMG;
			break;

		}

		if (equipAttachAble & attachPlag)
			return true;
	}
	return false;
}

void EquipBase::Tokenize_Spec()
{
	/*	[ �и��ؾ��� ���� ]
		1.ȭ��
		2.���
		3.��ź��
		4.����
		5.�尩
		6.����
		7.ȸ��
		8.�̵��ӵ�
		9.ġ��Ÿ��
		10.ġ������
	*/

	memset(&state, 0, sizeof(Status));

	string buf = str.spec;
	vector<string> vSpec;

	//�и��ؼ� ���Ϳ� ����
	while (buf.size() > 1)
	{
		string temp;
		temp.append(buf, 0, buf.find(" "));
		vSpec.push_back(temp);

		if (buf.find(" ") != string::npos)
			buf.erase(0, buf.find(" ") + 1);
		else
			buf.clear();
	}

	for (auto& it : vSpec)
	{
		if (it.find("ȭ��") != string::npos)
		{
			string s;
			s.append(it, it.find("ȭ��") + 4, it.size());
			state.AttackPoint = atoi(s.c_str());
		}

		else if (it.find("����") != string::npos)
		{
			string s;
			s.append(it, it.find("����") + 4, it.size());
			state.Accuracy = atof(s.c_str());
		}

		else if (it.find("ȸ��") != string::npos)
		{
			string s;
			s.append(it, it.find("ȸ��") + 4, it.size());
			state.Avoid = atof(s.c_str());
		}

		else if (it.find("���") != string::npos)
		{
			string s;
			s.append(it, it.find("���") + 4, it.size());
			state.AttackDelay = atof(s.c_str());
		}

		else if (it.find("����") != string::npos)
		{
			string s;
			s.append(it, it.find("����") + 4, it.size());
			state.ArmorPierce = atoi(s.c_str());
		}

		else if (it.find("�尩") != string::npos)
		{
			string s;
			s.append(it, it.find("�尩") + 4, it.size());
			state.ArmorPoint.max = atoi(s.c_str());
		}

		else if (it.find("ġ��Ÿ��") != string::npos)
		{
			string s;
			s.append(it, it.find("ġ��Ÿ��") + 8, it.size());
			state.CriticPoint = atof(s.c_str());
		}

		else if (it.find("ġ������") != string::npos)
		{
			string s;
			s.append(it, it.find("ġ������") + 8, it.size());
			state.CriticAcl = atof(s.c_str());
		}

		else if (it.find("�̵��ӵ�") != string::npos)
		{
			string s;
			s.append(it, it.find("�̵��ӵ�") + 8, it.size());
			state.AimDelay = atof(s.c_str());
		}
	}
}

void EquipBase::Tokenize_AttachAble()
{
	/*
		[ �и��ؾ� �� ���� ]
		
		1. ���� ����
		2. ���� �Ұ���
	
	*/

	string buf = str.native;

	BYTE equipAttachAble = 0;
	bool attachAble;

	// Plag Sort
	// AR SMG HG RF MG SG SR
	//  1  2  4  8  16 32 64

	if (buf.find("����Ұ�") == string::npos)
	{
		attachAble = false;
		equipAttachAble = pow(2, 7) - 1;	//	0111 1111 Set
	}

	else if (buf.find("���밡��") == string::npos)
	{
		attachAble = true;
		equipAttachAble = 0;
	}

	if (buf.find("AR") == string::npos)
		equipAttachAble = attachAble ? equipAttachAble += ATTACH_AR : equipAttachAble -= ATTACH_AR;

	if (buf.find("SMG") == string::npos)
		equipAttachAble = attachAble ? equipAttachAble += ATTACH_SMG : equipAttachAble -= ATTACH_SMG;

	if (buf.find("HG") == string::npos)
		equipAttachAble = attachAble ? equipAttachAble += ATTACH_HG : equipAttachAble -= ATTACH_HG;

	if (buf.find("RF") == string::npos)
		equipAttachAble = attachAble ? equipAttachAble += ATTACH_RF : equipAttachAble -= ATTACH_RF;

	if (buf.find("MG") == string::npos)
		equipAttachAble = attachAble ? equipAttachAble += ATTACH_MG : equipAttachAble -= ATTACH_MG;

	if (buf.find("SG") == string::npos)
		equipAttachAble = attachAble ? equipAttachAble += ATTACH_SG : equipAttachAble -= ATTACH_SG;

	if (buf.find("SR") == string::npos)
		equipAttachAble = attachAble ? equipAttachAble += ATTACH_SR : equipAttachAble -= ATTACH_SR;

	this->equipAttachAble = equipAttachAble;
}
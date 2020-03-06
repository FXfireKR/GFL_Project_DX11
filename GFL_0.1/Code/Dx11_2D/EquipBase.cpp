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

	Tokenize_AttachAble();		//	착용 여부를 분리해 낸다
	Tokenize_Spec();			//	스펙을 분리해 낸다.

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
	/*	[ 분리해야할 내용 ]
		1.화력
		2.사속
		3.장탄수
		4.관통
		5.장갑
		6.명중
		7.회피
		8.이동속도
		9.치명타율
		10.치명피해
	*/

	memset(&state, 0, sizeof(Status));

	string buf = str.spec;
	vector<string> vSpec;

	//분리해서 벡터에 저장
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
		if (it.find("화력") != string::npos)
		{
			string s;
			s.append(it, it.find("화력") + 4, it.size());
			state.AttackPoint = atoi(s.c_str());
		}

		else if (it.find("명중") != string::npos)
		{
			string s;
			s.append(it, it.find("명중") + 4, it.size());
			state.Accuracy = atof(s.c_str());
		}

		else if (it.find("회피") != string::npos)
		{
			string s;
			s.append(it, it.find("회피") + 4, it.size());
			state.Avoid = atof(s.c_str());
		}

		else if (it.find("사속") != string::npos)
		{
			string s;
			s.append(it, it.find("사속") + 4, it.size());
			state.AttackDelay = atof(s.c_str());
		}

		else if (it.find("관통") != string::npos)
		{
			string s;
			s.append(it, it.find("관통") + 4, it.size());
			state.ArmorPierce = atoi(s.c_str());
		}

		else if (it.find("장갑") != string::npos)
		{
			string s;
			s.append(it, it.find("장갑") + 4, it.size());
			state.ArmorPoint.max = atoi(s.c_str());
		}

		else if (it.find("치명타율") != string::npos)
		{
			string s;
			s.append(it, it.find("치명타율") + 8, it.size());
			state.CriticPoint = atof(s.c_str());
		}

		else if (it.find("치명피해") != string::npos)
		{
			string s;
			s.append(it, it.find("치명피해") + 8, it.size());
			state.CriticAcl = atof(s.c_str());
		}

		else if (it.find("이동속도") != string::npos)
		{
			string s;
			s.append(it, it.find("이동속도") + 8, it.size());
			state.AimDelay = atof(s.c_str());
		}
	}
}

void EquipBase::Tokenize_AttachAble()
{
	/*
		[ 분리해야 할 내용 ]
		
		1. 착용 가능
		2. 착용 불가능
	
	*/

	string buf = str.native;

	BYTE equipAttachAble = 0;
	bool attachAble;

	// Plag Sort
	// AR SMG HG RF MG SG SR
	//  1  2  4  8  16 32 64

	if (buf.find("착용불가") == string::npos)
	{
		attachAble = false;
		equipAttachAble = pow(2, 7) - 1;	//	0111 1111 Set
	}

	else if (buf.find("착용가능") == string::npos)
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
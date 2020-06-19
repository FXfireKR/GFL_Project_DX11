#pragma once

//	���� ������ ����ϴ� String
struct WeaponString
{
	string name;				//���� �̸�
	string spec;				//���� ����
	string explain;				//���� ����
	string native;				//���� ���뼳��
};

//	�����̰����� �κ�
enum AttachAblePlag
{
	ATTACH_AR = 1,
	ATTACH_SMG = 2,
	ATTACH_HG = 4,
	ATTACH_RF = 8,
	ATTACH_MG = 16,
	ATTACH_SG = 32,
	ATTACH_SR = 64
};

class EquipBase
{
private:
	static BYTE seed;			//	��� SEED
	BYTE equipID;				//	��� ID��

	string img_key;				//	�̹��� Key��
	WeaponString str;			//	������ ��� ��string

	EQUIPTOTAL_TYPE itemType;	//	������ Ÿ��
	EQUIPTYPE equipType;		//	������ Ÿ��

	BYTE equipAttachAble;		//	��� ���밡�� ����

	Status state;				//������ ����

public:
	EquipBase();
	~EquipBase();

	HRESULT init(string key, WeaponString lstr, EQUIPTOTAL_TYPE etype);		//	��� ����
	void render(FLOAT x, FLOAT y, FLOAT acl = 1.0F, FLOAT opc = 1.0F);		//	��� Image ������
	void render(D3DXVECTOR2 _trans, D3DXVECTOR2 _scale, float _alpha);		//	��� Image ������

	bool isAttachAble(TATICDOLL_WEAPON_TYPE twt);

private:
	void Tokenize_Spec();
	void Tokenize_AttachAble();

public:
	inline string getKey() { return img_key; }
	inline void setKey(string key) { img_key = key; }

	inline const Status& getState() { return state; }
	inline Status& p_getState() { return state; }

	inline const WeaponString& getString() { return str; }
	inline WeaponString& p_getString() { return str; }

	inline const EQUIPTOTAL_TYPE& getItemType() { return itemType; }
	inline EQUIPTOTAL_TYPE& p_getItemType() { return itemType; }

	inline const EQUIPTYPE& getEquipType() { return equipType; }
	inline EQUIPTYPE& p_getEquipType() { return equipType; }

	inline const BYTE getEquipID() { return equipID; }
};
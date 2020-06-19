#pragma once

//	무기 장착에 사용하는 String
struct WeaponString
{
	string name;				//무기 이름
	string spec;				//무기 스펙
	string explain;				//무기 설명
	string native;				//무기 착용설명
};

//	장착이가능한 부분
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
	static BYTE seed;			//	장비 SEED
	BYTE equipID;				//	장비 ID값

	string img_key;				//	이미지 Key값
	WeaponString str;			//	무기의 모든 내string

	EQUIPTOTAL_TYPE itemType;	//	아이템 타입
	EQUIPTYPE equipType;		//	부착물 타입

	BYTE equipAttachAble;		//	장비 착용가능 여부

	Status state;				//무기의 스펙

public:
	EquipBase();
	~EquipBase();

	HRESULT init(string key, WeaponString lstr, EQUIPTOTAL_TYPE etype);		//	장비 생성
	void render(FLOAT x, FLOAT y, FLOAT acl = 1.0F, FLOAT opc = 1.0F);		//	장비 Image 렌더링
	void render(D3DXVECTOR2 _trans, D3DXVECTOR2 _scale, float _alpha);		//	장비 Image 렌더링

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
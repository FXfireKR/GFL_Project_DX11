#pragma once
#include "EllipseBase.h"
#include "spineMotion.h"

template <typename T>
struct STATE {
	T max, curr;
};

//	���������� �Ҽ� ����
enum TATICDOLL_ALIANCE_TYPE
{
	ALIANCE_NONE = 0,		//	�߸�
	ALIANCE_GRIFFON,		//	�׸���
	ALIANCE_IRONBLOD,		//	ö������
	ALIANCE_SCCOM,			//	����Ÿ �� ����
	ALIANCE_PEREDEUS		//	�䷯���콺
};

//	���������� ���� ����
enum TATICDOLL_WEAPON_TYPE
{
	TWT_NONE = -1,
	TWT_AR,
	TWT_RF,
	TWT_SR,
	TWT_DMR,
	TWT_SG,
	TWT_HG,
	TWT_SMG
};

//	���������� �ɷ�ġ
struct Status
{
	STATE<int> HitPoint;		//	ü��
	STATE<int> ArmorPoint;		//	���� ��ġ	[ SG ���� ]

	int Armor;					//	��ü ����
	int ArmorPierce;			//	���� �ļ��ġ		[���������������, ������ ���� ���ֿ��� ���� �ټ�����.]
	double ArmorIgnore;			//	���� ���� ����		[ 0 ~ 1.0 ]
	
	STATE<int> ShieldPoint;		//	���� ��
	int ShieldPierce;			//	���� �ļ��ġ			[���������������, ������ ���� ���ֿ��� ���� �ټ�����.]
	double ShieldIgnore;		//	���� ���� Ȯ��		[ 0 ~ 1.0 ]

	int AttackPoint;			//	���ݷ�

	double Accuracy;			//	���߷�		[ 0 ~ 1.0 ]
	double Avoid;				//	ȸ����		[ 0 ~ 1.0 ]

	double CriticPoint;			//	ġ��Ÿ��		[ 0 ~ 1.0 ]
	double CriticAcl;			//	ġ������		[ 1 ~ INF ]

	double AttackDelay;			//	�������� ~ �� ���ݱ����� ���ð�
	double AimDelay;			//	��� ~ ����������� ���ؽð�
};

//	���������� ID
struct DOGTAG_ID
{
	signed int Squad_ID,	All_ID,		SquadMem_ID;
//			    �д��ȣ	    ��ü��ȣ      �д����ȣ
};

class TaticDoll
{
protected:	

	bool					Flip;					//	�������� �������Ǵ°�
	bool					isAlive;				//	����ִ°�
	bool					Select;					//	���õǾ��°�
	bool					Focus;					//	ī�޶� ��Ŀ���� �ΰ�
	bool					moveAble;				//	�����ϼ� �ִ°�

	RECT					charRect;				//	ĳ���� �ǰݹڽ�						

	float					Angle;
	float					TargetAngle;
	float					axisMax_LongRad;		//	������ �Ǵ� �ִ� ���� ��ݰ�
	float					axisMax_ShortRad;		//	������ �Ǵ� �ִ� ���� �ܹݰ�
	float					axisMin_LongRad;		//	������ �Ǵ� �ּ� ���� ��ݰ�
	float					axisMin_ShortRad;		//	������ �Ǵ� �ּ� ���� �ܹݰ�

	string					name;
	string					original_key;			//ĳ������ ���� �Ϸ���Ʈ path�Դϴ�.
	string					cardNormalKey;			//ĳ���� �޹���ִ� ���� �Ϸ���Ʈ�� KEY�� �Դϴ�.
	string					cardHurtKey;			//ĳ���� �޹���ִ� �λ� �Ϸ���Ʈ�� KEY�� �Դϴ�.

	double					sklColTime;
	double					atkColTime;
	double					moveSpd;
	double					msPointX;
	double					msPointY;

	Status					curState;
	Status					maxState;

	DOGTAG_ID				myID;		//	��ȣ�� -1�ϰ�� ������������.

	signed int				TargetID;
	
	D3DXCOLOR				_color;
	static ID3D11Buffer*	_colorBuffer;

	D3DXVECTOR2				dPos;		//	DrawPosition	[ ĳ���Ͱ� �׷����� ��ǥ ]
	D3DXVECTOR2				Pos;		//	Position		[ ĳ������ ���� �Ǵ� ��ǥ ]

	spineMotion*			motion;
	
	vector<SINT>			vRange;		//	��Ÿ����� ����ID
	vector<D3DXVECTOR2>		vPath;		//	�����̴� ��ġ ����

	TATICDOLL_WEAPON_TYPE	weaponType;
	TATICDOLL_ALIANCE_TYPE	alianceType;

	//map<EQUIPTYPE, EquipBase*> mEquip;

	map<string, EllipseBase*> mCollision;
	typedef map<string, EllipseBase*>::iterator iter_mCollision;

public:
	TaticDoll();
	TaticDoll(const TaticDoll& other) = delete;
	TaticDoll operator=(const TaticDoll& other) = delete;
	virtual ~TaticDoll();

	virtual void LoadTray_List();
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	//�и�
	virtual void render_VisualBar();
	virtual void render_Motion();
	virtual void render_Ellipse();

	virtual void Use_ActiveSkill();
	virtual void MotionUpdate();
	virtual void Update_DrawPos();
	virtual void Unit_CollitionCheck();		//���ְ��� SELF���� �浹üũ

	void update_Coltime();

	void Character_GetDamage(const Status& st);

	void StopMoving();											//�����̴°� ����
	void SetMovePoint();										//Ŭ���� ��Ҹ� ��������
	void MoveClickPoint();										//������ ��ҷ� �̵�
	bool MovePoint(float* nx, float* ny, float x, float y);		//���õ� ��ҷ� �̵�
	void MovePoint(float x, float y);

	void IsEnemy_at();
	bool FindEnemy_ID(SINT id);
	int getEnemy_ID_Pos(SINT id);

	void Set_Targetting();			//��Ÿ� ���� ���� ��Ŀ���մϴ�.
	void Set_MinTargetting();		//�ּ� ��Ÿ����� ���� ����Ŀ���մϴ�.
	void Set_Targetting_Angle();	//�����ÿ�, �ޱ��� �ֽ�ȭ ���ݴϴ�.

	void Set_Targetting_Other(SINT id);

private:
	void Limit_CharacterPosition();


public:
	inline bool isSelect() { return Select; }							//	Select�� ��ȯ�ؿ´�.
	inline void revSelect() { Select = Select ? false : true; }			//	Select�� bool���� ���������ش�

	inline bool isFocus() { return Focus; }
	inline void revFocus() { Focus = Focus ? false : true; }

	inline DOGTAG_ID* getID() { return &myID; }

	inline bool getAlive() const { return isAlive; }
	inline bool& setAlive() { return isAlive; }

	inline EllipseBase* getCollision(string key) 
		{ return mCollision.count(key) ? mCollision.find(key)->second : nullptr; }

	inline D3DXVECTOR2 getCharacterPos() const { return Pos; }
	inline D3DXVECTOR2* p_getCharacterPos() { return &Pos; }

	inline D3DXVECTOR2 getDrawPos() const { return dPos; }

	inline const RECT& getCharacterRect() { return charRect; }

	inline int getTargetID() { return TargetID; }

	inline const Status getStatus() const { return curState; }
	inline const Status getMaxStatus() const { return maxState; }
	inline Status& p_getStatus() { return curState; }
	inline Status& p_getMaxStatus() { return maxState; }

	inline string getName() const { return name; }

	inline const float& getViewAngle() { return Angle; }

	inline const bool getFlip() const { return Flip; }
	inline void setFlip(bool _flip) { Flip = _flip; }

};
#pragma once
#include "MotionManager.h"
#include "EllipseBase.h"
#include "VisualBar.h"

#define CHARACTERPOS_LIM_Y	340

enum TACDOLL_WEAPONTYPE
{
	TWP_NONE = -1,
	TWP_AR = 0,
	TWP_RF,
	TWP_SR,
	TWP_DMR,
	TWP_SG,
	TWP_HG,
	TWP_SMG
};

struct Status
{
	SPNT HeatPoint;		//	ü��

	SPNT ArmoPoint;		//	���� ��ġ [ SG�� ���� ]
	int Armor;			//	����
	double ArmorPierce;	//	���� �ļ����� [0.0d ~ 100.0d] �ۼ�Ʈ�� ��Ƽ� ����Ѵ�.

	SPNT ShieldPoint;		//	���� ��ġ (�ִ�ġ���� �𿩳������� �������� �ޱ�����Ѵ�.)	80%���� ������ 12.5%����
	UINT ShieldPierce;		//	���� �ļ��ġ	 [���������������, ������ ���� ���ֿ��� ���� �ټ�����. / �ݴ�� �����Ұ��, ������ ���� ���� ���Ϸ� ������������ �������� �ش�.]
	double SheildIgnore;	//	���� ���ü�ġ [0.0d ~ 100.0d] �ۼ�Ʈ�� ���ø� �ϰ� ���� �������� �ش�.

	int AttackPoint;	//	���ݷ�

	double HitPoint;	//	���߷� (0.0d ���� ����)
	double AvoidPoint;	//	ȸ���� (0.0d ���� ����)

	double CriticPoint;	//	ġ��Ÿ�� (0.0d ~ 100.0d)
	int CriticAcl;		//	ġ����� (100�̸�, ġ��Ÿ�� 100% �ߵ� => ġ��Ÿ�� 200%�� ���ݷ����� ���� �����¼� )

	double AttackDelay;		//	�������� ~ �� ���ݱ����� ���ð�
	double AimDelay;		//	��� ~ ����������� ���ؽð�
};

struct DOGTAG_ID
{
	UINT Squad_ID, All_ID, SquadMem_ID;
	//	�д��ȣ	,	��ü��ȣ,  �д����ȣ
};

class TaticDollBase
{
protected:
	TACDOLL_WEAPONTYPE weaponType;

	MotionManager* motion;
	//SoundManager* sound;

	VisualBar* hpBar;

	DOGTAG_ID myID;		//�д���� ��ȣ�� 0�ϰ�� ���������ʴ°����� ����
	Status state, maxState;
	RECT cr;			//ĳ���� ���ùڽ�

	D3DXCOLOR			_color;

	map<string, EllipseBase*> mCollision;
	map<string, EllipseBase*>::iterator miCollision;

protected:
	int Target_ID;				//Ÿ���� Tag

	string name;				//�������� �̸�

	FPNT dPos;					//�׷����� ��ǥ
	FPNT cPos;					//�ڽ��� �ν��ϴ� ��ǥ

	float SklColTime;			//��ų���� ���ð�
	float AtkColTime;			//�⺻���� ���� ���ð�

	float Speed;				//�̵��ӵ�

	float Angle, trgAngle;		//����, Ÿ���� ���� ����

	bool Flip;					//������⿡�� �������°�?
	bool isAlive;				//����ִ°�

public:
	TaticDollBase();
	virtual ~TaticDollBase();

	virtual void LoadTray_List();
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	//�и�
	virtual void render_VisualBar();
	virtual void render_Motion();
	virtual void render_Ellipse();

	virtual void Update_DrawPos();
	virtual void Unit_CollitionCheck();		//���ְ��� SELF���� �浹üũ

	void Limit_CharacterPosition();
	

	void Character_GetDamage(const Status& st);


public:
	inline DOGTAG_ID* getID() { return &myID; }
	inline bool getAlive() const { return isAlive; }
	inline bool& setAlive() { return isAlive; }

	inline EllipseBase* getCollision(string key) { return mCollision.count(key) ? mCollision.find(key)->second : nullptr; }

	inline FPNT getCharacterPos() { return cPos; }
	inline FPNT* p_getCharacterPos() { return &cPos; }

	inline FPNT getDrawPos() { return dPos; }

	inline RECT getCharacterRect() { return cr; }

	inline int getTargetID() { return Target_ID; }

	inline Status& p_getStatus() { return state; }
	inline const Status& getMaxStatus() { return maxState; }

	inline string getName() { return name; }

	inline const float& getViewAngle() { return Angle; }

	inline bool getFlip() const { return Flip; }
	inline bool& setFlip() { return Flip; }
};
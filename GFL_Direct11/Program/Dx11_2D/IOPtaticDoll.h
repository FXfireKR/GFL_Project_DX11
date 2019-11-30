#pragma once
#include "TaticDollBase.h"
//#include "EquipBase.h"

class IOPtaticDoll : public TaticDollBase
{
protected:
	string original_key;		//ĳ������ ���� �Ϸ���Ʈ path�Դϴ�.
	double msPointX;
	double msPointY;

	bool Select;		//���õǾ��°�
	bool Focus;
	bool moveAble;		//�����ϼ� �ִ°�

	vector<FPNT> vPath;		//�����̴� ��ġ ����
	vector<UINT> vRange;	//��Ÿ����� ����ID

	//map<EQUIPTYPE, EquipBase*> mEquip;

public:
	IOPtaticDoll();
	virtual ~IOPtaticDoll();

	virtual void LoadTray_List()override;
	virtual HRESULT init()override;
	virtual void release()override;
	virtual void update()override;
	virtual void render()override;
	virtual void Update_DrawPos()override;

	virtual void render_VisualBar();
	virtual void render_Motion();
	virtual void render_Ellipse();

	virtual void MotionUpdate();
	virtual void AttachEquipment();			//��� ����
	virtual void Use_ActiveSkill();

	virtual void Unit_CollitionCheck()override;		//���ְ��� SELF���� �浹üũ

public:
	void StopMoving();											//�����̴°� ����
	void SetMovePoint();										//Ŭ���� ��Ҹ� ��������
	void MoveClickPoint();										//������ ��ҷ� �̵�
	bool MovePoint(float* nx, float* ny, float x, float y);		//��õ� ��ҷ� �̵�
	void MovePoint(float x, float y);

	virtual void Camera_Focus();

	void IsEnemy_at();
	bool FindEnemy_ID(UINT id);
	int getEnemy_ID_Pos(UINT id);

	void Set_Targetting();			//��Ÿ� ���� ���� ��Ŀ���մϴ�.
	void Set_MinTargetting();		//�ּ� ��Ÿ����� ���� ����Ŀ���մϴ�.
	void Set_Targetting_Angle();	//�����ÿ�, �ޱ��� �ֽ�ȭ ���ݴϴ�.

	void Set_Targetting_Other(UINT id);

public:
	inline bool isSelect() { return Select; }							//Select�� ��ȯ�ؿ´�.
	inline void revSelect() { Select = Select ? false : true; }		//Select�� bool���� ���������ش�

	inline bool isFocus() { return Focus; }
	inline void revFocus() { Focus = Focus ? false : true; }


	//Static������ �Լ��� ����µ� ���Ǵ°͵�...
	inline MotionManager& getMotionManager() { return *motion; }
	//inline map<EQUIPTYPE, EquipBase*>& getEquipManager() { return mEquip; }

	inline float* getAttackColTime() { return &AtkColTime; }
	inline float* getSkillColTime() { return &SklColTime; }

	inline const Status& getStatus() { return state; }

	inline string getIllustKey() { return original_key; }
};
#pragma once
#include "TaticDollBase.h"
//#include "EquipBase.h"

class IOPtaticDoll : public TaticDollBase
{
protected:
	string original_key;		//캐릭터의 메인 일러스트 path입니다.
	double msPointX;
	double msPointY;

	bool Select;		//선택되었는가
	bool Focus;
	bool moveAble;		//움직일수 있는가

	vector<FPNT> vPath;		//움직이는 위치 지정
	vector<UINT> vRange;	//사거리내의 적군ID

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
	virtual void AttachEquipment();			//장비 착용
	virtual void Use_ActiveSkill();

	virtual void Unit_CollitionCheck()override;		//유닛간의 SELF구역 충돌체크

public:
	void StopMoving();											//움직이는걸 멈춤
	void SetMovePoint();										//클릭된 장소를 셋팅해줌
	void MoveClickPoint();										//지정된 장소로 이동
	bool MovePoint(float* nx, float* ny, float x, float y);		//명시된 장소로 이동
	void MovePoint(float x, float y);

	virtual void Camera_Focus();

	void IsEnemy_at();
	bool FindEnemy_ID(UINT id);
	int getEnemy_ID_Pos(UINT id);

	void Set_Targetting();			//사거리 내의 적을 포커싱합니다.
	void Set_MinTargetting();		//최소 사거리내의 적을 언포커싱합니다.
	void Set_Targetting_Angle();	//포착시에, 앵글을 최신화 해줍니다.

	void Set_Targetting_Other(UINT id);

public:
	inline bool isSelect() { return Select; }							//Select를 반환해온다.
	inline void revSelect() { Select = Select ? false : true; }		//Select의 bool값을 역전시켜준다

	inline bool isFocus() { return Focus; }
	inline void revFocus() { Focus = Focus ? false : true; }


	//Static형식의 함수를 만드는데 사용되는것들...
	inline MotionManager& getMotionManager() { return *motion; }
	//inline map<EQUIPTYPE, EquipBase*>& getEquipManager() { return mEquip; }

	inline float* getAttackColTime() { return &AtkColTime; }
	inline float* getSkillColTime() { return &SklColTime; }

	inline const Status& getStatus() { return state; }

	inline string getIllustKey() { return original_key; }
};
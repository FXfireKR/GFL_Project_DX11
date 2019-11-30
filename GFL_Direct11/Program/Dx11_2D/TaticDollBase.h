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
	SPNT HeatPoint;		//	체력

	SPNT ArmoPoint;		//	방어력 수치 [ SG에 적용 ]
	int Armor;			//	방어력
	double ArmorPierce;	//	방어력 파쇄정도 [0.0d ~ 100.0d] 퍼센트로 깎아서 계산한다.

	SPNT ShieldPoint;		//	역장 수치 (최대치에서 깎여나갈수록 데미지를 받기시작한다.)	80%부터 데미지 12.5%적용
	UINT ShieldPierce;		//	역장 파쇄수치	 [존재하지않을경우, 역장을 가진 유닛에게 딜을 줄수없다. / 반대로 존재할경우, 역장의 일정 공식 이하로 내려갔을때만 데미지를 준다.]
	double SheildIgnore;	//	역장 무시수치 [0.0d ~ 100.0d] 퍼센트로 무시를 하고 직접 데미지를 준다.

	int AttackPoint;	//	공격력

	double HitPoint;	//	명중률 (0.0d 부터 시작)
	double AvoidPoint;	//	회피율 (0.0d 부터 시작)

	double CriticPoint;	//	치명타율 (0.0d ~ 100.0d)
	int CriticAcl;		//	치명배율 (100이면, 치명타시 100% 추뎀 => 치명타면 200%의 공격력으로 적을 때리는셈 )

	double AttackDelay;		//	공격직후 ~ 재 공격까지의 대기시간
	double AimDelay;		//	대기 ~ 사격전까지의 조준시간
};

struct DOGTAG_ID
{
	UINT Squad_ID, All_ID, SquadMem_ID;
	//	분대번호	,	전체번호,  분대원번호
};

class TaticDollBase
{
protected:
	TACDOLL_WEAPONTYPE weaponType;

	MotionManager* motion;
	//SoundManager* sound;

	VisualBar* hpBar;

	DOGTAG_ID myID;		//분대관련 번호는 0일경우 존재하지않는것으로 판정
	Status state, maxState;
	RECT cr;			//캐릭터 선택박스

	D3DXCOLOR			_color;

	map<string, EllipseBase*> mCollision;
	map<string, EllipseBase*>::iterator miCollision;

protected:
	int Target_ID;				//타겟의 Tag

	string name;				//전술인형 이름

	FPNT dPos;					//그려지는 좌표
	FPNT cPos;					//자신이 인식하는 좌표

	float SklColTime;			//스킬재사용 대기시간
	float AtkColTime;			//기본공격 재사용 대기시간

	float Speed;				//이동속도

	float Angle, trgAngle;		//각도, 타겟을 보는 각도

	bool Flip;					//현재방향에서 뒤집혔는가?
	bool isAlive;				//살아있는가

public:
	TaticDollBase();
	virtual ~TaticDollBase();

	virtual void LoadTray_List();
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	//분리
	virtual void render_VisualBar();
	virtual void render_Motion();
	virtual void render_Ellipse();

	virtual void Update_DrawPos();
	virtual void Unit_CollitionCheck();		//유닛간의 SELF구역 충돌체크

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
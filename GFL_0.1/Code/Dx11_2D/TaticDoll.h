#pragma once
#include "EllipseBase.h"
#include "spineMotion.h"

template <typename T>
struct STATE {
	T max, curr;
};

//	전술인형의 소속 상태
enum TATICDOLL_ALIANCE_TYPE
{
	ALIANCE_NONE = 0,		//	중립
	ALIANCE_GRIFFON,		//	그리폰
	ALIANCE_IRONBLOD,		//	철혈공조
	ALIANCE_SCCOM,			//	쿠데타 군 병력
	ALIANCE_PEREDEUS		//	페러데우스
};

//	전술인형의 무기 종류
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

//	전술인형의 능력치
struct Status
{
	STATE<int> HitPoint;		//	체력
	STATE<int> ArmorPoint;		//	방패 수치	[ SG 한정 ]

	int Armor;					//	자체 방어력
	int ArmorPierce;			//	방어력 파쇄수치		[존재하지않을경우, 역장을 가진 유닛에게 딜을 줄수없다.]
	double ArmorIgnore;			//	방어력 무시 정도		[ 0 ~ 1.0 ]
	
	STATE<int> ShieldPoint;		//	역장 값
	int ShieldPierce;			//	역장 파쇄수치			[존재하지않을경우, 역장을 가진 유닛에게 딜을 줄수없다.]
	double ShieldIgnore;		//	역장 무시 확률		[ 0 ~ 1.0 ]

	int AttackPoint;			//	공격력

	double Accuracy;			//	명중률		[ 0 ~ 1.0 ]
	double Avoid;				//	회피율		[ 0 ~ 1.0 ]

	double CriticPoint;			//	치명타율		[ 0 ~ 1.0 ]
	double CriticAcl;			//	치명배율		[ 1 ~ INF ]

	double AttackDelay;			//	공격직후 ~ 재 공격까지의 대기시간
	double AimDelay;			//	대기 ~ 사격전까지의 조준시간
};

//	전술인형의 ID
struct DOGTAG_ID
{
	signed int Squad_ID,	All_ID,		SquadMem_ID;
//			    분대번호	    전체번호      분대원번호
};

class TaticDoll
{
protected:	

	bool					Flip;					//	뒤집혀서 랜더링되는가
	bool					isAlive;				//	살아있는가
	bool					Select;					//	선택되었는가
	bool					Focus;					//	카메라 포커싱중 인가
	bool					moveAble;				//	움직일수 있는가

	RECT					charRect;				//	캐릭터 피격박스						

	float					Angle;
	float					TargetAngle;
	float					axisMax_LongRad;		//	기준이 되는 최대 공격 장반경
	float					axisMax_ShortRad;		//	기준이 되는 최대 공격 단반경
	float					axisMin_LongRad;		//	기준이 되는 최소 공격 장반경
	float					axisMin_ShortRad;		//	기준이 되는 최소 공격 단반경

	string					name;
	string					original_key;			//캐릭터의 메인 일러스트 path입니다.
	string					cardNormalKey;			//캐릭터 뒷배경있는 정상 일러스트의 KEY값 입니다.
	string					cardHurtKey;			//캐릭터 뒷배경있는 부상 일러스트의 KEY값 입니다.

	double					sklColTime;
	double					atkColTime;
	double					moveSpd;
	double					msPointX;
	double					msPointY;

	Status					curState;
	Status					maxState;

	DOGTAG_ID				myID;		//	번호가 -1일경우 존재하지않음.

	signed int				TargetID;
	
	D3DXCOLOR				_color;
	static ID3D11Buffer*	_colorBuffer;

	D3DXVECTOR2				dPos;		//	DrawPosition	[ 캐릭터가 그려지는 좌표 ]
	D3DXVECTOR2				Pos;		//	Position		[ 캐릭터의 기준 판단 좌표 ]

	spineMotion*			motion;
	
	vector<SINT>			vRange;		//	사거리내의 적군ID
	vector<D3DXVECTOR2>		vPath;		//	움직이는 위치 지정

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

	//분리
	virtual void render_VisualBar();
	virtual void render_Motion();
	virtual void render_Ellipse();

	virtual void Use_ActiveSkill();
	virtual void MotionUpdate();
	virtual void Update_DrawPos();
	virtual void Unit_CollitionCheck();		//유닛간의 SELF구역 충돌체크

	void update_Coltime();

	void Character_GetDamage(const Status& st);

	void StopMoving();											//움직이는걸 멈춤
	void SetMovePoint();										//클릭된 장소를 셋팅해줌
	void MoveClickPoint();										//지정된 장소로 이동
	bool MovePoint(float* nx, float* ny, float x, float y);		//명시된 장소로 이동
	void MovePoint(float x, float y);

	void IsEnemy_at();
	bool FindEnemy_ID(SINT id);
	int getEnemy_ID_Pos(SINT id);

	void Set_Targetting();			//사거리 내의 적을 포커싱합니다.
	void Set_MinTargetting();		//최소 사거리내의 적을 언포커싱합니다.
	void Set_Targetting_Angle();	//포착시에, 앵글을 최신화 해줍니다.

	void Set_Targetting_Other(SINT id);

private:
	void Limit_CharacterPosition();


public:
	inline bool isSelect() { return Select; }							//	Select를 반환해온다.
	inline void revSelect() { Select = Select ? false : true; }			//	Select의 bool값을 역전시켜준다

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
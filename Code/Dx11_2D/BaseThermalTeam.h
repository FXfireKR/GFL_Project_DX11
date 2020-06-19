#pragma once

#include "EllipseBase.h"
#include "spineMotion.h"
#include "StatusManager.h"
 
/*
	@ ThermalSupportTeam Base Class @

	 - 화력지원소대를 직접적으로 SquadManager를 거치지않고 상위클래스에서 조절할 수 있게한다.
	 - 각각의 소대마다 1회 공격시 착탄숫자를 가지고, 공격범위를 가진다.
*/


class BaseThermalTeam
{
public:
	BaseThermalTeam();
	virtual ~BaseThermalTeam();

	virtual void LoadTray_SoundList();
	virtual void LoadTray_ImageList();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void reset();

	virtual void render_Motion();
	virtual void render_Ellipse();

	virtual void MotionUpdate();
	virtual void Update_DrawPos();

	void StopMoving();											//움직이는걸 멈춤
	void SetMovePoint();										//클릭된 장소를 셋팅해줌
	void MoveClickPoint();										//지정된 장소로 이동
	bool MovePoint(float* nx, float* ny, float x, float y);		//명시된 장소로 이동
	void MovePoint(float x, float y);

	void update_Coltime();

public:
	inline const bool& isSelect() { return Select; }					//	Select를 반환해온다.
	inline void revSelect() { Select = Select ? false : true; }			//	Select의 bool값을 역전시켜준다

	inline const bool& isFocus() { return Focus; }
	inline void revFocus() { Focus = Focus ? false : true; }

	inline DOGTAG_ID* getID() { return &myID; }

	inline const bool& getAlive() const { return isAlive; }
	inline bool& setAlive() { return isAlive; }

	inline StatusManager* getBuffList() { return statusManager; }

	inline const vector<SINT>& getRange() { return vRange; }

	inline EllipseBase* getCollision(string key) {
		return mCollision.count(key) ? mCollision.find(key)->second : nullptr;
	}

	inline VEC2 getCharacterPos() const { return Pos; }
	inline VEC2* p_getCharacterPos() { return &Pos; }

	inline VEC2 getDrawPos() const { return dPos; }

	inline const RECT& getCharacterRect() { return charRect; }

	inline const Status getStatus() const { return curState; }
	inline const Status getMaxStatus() const { return maxState; }
	inline Status& p_getStatus() { return curState; }
	inline Status& p_getMaxStatus() { return maxState; }

	inline string getName() const { return name; }

	inline const float& getViewAngle() const { return Angle; }

	inline const bool& getFlip() const { return Flip; }
	inline void setFlip(bool _flip) { Flip = _flip; }

	inline const bool& getMoveAble() const { return moveAble; }
	inline void setMoveAble(bool _move) { moveAble = _move; }

	inline spineMotion* getMotion() const { return motion; }

	inline double& pGetAtkColTime() { return atkColTime; }


protected:
	bool					Flip;					//	뒤집혀서 랜더링되는가
	bool					isAlive;				//	살아있는가
	bool					Select;					//	선택되었는가
	bool					Focus;					//	카메라 포커싱중 인가
	bool					moveAble;				//	움직일수 있는가

	RECT					charRect;				//	캐릭터 피격박스						

	float					Angle;
	float					TargetAngle;

	//	squad leader's angle value
	float				rAngle;
	float				dAngle;
	//	squad world position
	D3DXVECTOR2			pos;

	float					axisMax_LongRad;		//	기준이 되는 최대 공격 장반경
	float					axisMax_ShortRad;		//	기준이 되는 최대 공격 단반경
	float					axisMin_LongRad;		//	기준이 되는 최소 공격 장반경
	float					axisMin_ShortRad;		//	기준이 되는 최소 공격 단반경

	double					atkColTime;
	float					worldMoveSpd;
	float					moveSpd;
	float					msPointX;
	float					msPointY;

	Status					curState;
	Status					maxState;

	DOGTAG_ID				myID;		//	번호가 -1일경우 존재하지않음.

	D3DXCOLOR				_color;
	ID3D11Buffer*			_colorBuffer;

	D3DXVECTOR2				dPos;		//	DrawPosition	[ 캐릭터가 그려지는 좌표 ]
	D3DXVECTOR2				Pos;		//	Position		[ 캐릭터의 기준 판단 좌표 ]

	spineMotion*			motion;
	StatusManager*			statusManager;

	vector<SINT>			vRange;		//	사거리내의 적군ID

	UINT					nowNodeID;
	UINT					nextNodeID;
	vector<UINT>			vNextNode;

	TATICDOLL_ALIANCE_TYPE	alianceType;

	unordered_map<string, EllipseBase*> mCollision;
	typedef unordered_map<string, EllipseBase*>::iterator iter_mCollision;

public:
	string name;
};
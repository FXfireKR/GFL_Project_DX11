#pragma once

#include "EllipseBase.h"
#include "spineMotion.h"
#include "StatusManager.h"
 
/*
	@ ThermalSupportTeam Base Class @

	 - ȭ�������Ҵ븦 ���������� SquadManager�� ��ġ���ʰ� ����Ŭ�������� ������ �� �ְ��Ѵ�.
	 - ������ �Ҵ븶�� 1ȸ ���ݽ� ��ź���ڸ� ������, ���ݹ����� ������.
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

	void StopMoving();											//�����̴°� ����
	void SetMovePoint();										//Ŭ���� ��Ҹ� ��������
	void MoveClickPoint();										//������ ��ҷ� �̵�
	bool MovePoint(float* nx, float* ny, float x, float y);		//��õ� ��ҷ� �̵�
	void MovePoint(float x, float y);

	void update_Coltime();

public:
	inline const bool& isSelect() { return Select; }					//	Select�� ��ȯ�ؿ´�.
	inline void revSelect() { Select = Select ? false : true; }			//	Select�� bool���� ���������ش�

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
	bool					Flip;					//	�������� �������Ǵ°�
	bool					isAlive;				//	����ִ°�
	bool					Select;					//	���õǾ��°�
	bool					Focus;					//	ī�޶� ��Ŀ���� �ΰ�
	bool					moveAble;				//	�����ϼ� �ִ°�

	RECT					charRect;				//	ĳ���� �ǰݹڽ�						

	float					Angle;
	float					TargetAngle;

	//	squad leader's angle value
	float				rAngle;
	float				dAngle;
	//	squad world position
	D3DXVECTOR2			pos;

	float					axisMax_LongRad;		//	������ �Ǵ� �ִ� ���� ��ݰ�
	float					axisMax_ShortRad;		//	������ �Ǵ� �ִ� ���� �ܹݰ�
	float					axisMin_LongRad;		//	������ �Ǵ� �ּ� ���� ��ݰ�
	float					axisMin_ShortRad;		//	������ �Ǵ� �ּ� ���� �ܹݰ�

	double					atkColTime;
	float					worldMoveSpd;
	float					moveSpd;
	float					msPointX;
	float					msPointY;

	Status					curState;
	Status					maxState;

	DOGTAG_ID				myID;		//	��ȣ�� -1�ϰ�� ������������.

	D3DXCOLOR				_color;
	ID3D11Buffer*			_colorBuffer;

	D3DXVECTOR2				dPos;		//	DrawPosition	[ ĳ���Ͱ� �׷����� ��ǥ ]
	D3DXVECTOR2				Pos;		//	Position		[ ĳ������ ���� �Ǵ� ��ǥ ]

	spineMotion*			motion;
	StatusManager*			statusManager;

	vector<SINT>			vRange;		//	��Ÿ����� ����ID

	UINT					nowNodeID;
	UINT					nextNodeID;
	vector<UINT>			vNextNode;

	TATICDOLL_ALIANCE_TYPE	alianceType;

	unordered_map<string, EllipseBase*> mCollision;
	typedef unordered_map<string, EllipseBase*>::iterator iter_mCollision;

public:
	string name;
};
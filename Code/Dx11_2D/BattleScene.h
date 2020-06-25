#pragma once
#include "Scene.hpp"
#include "BTM_Loader.h"

class BattleScene : public Scene
{
private:
	//	Instad of #define ->
	const float		ZORDER_REFRESH_TIME = 0.2f;

	const BYTE		ANNIHILATION = 1 << 1;		//	모든 적 섬멸
	const BYTE		CONQUER_AREA = 1 << 2;		//	지역 점령
	const BYTE		PROTECT_AREA = 1 << 3;		//	지역 방어
	const BYTE		PROTECT_OBJ = 1 << 4;		//	유닛 보호

	const FLOAT		MAX_CONQUER_TIME = 100.0f;
	const FLOAT		M_MAX_CONQUER_TIME = -100.0f;

	enum class CONQUERSTATE {
		ENEMY_CONQUERING = -1,		//	적군 점령중
		NON_CONQUERING,				//	점령 없음
		REPLACE_CONQUER,			//	점령 대치중
		FRIENDLY_CONQUERING,		//	아군 점령중
		FRIENDLY_CONQ_COMPLETE		//	아군 점령 완료
	};

private:
	vector<POINT>					vRendList;
	map<FLOAT, map<FLOAT, POINT>>	mRendList;
	vector<EllipseBase*>			conquerArea;

	float							resultTimer;				//	결과 타이머
	float							conquerTimer;				//	점령 타이머	[ 음수 : 적군 / 양수 : 아군 ]

	CONQUERSTATE					conquerState;
	ColorF							conquerColor = ColorF(1, 1, 1, 1);
	ColorF							conquerTextColor = ColorF(1, 1, 1, 1);
	bool							battleOver;

	float							highLightScale;
	float							nextFloatTimer;
	float							floatingTimer;
	bool							highlight;

public:
	BattleScene();
	~BattleScene();

	// Scene을(를) 통해 상속됨
	void init() override;
	void release() override;
	void update() override;
	void render() override;

private:
	void ZOrder_Sort();
	void check_BattlePlag();

};
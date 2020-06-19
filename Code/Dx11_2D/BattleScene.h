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

private:
	vector<POINT>					vRendList;
	map<FLOAT, map<FLOAT, POINT>>	mRendList;

	float resultTimer;				//	결과 타이머
	bool battleOver;

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
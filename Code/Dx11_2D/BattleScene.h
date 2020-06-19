#pragma once
#include "Scene.hpp"
#include "BTM_Loader.h"

class BattleScene : public Scene
{
private:
	//	Instad of #define ->
	const float		ZORDER_REFRESH_TIME = 0.2f;

	const BYTE		ANNIHILATION = 1 << 1;		//	��� �� ����
	const BYTE		CONQUER_AREA = 1 << 2;		//	���� ����
	const BYTE		PROTECT_AREA = 1 << 3;		//	���� ���
	const BYTE		PROTECT_OBJ = 1 << 4;		//	���� ��ȣ

private:
	vector<POINT>					vRendList;
	map<FLOAT, map<FLOAT, POINT>>	mRendList;

	float resultTimer;				//	��� Ÿ�̸�
	bool battleOver;

public:
	BattleScene();
	~BattleScene();

	// Scene��(��) ���� ��ӵ�
	void init() override;
	void release() override;
	void update() override;
	void render() override;

private:
	void ZOrder_Sort();
	void check_BattlePlag();

};
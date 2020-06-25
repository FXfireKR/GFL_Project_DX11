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

	const FLOAT		MAX_CONQUER_TIME = 100.0f;
	const FLOAT		M_MAX_CONQUER_TIME = -100.0f;

	enum class CONQUERSTATE {
		ENEMY_CONQUERING = -1,		//	���� ������
		NON_CONQUERING,				//	���� ����
		REPLACE_CONQUER,			//	���� ��ġ��
		FRIENDLY_CONQUERING,		//	�Ʊ� ������
		FRIENDLY_CONQ_COMPLETE		//	�Ʊ� ���� �Ϸ�
	};

private:
	vector<POINT>					vRendList;
	map<FLOAT, map<FLOAT, POINT>>	mRendList;
	vector<EllipseBase*>			conquerArea;

	float							resultTimer;				//	��� Ÿ�̸�
	float							conquerTimer;				//	���� Ÿ�̸�	[ ���� : ���� / ��� : �Ʊ� ]

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

	// Scene��(��) ���� ��ӵ�
	void init() override;
	void release() override;
	void update() override;
	void render() override;

private:
	void ZOrder_Sort();
	void check_BattlePlag();

};
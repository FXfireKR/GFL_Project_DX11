#pragma once
#include "spineMotion.h"
#include "BTM_Loader.h"

class MainScene
{
private:
	//	Typedef
	typedef struct tagPoint { FLOAT x, y; } FPNT;

	//	Instad of #define ->
	const float Delta = g_pTimeManager->GetDeltaTime();
	const double AK12_AIM_TIME = 1.25;

protected:
	spineMotion*	ak12;
	spineMotion*	gua91;

	BTM_Loader*		loader;
	BattleMap*		btm;

	double			aimTime;
	//double			

public:
	MainScene();
	~MainScene();

	void init();
	void release();
	void update();
	void render();

	static void attackMotion(void* obj);
};
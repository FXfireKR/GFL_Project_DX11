#pragma once
#include "MotionManager.h"

class NTW_20
{
private:
	MotionManager* motion;

	float angle;
	D3DXVECTOR2		pos;

public:
	NTW_20();
	~NTW_20();

	static void LoadTrayList();
	void init();
	void release();
	void update();
	void render();

public:
	static void NTW20_AttackAction(void * ob);
	static void NTW20_VictoryAction(void * ob);

	inline MotionManager& getMotionManager() { return *motion; }
	inline float& getPosX() { return (float&)pos.x; }
	inline float& getPosY() { return (float&)pos.y; }
};
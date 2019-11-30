#pragma once
#include "Image.h"

enum BULLET_TYPE
{
	GRF_BLT = 0,
	SNV_BLT
};

class Bullet
{
private:
	BULLET_TYPE type;

	string  key;
	RECT rc;
	FPNT pos;

	FLOAT Limit_Distance;

	Status state;

	UINT targID;		// ´ë»ó ID

	Image* img;

	float Angle;
	float Speed;

	bool Fire;
	bool Arrive;

public:
	Bullet();
	~Bullet();

	HRESULT init();

	HRESULT Create_Bullet(string k, FPNT sp, UINT id, Status st, BULLET_TYPE t, float spd = 25.0f);		//Å¸°ÙÆÃ ÃÑ¾Ë
	HRESULT Create_Bullet(string k, FPNT sp, FLOAT angle, FLOAT limDist, Status st, BULLET_TYPE t, float spd = 25.0f);				//³íÅ¸°ÙÆÃ ÃÑ¾Ë

	void update();
	void update_TargetBullet();
	void update_NoneTargetBullet();

	void render();

public:
	inline const bool& getFire() { return Fire; }
	inline bool& getArrive() { return Arrive; }

	inline FPNT& getPos() { return pos; }

};
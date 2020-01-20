#pragma once

class Bullet
{
private:

	bool						Fire;
	bool						Arrive;

	RECT						rc;

	SINT						targID;		// ´ë»ó ID
	
	float						Angle;
	float						Speed;
	float						Limit_Distance;

	string						key;
	Status						state;

	D3DXVECTOR2					pos;
	D3DXVECTOR2					imgSize;

	TATICDOLL_ALIANCE_TYPE		bAliance;

public:
	Bullet();
	~Bullet();

	HRESULT init();

	HRESULT Create_Bullet(string k, D3DXVECTOR2 sp, UINT id, Status st, TATICDOLL_ALIANCE_TYPE t, float spd = 25.0f);									//Å¸°ÙÆÃ ÃÑ¾Ë
	HRESULT Create_Bullet(string k, D3DXVECTOR2 sp, FLOAT angle, FLOAT limDist, Status st, TATICDOLL_ALIANCE_TYPE t, float spd = 25.0f);				//³íÅ¸°ÙÆÃ ÃÑ¾Ë

	void update();
	void update_TargetBullet();
	void update_NoneTargetBullet();

	void render();

public:
	inline const bool& getFire() { return Fire; }
	inline bool& getArrive() { return Arrive; }

	inline D3DXVECTOR2& getPos() { return pos; }

};
#pragma once

#include "EllipseCol.h"
/*
	map<int, EllipseBase*> mCollision;	형태로 T_Doll에 이식.
	필요시마다, insert로 넣어주고, 필요없을시마다 erase로 빼줌.
*/

class EllipseBase
{
private:
	EllipseCol* esc;	//충돌을 위한 클래스

	float Long_Radius;	//장반경
	float Short_Radius;	//단반경

	float* CenterX;	//타원의 중심 X
	float* CenterY;	//타원의 중심 Y


//inline Fuction
public:
	inline float getCenterX() { return (*CenterX); };
	inline float getCenterY() { return (*CenterY); };
	inline float* p_getCenterX() { return CenterX; };
	inline float* p_getCenterY() { return CenterY; };
	inline float getLongRad() { return Long_Radius; };
	inline float getShortRad() { return Short_Radius; };


	//Normal Fuction
public:
	EllipseBase(void);
	EllipseBase(float* x, float* y, float lr, float sr);
	~EllipseBase(void);

	void Create_Ellipse(float* x, float* y, float lr, float sr);

	void Release_Ellipse();

	void Update_Ellipse();

	void Rend_Ellipse();
	void Rend_Ellipse(FLOAT x, FLOAT y);		//임의 지정된 위치에 랜더링
	void Rend_Ellipse(D3DXCOLOR c);

	void ChangeLadius(float lr, float sr);

	bool EllipseCollision_Check(float A_x, float A_y, float A_l, float A_s,
		float E_x, float E_y, float E_l, float E_s);

	bool PointCollision_Check(float ptx, float pty);

	bool EllipseCollision_Check(EllipseBase* EB);	//어디가 문제인가 ㅡ,ㅡ
	bool AABB_BoxCollition(EllipseBase* EB);

};
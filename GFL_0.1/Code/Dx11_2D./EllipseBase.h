#pragma once

#include "EllipseCol.h"
/*
	map<int, EllipseBase*> mCollision;	���·� T_Doll�� �̽�.
	�ʿ�ø���, insert�� �־��ְ�, �ʿ�����ø��� erase�� ����.
*/

class EllipseBase
{
private:
	EllipseCol* esc;	//�浹�� ���� Ŭ����

	float Long_Radius;	//��ݰ�
	float Short_Radius;	//�ܹݰ�

	float* CenterX;	//Ÿ���� �߽� X
	float* CenterY;	//Ÿ���� �߽� Y


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
	void Rend_Ellipse(FLOAT x, FLOAT y);		//���� ������ ��ġ�� ������
	void Rend_Ellipse(D3DXCOLOR c);

	void ChangeLadius(float lr, float sr);

	bool EllipseCollision_Check(float A_x, float A_y, float A_l, float A_s,
		float E_x, float E_y, float E_l, float E_s);

	bool PointCollision_Check(float ptx, float pty);

	bool EllipseCollision_Check(EllipseBase* EB);	//��� �����ΰ� ��,��
	bool AABB_BoxCollition(EllipseBase* EB);

};
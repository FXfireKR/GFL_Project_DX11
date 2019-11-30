#include "Stdafx.h"
#include "EllipseBase.h"

EllipseBase::EllipseBase(void)
{
}

EllipseBase::EllipseBase(float * x, float * y, float lr, float sr)
{
	Create_Ellipse(x, y, lr, sr);
}

EllipseBase::~EllipseBase(void)
{
}

void EllipseBase::Create_Ellipse(float* x, float* y, float lr, float sr)
{
	esc = new EllipseCol;
	rc = new DXCircle;

	Long_Radius = lr;
	Short_Radius = sr;

	CenterX = x;
	CenterY = y;

	rc->setScale(D3DXVECTOR2(lr * 2, sr * 2));
}

void EllipseBase::Release_Ellipse()
{
	if (rc != nullptr) {
		rc->release();
		SAFE_DEL(rc);
	}

	SAFE_DEL(esc);
	
	CenterX = NULL;
	CenterY = NULL;
}

void EllipseBase::Update_Ellipse()
{
	//rc = RectMakeCenter((*CenterX), (*CenterY), Long_Radius * 2, Short_Radius * 2);
	//rc->update();
}

void EllipseBase::Rend_Ellipse(HDC hdc)
{
	
}

void EllipseBase::Rend_Ellipse()
{
	D2D1_POINT_2F p;
	p.x = *CenterX - VPOS->x;
	p.y = *CenterY - VPOS->y;

	DRAWMANAGER->renderEllipse(p.x, p.y, Long_Radius, Short_Radius);
}

void EllipseBase::Rend_Ellipse(FLOAT x, FLOAT y)
{
	D2D1_POINT_2F p;
	p.x = x;
	p.y = y;

	DRAWMANAGER->renderEllipse(p.x, p.y, Long_Radius, Short_Radius);
}

void EllipseBase::Rend_Ellipse(D3DXCOLOR c)
{
	D2D1_POINT_2F p;
	p.x = *CenterX - VPOS->x;
	p.y = *CenterY - VPOS->y;
	//rc->setTrans(D3DXVECTOR2(p.x, p.y));
	//rc->render();
	//rc->setColor(c);
	//rc->render();

	DRAWMANAGER->renderEllipse(p.x, p.y, Long_Radius, Short_Radius, ColorF(c.r, c.g, c.b, c.a));
}

void EllipseBase::ChangeLadius(float lr, float sr)
{
	Long_Radius = lr;
	Short_Radius = sr;
}

bool EllipseBase::EllipseCollision_Check(float A_x, float A_y, float A_l, float A_s,
	float E_x, float E_y, float E_l, float E_s)
{
	//	1.	PtInRect로 1차 검증
	//	2.	EllipseCollition으로 2차 검증

	return AABB_Collition(A_x, A_y, A_l, A_s, E_x, E_y, E_l, E_s) ? esc->collide(A_x, A_y, A_l, A_s, E_x, E_y, E_l, E_s) : false;

	//if (AABB_Collition(A_x, A_y, A_l, A_s, E_x, E_y, E_l, E_s))
	//	return esc->collide(A_x, A_y, A_l, A_s, E_x, E_y, E_l, E_s);
	//else
	//	return false;
	
}

bool EllipseBase::PointCollision_Check(float ptx, float pty)
{
	float X = ptx - *CenterX;
	float Y = pty - *CenterY;

	float result = (pow(X, 2) / pow(Long_Radius, 2)) + (pow(Y, 2) / pow(Short_Radius, 2));

	if (result <= 1.0f)
		return true;
	else
		return false;
}

bool EllipseBase::EllipseCollision_Check(EllipseBase* EB)
{
	return AABB_Collition((*CenterX), (*CenterY), Long_Radius, Short_Radius, EB->getCenterX(), EB->getCenterY(), EB->getLongRad(), EB->getShortRad()) ? 
		esc->collide((*CenterX), (*CenterY), Long_Radius, Short_Radius, EB->getCenterX(), EB->getCenterY(), EB->getLongRad(), EB->getShortRad()) : false;

	//return esc->collide((*CenterX), (*CenterY), Long_Radius, Short_Radius, EB->getCenterX(), EB->getCenterY(), EB->getLongRad(), EB->getShortRad());
}

bool EllipseBase::AABB_BoxCollition(EllipseBase * EB)
{
	return AABB_Collition((*CenterX), (*CenterY), Long_Radius, Short_Radius, EB->getCenterX(), EB->getCenterY(), EB->getLongRad(), EB->getShortRad());
}

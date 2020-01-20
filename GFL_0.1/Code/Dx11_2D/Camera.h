#pragma once

#define g_pCamera Camera::GetInstance()

class Camera : public SingletonBase<Camera>
{
private:
	float				m_fDistance;		// 카메라와 타겟 사이의 거리
	float				m_fRotX;
	float				m_fRotY;

	POINT				m_ptPrevMouse;
	bool				m_isRButtonDown;

	D3DXVECTOR3			m_vEye;				// 카메라의 위치
	D3DXVECTOR3			m_vLookAt;			// 카메라의 타겟 위치
	D3DXVECTOR3			m_vUp;				// 임의의 업 벡터

	ViewProjMatrix*		m_pViewProjMatrix;	// 뷰x프로젝션 구조
	ID3D11Buffer*		m_pViewProjBuffer;	// 뷰x프로젝션 매트릭스 버퍼

public:
	Camera();
	~Camera();

	void Init();
	void Update(const D3DXVECTOR3* pTarget = NULL);

	const D3DXVECTOR3 GetDir();
	const D3DXVECTOR4 GetPos();
	const ViewProjMatrix* GetViewProjMatrix() { return m_pViewProjMatrix; }

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};


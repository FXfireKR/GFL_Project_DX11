#pragma once

#define g_pCamera Camera::GetInstance()

class Camera : public SingletonBase<Camera>
{
private:
	float				m_fDistance;		// ī�޶�� Ÿ�� ������ �Ÿ�
	float				m_fRotX;
	float				m_fRotY;

	POINT				m_ptPrevMouse;
	bool				m_isRButtonDown;

	D3DXVECTOR3			m_vEye;				// ī�޶��� ��ġ
	D3DXVECTOR3			m_vLookAt;			// ī�޶��� Ÿ�� ��ġ
	D3DXVECTOR3			m_vUp;				// ������ �� ����

	ViewProjMatrix*		m_pViewProjMatrix;	// ��x�������� ����
	ID3D11Buffer*		m_pViewProjBuffer;	// ��x�������� ��Ʈ���� ����

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


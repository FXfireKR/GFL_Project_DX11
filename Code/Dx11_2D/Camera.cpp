#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	m_pViewProjMatrix = new ViewProjMatrix;

	// 뷰 매트릭스 셋팅(임시로 아이덴티티로 설정)
	D3DXMatrixIdentity(&m_pViewProjMatrix->View);

	// 프로젝션 매트릭스 셋팅
	D3DXMatrixPerspectiveFovLH(&m_pViewProjMatrix->Projection, D3DX_PI * 0.25f, WINSIZEX / (float)WINSIZEY, 1, 1000);
	//D3DXMatrixTranspose(&m_pViewProjMatrix->Projection, &m_pViewProjMatrix->Projection);

	// 뷰x프로젝션 버퍼 생성
	CreateConstantBuffer(&m_pViewProjBuffer, sizeof(ViewProjMatrix), m_pViewProjMatrix);
}


Camera::~Camera()
{
	SAFE_RELEASE(m_pViewProjBuffer);
	SAFE_DELETE(m_pViewProjMatrix);
}

void Camera::Init()
{
	m_fDistance = 5;
	m_fRotX = 0;
	m_fRotY = 0;
	m_isRButtonDown = false;

	m_vEye = D3DXVECTOR3(0, 0, -m_fDistance);
	m_vLookAt = D3DXVECTOR3(0, 0, 0);
	m_vUp = D3DXVECTOR3(0, 1, 0);
}

void Camera::Update(const D3DXVECTOR3* pTarget)
{
	m_vEye = D3DXVECTOR3(0, 0, -m_fDistance);

	D3DXMATRIX rotX, rotY;
	D3DXMatrixRotationX(&rotX, m_fRotX);
	D3DXMatrixRotationY(&rotY, m_fRotY);

	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &(rotX * rotY));

	if (pTarget)
	{
		m_vLookAt = *pTarget;
		m_vEye = m_vEye + *pTarget;
	}

	D3DXMatrixLookAtLH(&m_pViewProjMatrix->View, &m_vEye, &m_vLookAt, &m_vUp);
	//D3DXMatrixTranspose(&m_pViewProjMatrix->View, &m_pViewProjMatrix->View);

	g_pDeviceContext->UpdateSubresource(m_pViewProjBuffer, 0, NULL, m_pViewProjMatrix, 0, 0);
	g_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pViewProjBuffer);
}

const D3DXVECTOR3 Camera::GetDir()
{
	D3DXVECTOR3 dir = m_vLookAt - m_vEye;
	D3DXVec3Normalize(&dir, &dir);

	return dir;
}

const D3DXVECTOR4 Camera::GetPos()
{
	return D3DXVECTOR4(m_vEye, 0);
}

void Camera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_RBUTTONDOWN:
		m_ptPrevMouse.x = LOWORD(lParam);
		m_ptPrevMouse.y = HIWORD(lParam);
		m_isRButtonDown = true;
		break;
	case WM_RBUTTONUP:
		m_isRButtonDown = false;
		break;
	case WM_MOUSEMOVE:
	{
		if (m_isRButtonDown)
		{
			POINT ptCurrMouse;
			ptCurrMouse.x = LOWORD(lParam);
			ptCurrMouse.y = HIWORD(lParam);

			m_fRotX += (ptCurrMouse.y - m_ptPrevMouse.y) / 100.0f;
			m_fRotY += (ptCurrMouse.x - m_ptPrevMouse.x) / 100.0f;

			if (m_fRotX < -D3DX_PI * 0.5f + D3DX_16F_EPSILON)
				m_fRotX = -D3DX_PI * 0.5f + D3DX_16F_EPSILON;
			else if (m_fRotX > D3DX_PI * 0.49f + D3DX_16F_EPSILON)
				m_fRotX = D3DX_PI * 0.49f + D3DX_16F_EPSILON;

			if (m_fRotY >= D3DX_PI * 2)
				m_fRotY = 0.0f;

			m_ptPrevMouse = ptCurrMouse;
		}
	}
	break;
	case WM_MOUSEWHEEL:
		m_fDistance -= GET_WHEEL_DELTA_WPARAM(wParam) / 100.0f;

		if (m_fDistance < 5)
			m_fDistance = 5;
		break;
	}
}

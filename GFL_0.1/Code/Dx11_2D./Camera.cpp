#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
	this->CameraReset();

	m_vEye = D3DXVECTOR3(0, 0, -5);
	m_vLookAt = D3DXVECTOR3(0, 0, 0);
	m_vUp = D3DXVECTOR3(0, 1, 0);

	// Matrix Setting	
	m_pViewProjMatrix = new ViewProjMatrix;
	D3DXMatrixLookAtLH(&m_pViewProjMatrix->View, &m_vEye, &m_vLookAt, &m_vUp);
	D3DXMatrixTranspose(&m_pViewProjMatrix->View, &m_pViewProjMatrix->View);

	// Projection
	D3DXMatrixOrthoOffCenterLH(&m_pViewProjMatrix->Projection, 0, (float)WINSIZEX, 0, (float)WINSIZEY, -1, 1);
	D3DXMatrixTranspose(&m_pViewProjMatrix->Projection, &m_pViewProjMatrix->Projection);
	
	// Create VPBuffer	
	D3D11_BUFFER_DESC desc = { 0, };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(ViewProjMatrix);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = Device->CreateBuffer(&desc, NULL, &m_pViewProjBuffer);
	assert(SUCCEEDED(hr));
}

Camera::~Camera()
{
	SAFE_DELETE(m_pViewProjMatrix);
	SAFE_RELEASE(m_pViewProjBuffer);
}

void Camera::update()
{
	keyboardUpdate();
}

void Camera::bufferUpdate()
{
	m_vEye = D3DXVECTOR3(CameraPositionX, CameraPositionY, -5);
	m_vLookAt = D3DXVECTOR3(CameraPositionX, CameraPositionY, 0);

	D3DXMatrixLookAtLH(&m_pViewProjMatrix->View, &m_vEye, &m_vLookAt, &m_vUp);
	D3DXMatrixTranspose(&m_pViewProjMatrix->View, &m_pViewProjMatrix->View);
	DeviceContext->UpdateSubresource(m_pViewProjBuffer, 0, NULL, m_pViewProjMatrix, 0, 0);
}

void Camera::keyboardUpdate()
{ 
	if (!cameraFix)
	{
		if (KEYMANAGER->isKeyStayDown(VK_LEFT))
			CameraPositionX -= GAMETIME->DeltaTime() * Speed;

		else if (KEYMANAGER->isKeyStayDown(VK_RIGHT))
			CameraPositionX += GAMETIME->DeltaTime() * Speed;

		else if (KEYMANAGER->isKeyStayDown(VK_DOWN))
			CameraPositionY -= GAMETIME->DeltaTime() * Speed;

		else if (KEYMANAGER->isKeyStayDown(VK_UP))
			CameraPositionY += GAMETIME->DeltaTime() * Speed;
	}

	if (CameraPositionX < 0)
		CameraPositionX = 0;
}

void Camera::CameraReset()
{
	CameraPositionX = CameraPositionY = 0;
}

void Camera::setCameraBuffer()
{
	DeviceContext->VSSetConstantBuffers(0, 1, &m_pViewProjBuffer);
}

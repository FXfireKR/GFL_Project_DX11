#include "stdafx.h"
#include "ViewCamera.h"

ViewCamera::ViewCamera()
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

#ifdef _DEBUG
	assert(SUCCEEDED(Device->CreateBuffer(&desc, NULL, &m_pViewProjBuffer)));
#else
	Device->CreateBuffer(&desc, NULL, &m_pViewProjBuffer);
#endif
}

ViewCamera::~ViewCamera()
{
	SAFE_DELETE(m_pViewProjMatrix);
	SAFE_RELEASE(m_pViewProjBuffer);
}

void ViewCamera::update(){
	keyboardUpdate();
}

void ViewCamera::bufferUpdate()
{
	m_vEye = D3DXVECTOR3(CameraPositionX, CameraPositionY, -5);
	m_vLookAt = D3DXVECTOR3(CameraPositionX, CameraPositionY, 0);

	D3DXMatrixLookAtLH(&m_pViewProjMatrix->View, &m_vEye, &m_vLookAt, &m_vUp);
	D3DXMatrixTranspose(&m_pViewProjMatrix->View, &m_pViewProjMatrix->View);
	DeviceContext->UpdateSubresource(m_pViewProjBuffer, 0, NULL, m_pViewProjMatrix, 0, 0);
}

void ViewCamera::keyboardUpdate(){

	if (!cameraFix)
	{
		if (KEYMANAGER->isKeyStayDown(VK_LEFT))
			CameraPositionX -= DELTA() * Speed;

		else if (KEYMANAGER->isKeyStayDown(VK_RIGHT))
			CameraPositionX += DELTA() * Speed;

		else if (KEYMANAGER->isKeyStayDown(VK_DOWN))
			CameraPositionY -= DELTA() * Speed;

		else if (KEYMANAGER->isKeyStayDown(VK_UP))
			CameraPositionY += DELTA() * Speed;
	}

	/*if (CameraPositionX < 0)
		CameraPositionX = 0;*/
}

void ViewCamera::CameraReset(){
	CameraPositionX = CameraPositionY = 0;
}

void ViewCamera::setCameraBuffer(){
	DeviceContext->VSSetConstantBuffers(0, 1, &m_pViewProjBuffer);
}
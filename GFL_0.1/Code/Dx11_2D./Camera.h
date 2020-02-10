#pragma once
#include "singleton.h"

class Camera : public singleton<Camera>
{
private:
	const float			Speed = 450.0f;			//	ī�޶� �����̴� �ӵ�

	D3DXVECTOR3			m_vEye;					// ī�޶��� ��ġ
	D3DXVECTOR3			m_vLookAt;				// ī�޶��� Ÿ�� ��ġ
	D3DXVECTOR3			m_vUp;					// ������ �� ����

	ViewProjMatrix*		m_pViewProjMatrix;		// ��x�������� ����
	ID3D11Buffer*		m_pViewProjBuffer;		// ��x�������� ��Ʈ���� ����

	bool				cameraFix;				//	ī�޶� ����

public:
	Camera();
	~Camera();

	void update();
	void bufferUpdate();
	void keyboardUpdate();
	void CameraReset();
	void setCameraBuffer();

public:
	inline const bool& getCameraFix() { return cameraFix; }
	inline void setCameraFix(bool _fix) { cameraFix = _fix; }
};
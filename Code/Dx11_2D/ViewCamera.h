#ifndef VIEWCAMERA_H
#define VIEWCAMERA_H

#include "singleton.hpp"

class ViewCamera : public singleton<ViewCamera>
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
	ViewCamera();
	~ViewCamera();

	void update();
	void bufferUpdate();
	void keyboardUpdate();
	void CameraReset();
	void setCameraBuffer();

public:
	inline const bool& getCameraFix() { return cameraFix; }
	inline void setCameraFix(bool _fix) { cameraFix = _fix; }

	inline const VEC3& getCameraPos() { return m_vEye; }
	inline const VEC3& getLookPos() { return m_vLookAt; }
	inline const VEC3& getUpDir() { return m_vUp; }
};

#endif // VIEWCAMERA_H
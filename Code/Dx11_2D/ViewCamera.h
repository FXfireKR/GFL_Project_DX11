#ifndef VIEWCAMERA_H
#define VIEWCAMERA_H

#include "singleton.hpp"

class ViewCamera : public singleton<ViewCamera>
{
private:
	const float			Speed = 450.0f;			//	카메라 움직이는 속도

	D3DXVECTOR3			m_vEye;					// 카메라의 위치
	D3DXVECTOR3			m_vLookAt;				// 카메라의 타겟 위치
	D3DXVECTOR3			m_vUp;					// 임의의 업 벡터

	ViewProjMatrix*		m_pViewProjMatrix;		// 뷰x프로젝션 구조
	ID3D11Buffer*		m_pViewProjBuffer;		// 뷰x프로젝션 매트릭스 버퍼

	bool				cameraFix;				//	카메라 고정

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
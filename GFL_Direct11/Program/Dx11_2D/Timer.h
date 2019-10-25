#pragma once
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")
// timeGetTime 함수를 사용하기 위한 라이브러리 추가
// 윈도우가 시작되고 난 후 시간을 리턴시켜 준다.

class Timer
{
private:
	bool			m_isHardware;		// 고성능 타이머 지원여부
	float			m_fTimeScale;		// 초당 시간 계산 범위
	float			m_fDeltaTime;		// 한 프레임 당 경과 시간
	__int64			m_nCurrTime;		// 현재 카운트 시간
	__int64			m_nLastTime;		// 마지막 카운트 시간

	unsigned int	m_nFrameRate;		// FPS (Frame Per Second)
	unsigned int	m_nFPSFrameCount;	// FPS 카운터
	float			m_fElapsedTime;		// 경과 시간 (초단위 초기화)
	float			m_fWorldTime;		// 전체 경과 시간

public:
	Timer();
	~Timer();

	void InitTimer();					// 타이머 초기화
	void Update(float lockFPS = 0.0f);	// 업데이트 (FPS 제한값 설정)

	unsigned int GetFPS(char* str = NULL) const { return m_nFrameRate; } // 현재 FPS
	float GetDeltaTime() const { return m_fDeltaTime; }	// 한 프레임당 경과 시간
	float GetWorldTime() const { return m_fWorldTime; }	// 전체 경과 시간
};


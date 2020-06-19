#include "stdafx.h"
#include "Timer.h"


Timer::Timer()
{
	__int64 nPeriodFrequency;	// 시간 업데이트 간격 초분할

	// 고성능 타이머 지원 여부 체크 : 마이크로 세컨드 (1 / 1,000,000)
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&nPeriodFrequency))
	{
		m_isHardware = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastTime);

		// 초당 시간 계산 범위
		m_fTimeScale = 1.0f / nPeriodFrequency;
	}
	else
	{
		m_isHardware = false;
		m_nLastTime = timeGetTime();
		m_fTimeScale = 0.001f; // 밀리 세컨드 (1 / 1,000)
	}

	InitTimer();
}


Timer::~Timer()
{
}

void Timer::InitTimer()
{
	if (m_isHardware)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastTime);
	}
	else
	{
		m_nLastTime = timeGetTime();
	}

	// 타이머 관련 변수 초기화
	m_nFrameRate = INT_MAX;
	m_nFPSFrameCount = 0;
	m_fElapsedTime = 0.0f;
	m_fWorldTime = 0.0f;
	m_fDeltaTime = 1.0f / 60.0f;
}

void Timer::Update(float lockFPS)
{
	if (m_isHardware)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrTime);
	}
	else
	{
		m_nCurrTime = timeGetTime();
	}

	// 초단위로 델타값 생성
	m_fDeltaTime = (m_nCurrTime - m_nLastTime) * m_fTimeScale;

	if (lockFPS > 0.0f)
	{
		// FPS 제한 설정
		while (m_fDeltaTime < (1.0f / lockFPS))
		{
			if (m_isHardware)
				QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrTime);
			else
				m_nCurrTime = timeGetTime();

			m_fDeltaTime = (m_nCurrTime - m_nLastTime) * m_fTimeScale;
		}
	}

	m_nLastTime = m_nCurrTime;
	m_nFPSFrameCount++;
	m_fElapsedTime += m_fDeltaTime;
	m_fWorldTime += m_fDeltaTime;

	// 1초후 프레임 갱신 및 초기화
	if (m_fElapsedTime > 1.0f)
	{
		m_nFrameRate = m_nFPSFrameCount;
		m_nFPSFrameCount = 0;
		m_fElapsedTime = 0;
	}
}
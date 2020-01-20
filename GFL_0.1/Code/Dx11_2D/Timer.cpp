#include "stdafx.h"
#include "Timer.h"


Timer::Timer()
{
	__int64 nPeriodFrequency;	// �ð� ������Ʈ ���� �ʺ���

	// ���� Ÿ�̸� ���� ���� üũ : ����ũ�� ������ (1 / 1,000,000)
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&nPeriodFrequency))
	{
		m_isHardware = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastTime);

		// �ʴ� �ð� ��� ����
		m_fTimeScale = 1.0f / nPeriodFrequency;
	}
	else
	{
		m_isHardware = false;
		m_nLastTime = timeGetTime();
		m_fTimeScale = 0.001f; // �и� ������ (1 / 1,000)
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

	// Ÿ�̸� ���� ���� �ʱ�ȭ
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

	// �ʴ����� ��Ÿ�� ����
	m_fDeltaTime = (m_nCurrTime - m_nLastTime) * m_fTimeScale;

	if (lockFPS > 0.0f)
	{
		// FPS ���� ����
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

	// 1���� ������ ���� �� �ʱ�ȭ
	if (m_fElapsedTime > 1.0f)
	{
		m_nFrameRate = m_nFPSFrameCount;
		m_nFPSFrameCount = 0;
		m_fElapsedTime = 0;
	}
}
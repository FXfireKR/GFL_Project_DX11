#pragma once
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")
// timeGetTime �Լ��� ����ϱ� ���� ���̺귯�� �߰�
// �����찡 ���۵ǰ� �� �� �ð��� ���Ͻ��� �ش�.

class Timer
{
private:
	bool			m_isHardware;		// ���� Ÿ�̸� ��������
	float			m_fTimeScale;		// �ʴ� �ð� ��� ����
	float			m_fDeltaTime;		// �� ������ �� ��� �ð�
	__int64			m_nCurrTime;		// ���� ī��Ʈ �ð�
	__int64			m_nLastTime;		// ������ ī��Ʈ �ð�

	unsigned int	m_nFrameRate;		// FPS (Frame Per Second)
	unsigned int	m_nFPSFrameCount;	// FPS ī����
	float			m_fElapsedTime;		// ��� �ð� (�ʴ��� �ʱ�ȭ)
	float			m_fWorldTime;		// ��ü ��� �ð�

public:
	Timer();
	~Timer();

	void InitTimer();					// Ÿ�̸� �ʱ�ȭ
	void Update(float lockFPS = 0.0f);	// ������Ʈ (FPS ���Ѱ� ����)

	unsigned int GetFPS(char* str = NULL) const { return m_nFrameRate; } // ���� FPS
	float GetDeltaTime() const { return m_fDeltaTime; }	// �� �����Ӵ� ��� �ð�
	float GetWorldTime() const { return m_fWorldTime; }	// ��ü ��� �ð�
};


#pragma once
#include "Timer.h"
#include "singleton.h"

class TimeManager : public singleton<TimeManager>
{
private:
	Timer* _timer;

public:
	TimeManager();
	~TimeManager();

	//�ʱ�ȭ
	HRESULT init();

	//������
	void release();

	//������ ����
	void update(float lock = 0.0f);

	//�ش� HDC�� ���� ��´�
	void render();

	inline float getFPS() const { return _timer->GetFPS(); }
	inline float getDeltaTime() const { return _timer->GetDeltaTime(); }
};
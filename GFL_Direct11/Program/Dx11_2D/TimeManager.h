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

	//초기화
	HRESULT init();

	//릴리즈
	void release();

	//프레임 적용
	void update(float lock = 0.0f);

	//해당 HDC에 정보 찍는다
	void render();

	inline float getFPS() const { return _timer->GetFPS(); }
	inline float getDeltaTime() const { return _timer->GetDeltaTime(); }
};
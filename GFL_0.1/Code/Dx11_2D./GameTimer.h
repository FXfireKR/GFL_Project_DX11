#pragma once
#include "singleton.h"
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")

class GameTimer : public singleton<GameTimer>
{
private:
	double		mSecondPerCount;
	double		mDeltaTime;

	float		fps;
	float		mspf;

	__int64		mBaseTime;
	__int64		mPausedTime;
	__int64		mStopTime;
	__int64		mPrevTime;
	__int64		mCurrTime;

	bool		mStopped;

public:
	GameTimer();

public:
	void Reset();
	void Start();
	void Stop();
	void Tick();

	void CalculateFrameStats();
	float TotalTime() const;

	inline float FPS() const { return fps; }
	inline float ElaspedTime() const { return mspf; }
	inline float GameTime()const { return TotalTime(); }
	inline float DeltaTime()const { return static_cast<float>(mDeltaTime); }
};
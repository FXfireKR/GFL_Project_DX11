#ifndef GAMETIMER_H 
#define GAMETIMER_H

#include "singleton.hpp"

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
	~GameTimer();

public:
	void Reset();
	void Start();
	void Stop();
	void Tick();

	void CalculateFrameStats();
	const float TotalTime() const;

	inline const float& FPS() const { return fps; }
	inline const float& ElaspedTime() const { return mspf; }
	inline const float GameTime()const { return TotalTime(); }
	inline const double& DeltaTime()const { return mDeltaTime; }
};

#endif
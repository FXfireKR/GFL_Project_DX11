#include "stdafx.h"
#include "GameTimer.h"

GameTimer::GameTimer()
	:mSecondPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0),
	mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false),
	fps(0.0), mspf(0.0)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondPerCount = 1.0 / static_cast<double>(countsPerSec);
}

GameTimer::~GameTimer(){}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (mStopped)
	{
		mPausedTime += (startTime - mStopTime);
		mPrevTime = startTime;
		mStopTime = 0;
		mStopped = false;
	}
}

void GameTimer::Stop()
{
	if (!mStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopTime = currTime;
		mStopped = true;
	}
}

void GameTimer::Tick()
{
	if (mStopped) {
		mDeltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;
	mDeltaTime = (mCurrTime - mPrevTime) * mSecondPerCount;

	// FPS 제한 설정
	while (mDeltaTime < (1.0f / 60.0f)){
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mCurrTime = currTime;
		mDeltaTime = (mCurrTime - mPrevTime) * mSecondPerCount;
	}

	mPrevTime = mCurrTime;

	if (mDeltaTime < 0.0)
		mDeltaTime = 0.0;
}

void GameTimer::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	++frameCnt;

	if (!(TotalTime() - timeElapsed < 1.0f))
	{
		fps = static_cast<float>(frameCnt);
		mspf = 1000.0f / fps;

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

const float GameTimer::TotalTime() const
{
	if (mStopped)
		return static_cast<float>(((mStopTime - mPausedTime) - mBaseTime) * mSecondPerCount);

	else
		return static_cast<float>(((mCurrTime - mPausedTime) - mBaseTime) * mSecondPerCount);
}

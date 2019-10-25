#include "stdafx.h"
#include "TimeManager.h"

TimeManager::TimeManager()
{
}

TimeManager::~TimeManager()
{
}

HRESULT TimeManager::init()
{
	//Ÿ�̸� �����Ѵ�
	_timer = new Timer;
	_timer->InitTimer();

	return S_OK;
}

void TimeManager::release()
{
	//Ÿ�̸� ����
	SAFE_DEL(_timer);
	_timer = NULL;
}

void TimeManager::update(float lock)
{
	if (_timer != NULL)
		_timer->Update(lock);
}

void TimeManager::render()
{
}

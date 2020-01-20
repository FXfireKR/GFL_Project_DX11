#include "stdafx.h"
#include "TimeManager.h"


TimeManager::TimeManager()
{
	m_pTimer = new Timer;
}


TimeManager::~TimeManager()
{
	SAFE_DELETE(m_pTimer);
}

void TimeManager::Init()
{
	if (m_pTimer)
		m_pTimer->InitTimer();
}

void TimeManager::Update(float lock)
{
	if (m_pTimer)
		m_pTimer->Update(lock);
}

void TimeManager::Render()
{
	int nFPS = m_pTimer->GetFPS();

	ImGui::InputInt("FPS", &nFPS);
}

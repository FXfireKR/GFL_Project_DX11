#include "stdafx.h"
#include "MotionManager.h"

MotionManager::MotionManager()
{
}

MotionManager::~MotionManager()
{
}

HRESULT MotionManager::Create(string motionKey, string key, FLOAT _pWidth, FLOAT _pHeight, FLOAT fps, bool rev)
{
	if (!mMotion.count(motionKey)) 
	{
		Motion* m = new Motion;
		m->Create_Motion(key, _pWidth, _pHeight, fps, rev);
		mMotion.insert(make_pair(motionKey, m));
	}

	return E_FAIL;
}

HRESULT MotionManager::RegisterFunction(string key, void * fnc, void * ob)
{
	if (mMotion.count(key))
	{
		mMotion.find(key)->second->RegistFunc(fnc, ob);
		return S_OK;
	}

	return E_FAIL;
}

void MotionManager::Change_Motion(string key)
{
	if (mMotion.count(key))
	{
		bfrMotion = crtMotion;
		crtMotion = key;

		if (mMotion.count(bfrMotion))
			mMotion.find(bfrMotion)->second->Reset_Motion();
	}
}

void MotionManager::release()
{
	for (auto& it : mMotion)
	{
		it.second->release();
		SAFE_DEL(it.second);
	}

	//mMotion.clear();
}

void MotionManager::update()
{
	if (mMotion.count(crtMotion))
		mMotion.find(crtMotion)->second->Update_Motion();
}

void MotionManager::pause()
{
	if (mMotion.count(crtMotion))
		mMotion.find(crtMotion)->second->Pause_Motion();
}
void MotionManager::render(FLOAT x, FLOAT y)
{
	if (mMotion.count(crtMotion))
		mMotion.find(crtMotion)->second->Render_Motion(x, y);
}

void MotionManager::render(FLOAT x, FLOAT y, FLOAT sx, FLOAT sy)
{
	if (mMotion.count(crtMotion))
		mMotion.find(crtMotion)->second->Render_Motion(x, y, sx, sy);
}

void MotionManager::render(FLOAT x, FLOAT y, FLOAT sx, FLOAT sy, FLOAT alpha)
{
	if (mMotion.count(crtMotion))
		mMotion.find(crtMotion)->second->Render_Motion(x, y, sx, sy, alpha);
}

void MotionManager::render(D3DXVECTOR2 t, D3DXVECTOR2 s, FLOAT alpha, FLOAT dgr)
{
	if (mMotion.count(crtMotion))
		mMotion.find(crtMotion)->second->Render_Motion(t, s, alpha, dgr);
}

void MotionManager::render(D3DXVECTOR2 t, D3DXVECTOR2 s, D3DXCOLOR color, FLOAT dgr)
{
	if (mMotion.count(crtMotion))
	{
		if (dgr < 0.0f)
			dgr += 360.0f;
		mMotion.find(crtMotion)->second->Render_Motion(t, s, color, dgr);
	}
}

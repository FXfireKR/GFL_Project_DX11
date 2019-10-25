#include "stdafx.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::release()
{
	for (auto& it : mScene)
	{
		it.second->release();
		SAFE_DEL(it.second);
	}
}

HRESULT SceneManager::Create_Scene(string key, Scene * sc)
{
	if (!mScene.count(key))
	{
		mScene.insert(make_pair(key, sc));
		currentScene = key;
	}
	
	return E_FAIL;
}

HRESULT SceneManager::Change_Scene(string key)
{
	if (mScene.count(key))
		currentScene = key;

	return E_FAIL;
}

HRESULT SceneManager::Init_Scene()
{
	if (mScene.size() > 0)
	{
		mScene.find(currentScene)->second->init();
		return S_OK;
	}
	return E_FAIL;
}

HRESULT SceneManager::Release_Scene()
{
	if (mScene.size() > 0)
	{
		mScene.find(currentScene)->second->release();
		return S_OK;
	}
	return E_FAIL;
}

HRESULT SceneManager::Update_Scene()
{
	if (mScene.size() > 0)
	{
		mScene.find(currentScene)->second->update();
		return S_OK;
	}
	return E_FAIL;
}

HRESULT SceneManager::Render_Scene()
{
	if (mScene.size() > 0)
	{
		mScene.find(currentScene)->second->render();
		return S_OK;
	}

	return E_FAIL;
}

#include "stdafx.h"
#include "SceneManager.h"

SceneManager::SceneManager()
	:iterCurScene(mScene.end())
{

}

SceneManager::~SceneManager()
{
	this->release();
	iterCurScene = mScene.end();
	SCENE->delInstance();
}

void SceneManager::release()
{
	for (iterCurScene = mScene.begin(); iterCurScene != mScene.end(); ++iterCurScene)
	{
		iterCurScene->second->release();
		SAFE_DELETE(iterCurScene->second);
	}
}

HRESULT SceneManager::Create_Scene(string key, Scene * sc)
{
	if (!mScene.count(key))
	{
		mScene.insert(make_pair(key, sc));
		curScene = key;

		return S_OK;
	}

	return E_FAIL;
}

HRESULT SceneManager::Change_Scene(string key)
{
	if (mScene.count(key))
	{
		revScene = curScene;
		curScene = key;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT SceneManager::Init_Scene()
{
	if ((iterCurScene = mScene.find(curScene)) != mScene.end())
	{
		iterCurScene->second->init();
		return S_OK;
	}
	return E_FAIL;
}

HRESULT SceneManager::Release_Scene()
{
	if ((iterCurScene = mScene.find(curScene)) != mScene.end())
	{
		iterCurScene->second->release();
		return S_OK;
	}
	return E_FAIL;
}

HRESULT SceneManager::Update_Scene()
{
	if ((iterCurScene = mScene.find(curScene)) != mScene.end())
	{
		iterCurScene->second->update();
		return S_OK;
	}
	return E_FAIL;
}

HRESULT SceneManager::Render_Scene()
{
	if ((iterCurScene = mScene.find(curScene)) != mScene.end())
	{
		iterCurScene->second->render();
		return S_OK;
	}

	return E_FAIL;
}

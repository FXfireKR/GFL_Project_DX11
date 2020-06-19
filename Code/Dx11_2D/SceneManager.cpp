#include "stdafx.h"
#include "SceneManager.h"

SceneManager::SceneManager() 
	: curSceneKey(""), beforeSceneKey(""), 
	curScene(nullptr), iterTarget(nullptr)
{
}

SceneManager::~SceneManager(){
	this->releaseScene();
}

const bool SceneManager::createScene(string _key, Scene * _data)
{
	if (Scenes.count(_key) == 0){
		if (_data != nullptr) {
			Scenes.insert(make_pair(_key, _data));
			return true;
		}
	}
	return false;
}

const bool SceneManager::changeScene(string _key, bool init)
{
	if ((iterScene = Scenes.find(_key)) != Scenes.end()) {
		beforeSceneKey = curSceneKey;
		curSceneKey = iterScene->first;
		curScene = iterScene->second != nullptr ?
			iterScene->second : nullptr;
		if (curScene != nullptr && init)
			curScene->init();
		return true;
	}
	return false;
}

void SceneManager::initScene(string _key)
{
	if ((iterScene = Scenes.find(_key)) != Scenes.end()) {
		if ((iterTarget = iterScene->second) != nullptr)
			iterTarget->init();
	}
}

void SceneManager::releaseScene()
{
	for (auto& iter : Scenes) {
		if (iter.second != nullptr) {
			iter.second->release();
			delete iter.second;
			iter.second = nullptr;
		}
	}
	Scenes.clear();
}

const bool SceneManager::releaseScene(string _key)
{
	if ((iterScene = Scenes.find(_key)) != Scenes.end()) {
		if ((iterTarget = iterScene->second) != nullptr) {
			iterTarget->release();
			SAFE_DELETE(iterTarget);
			return true;
		}
	}
	return false;
}

void SceneManager::update_curScene() const
{
	if (curScene != nullptr)
		curScene->update();
}

void SceneManager::update_Scene(string _key)
{
	if ((iterScene = Scenes.find(_key)) != Scenes.end()) {
		if ((iterTarget = iterScene->second) != nullptr)
			iterTarget->render();
	}
}

void SceneManager::render_curScene() const
{
	if (curScene != nullptr)
		curScene->render();
}

void SceneManager::render_Scene(string _key)
{
	if ((iterScene = Scenes.find(_key)) != Scenes.end())
		if ((iterTarget = iterScene->second) != nullptr)
			iterTarget->render();
}
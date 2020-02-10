#pragma once
#include "Scene.h"
#include "singleton.h"

class SceneManager : public singleton<SceneManager>
{
private:
	string								curScene;
	string								revScene;

	map<string, Scene*>					mScene;
	map<string, Scene*>::iterator		iterCurScene;

public:
	SceneManager();
	~SceneManager();

	void release();					//Scene Manager Release / Free Memory

	HRESULT Create_Scene(string key, Scene* sc);
	HRESULT Change_Scene(string key);

	HRESULT Init_Scene();			//Current Scene Initializer
	HRESULT Release_Scene();		//Current Scene Release
	HRESULT Update_Scene();			//Current Scene Update
	HRESULT Render_Scene();			//Current Scene Render
};
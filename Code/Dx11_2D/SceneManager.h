#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP
#include "Scene.hpp"
#include "singleton.hpp"

class SceneManager : public singleton<SceneManager>{
private:
	string curSceneKey;
	string beforeSceneKey;

	Scene* curScene;
	Scene* iterTarget;

	unordered_map<string, Scene*>			Scenes;
	unordered_map<string, Scene*>::iterator	iterScene;

public:
	SceneManager();
	~SceneManager();

	const bool createScene(string _key, Scene* _data);
	const bool changeScene(string _key, bool init = false);

	void initScene(string _key);

	void releaseScene();
	const bool releaseScene(string _key);
	void update_curScene()const;
	void update_Scene(string _key);
	void render_curScene()const;
	void render_Scene(string _key);

public:
	inline const string& getCurrentSceneKey() const { return curSceneKey; }
	inline const string& getBeforeSceneKey() const { return beforeSceneKey; }
	
	inline const unordered_map<string, Scene*>& getSceneList() const { return Scenes; }
};

#endif // !SCENEMANAGER_HPP
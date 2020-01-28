#pragma once
#include "ThreadPool.h"
#include "singleton.h"

class LoadManager : public singleton<LoadManager>
{
private:
	struct LoadResourceInfo
	{
		string key;
		string path;
		LOADRESOURCE_TYPE type;
		UINT frameX, frameY;
	};

private:
	string						nextScene;			//	로딩이 끝난뒤에 넘어갈 Scene Key
	string						storyScript;		//	스토리모드시, 읽어들일 스크립트

	vector<LoadResourceInfo*>	LoadList;			//로딩할 리스트
	LoadResourceInfo*			curLoad;			//진행중인 리스트
	vector<thread>				vThreadList;		//쓰레드 리스트

	string						LoadImageKey;		//로딩씬에 사용할 배경 이미지 키

	bool			threadLock;
	bool			autoInit;

	int ListPointer;

	ThreadPool*					loadThreadPool;

public:
	LoadManager();
	~LoadManager();

	void Add_LoadTray(__in string _key, __in const char* _path, __in LOADRESOURCE_TYPE type);
	void Add_LoadTray(__in string _key, __in string _path, __in LOADRESOURCE_TYPE type, __in SINT _frameX = 0, __in SINT _frameY = 0);
	void release();
	void update();


public:
	inline ThreadPool* const pGetThreadPool() { return loadThreadPool; }

	inline string getNextScene() const { return nextScene; }
	inline void setNextScene(string key) { nextScene = key; }

	inline int getLoadListSize() const { return LoadList.size(); }
	inline int getLoadPointer() const { return ListPointer; }

	inline bool getAutoInit() const { return autoInit; }
	inline void setAutoInit(bool _atinit) { autoInit = _atinit; }

	inline void setLoadImageKey(string key) { LoadImageKey = key; }
	inline string getLoadImageKey() const { return LoadImageKey; }

	inline string getStoryScriptPath()const { return storyScript; }
	inline void setStoryScriptPath(string path) { storyScript = path; }

};
#ifndef LOADMANAGER_H
#define LOADMANAGER_H

#include "ThreadPool.h"
#include "singleton.hpp"

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

	vector<LoadResourceInfo*>	LoadList;			//	로딩할 리스트
	vector<LoadResourceData>*	unloadList;			//	언로드할 리스트
	LoadResourceInfo*			curLoad;			//	진행중인 리스트
	vector<thread>				vThreadList;		//	쓰레드 리스트

	string						LoadImageKey;		//	로딩씬에 사용할 배경 이미지 키

	bool						threadLock;			//	쓰레드락
	bool						autoInit;			//	자동으로 init함수 호출 여부

	float						progPercent;		//	현재 진행중인 퍼센트
	float						targPercent;		//	타겟 퍼센트

	int							ListPointer;

	ThreadPool*					loadThreadPool;

public:
	LoadManager();
	~LoadManager();

	void Add_LoadTray(__in string _key, __in const char* _path, __in LOADRESOURCE_TYPE type);
	void Add_LoadTray(__in string _key, __in string _path, __in LOADRESOURCE_TYPE type, __in SINT _frameX = 0, __in SINT _frameY = 0);
	void release();

	bool mallocThread();
	bool freeThread();
	bool loadUpdate();
	bool unloadUpdate();

	void insertUnLoadList(vector<LoadResourceData>& unloadList);

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

	inline const float& getCurPercent() const { return progPercent; }
	inline const float& getTargPercent() const { return targPercent; }

	inline const vector<LoadResourceInfo*>  getLoadList() { return LoadList; }

};
#endif
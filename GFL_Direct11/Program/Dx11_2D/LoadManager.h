#pragma once
#include "singleton.h"

class LoadManager : public singleton<LoadManager>
{
private:
	struct LoadResourceInfo
	{
		string key;
		string path;
		LOADRESOURCE_TYPE type;
	};

private:
	string nextScene;						//로딩이 끝난뒤에 넘어갈 Scene Key

	vector<LoadResourceInfo*>	LoadList;			//로딩할 리스트
	LoadResourceInfo*			curLoad;			//진행중인 리스트
	vector<thread>				vThreadList;		//쓰레드 리스트

	
	bool			threadLock;
	bool			autoInit;

	int ListPointer;

public:
	LoadManager();
	~LoadManager();

	void Add_LoadTray(__in string _key, __in const char* _path, __in LOADRESOURCE_TYPE type);
	void Add_LoadTray(__in string _key, __in string _path, __in LOADRESOURCE_TYPE type);
	void update();

public:
	inline string getNextScene() const { return nextScene; }
	inline void setNextScene(string key) { nextScene = key; }

	inline int getLoadListSize() const { return LoadList.size(); }
	inline int getLoadPointer() const { return ListPointer; }

	inline bool getAutoInit() const { return autoInit; }
	inline void setAutoInit(bool _atinit) { autoInit = _atinit; }
	
};
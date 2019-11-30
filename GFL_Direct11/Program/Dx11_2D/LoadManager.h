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
	string nextScene;						//�ε��� �����ڿ� �Ѿ Scene Key

	vector<LoadResourceInfo*>	LoadList;			//�ε��� ����Ʈ
	LoadResourceInfo*			curLoad;			//�������� ����Ʈ
	vector<thread>				vThreadList;		//������ ����Ʈ

	
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
#include "stdafx.h"
#include "LoadManager.h"

LoadManager::LoadManager()
{
	curLoad = nullptr;
	ListPointer = 0;
	threadLock = false;
	autoInit = true;

	loadThreadPool = nullptr;

	progPercent = targPercent = 0.0f;
}

LoadManager::~LoadManager()
{
	this->release();
}

void LoadManager::Add_LoadTray(string _key, const char * _path, __in LOADRESOURCE_TYPE type)
{
	switch (type)
	{
	case RESOURCE_SOUND:
		if (SOUND->isValidKey(_key))
		{
			LoadResourceInfo* info = new LoadResourceInfo;

			info->key = _key;
			info->path = _path;
			info->type = type;

			LoadList.push_back(info);
		}
		break;
	case RESOURCE_IMAGE:
		if (IMAGEMAP->isValidKey(_key))
		{
			LoadResourceInfo* info = new LoadResourceInfo;

			info->key = _key;
			info->path = _path;
			info->type = type;
			info->frameX = 0;
			info->frameY = 0;

			LoadList.push_back(info);
		}
		break;
	}


}

void LoadManager::Add_LoadTray(string _key, string _path, LOADRESOURCE_TYPE type, SINT _frameX, SINT _frameY)
{
	// is there same Key?

	bool isSame = false;
	for (auto& it : LoadList)
	{
		if (!it->key.compare(_key))
		{
			isSame = true;
			break;
		}
	}

	if (!isSame) {

		switch (type)
		{
		case RESOURCE_SOUND:
		{
			if (SOUND->isValidKey(_key))
			{
				LoadResourceInfo* info = new LoadResourceInfo;

				info->key = _key;
				info->path = _path;
				info->type = type;

				LoadList.push_back(info);
			}
		}
		break;

		case RESOURCE_IMAGE:
			if (IMAGEMAP->isValidKey(_key))
			{
				LoadResourceInfo* info = new LoadResourceInfo;

				info->key = _key;
				info->path = _path;
				info->type = type;
				info->frameX = _frameX;
				info->frameY = _frameY;

				LoadList.push_back(info);
			}
			break;
		}



	}
}

void LoadManager::release()
{
}

bool LoadManager::mallocThread()
{
	if (loadThreadPool == nullptr){
		loadThreadPool = new ThreadPool;
		loadThreadPool->SetThread(8);
		return true;
	}
	return false;
}

bool LoadManager::freeThread()
{
	if (loadThreadPool != nullptr) {
		loadThreadPool->release();
		SAFE_DELETE(loadThreadPool);
		loadThreadPool = nullptr;
		return true;
	}
	return false;
}

bool LoadManager::loadUpdate()
{
	if (!threadLock)
	{
		thread trd([&]() {

			while (LoadList.size() != static_cast<UINT>(ListPointer))
			{
				if (curLoad == nullptr && LoadList.size() > static_cast<UINT>(ListPointer))
				{
					curLoad = LoadList[ListPointer];

					switch (curLoad->type)
					{
					case RESOURCE_SOUND:
					#ifdef _DEBUG
						SOUND->InsertSoundFile(LoadList[ListPointer]->key, LoadList[ListPointer]->path);
					#else
						SOUND->InsertSoundBianry(LoadList[ListPointer]->key, LoadList[ListPointer]->path);
					#endif // _DEBUG

						
						break;

					case RESOURCE_IMAGE:
					#ifdef _DEBUG
						IMAGEMAP->InsertImageFile(LoadList[ListPointer]->key, LoadList[ListPointer]->path);
					#else
						IMAGEMAP->InsertImageBinary(LoadList[ListPointer]->key, LoadList[ListPointer]->path,
							LoadList[ListPointer]->frameX, LoadList[ListPointer]->frameY);
					#endif // _DEBUG						
						break;

					default:
						break;
					}

					locker.lock();
					curLoad = nullptr;
					++ListPointer;
					locker.unlock();
				}
			}

		});
		trd.detach();

		threadLock = true;
	}

	else
	{
		float size = static_cast<float>(1.0f / static_cast<float>(LoadList.size()));

		targPercent = LoadList.size() > 0 ?
			static_cast<float>(static_cast<float>(ListPointer) * size * static_cast<float>(WINSIZEX)) : static_cast<float>(WINSIZEX);

		if (progPercent < targPercent) {
			if (targPercent - progPercent > FLOAT_EPSILON)
				progPercent += (ListPointer == LoadList.size()) ? DELTA() * 900.0f : DELTA() * 250.0f;
			
			else
				progPercent = targPercent;
		}

		else {
			if (LoadList.size() == static_cast<UINT>(ListPointer))
			{
				for (auto& it : LoadList)
					SAFE_DELETE(it);
				LoadList.clear();

				if (freeThread()) {

					SCENE->changeScene(this->getNextScene(), autoInit);

					ListPointer = 0;
					threadLock = false;
					progPercent = targPercent = 0.0f;
					return true;
				}

				else
				{
					SCENE->changeScene(this->getNextScene(), autoInit);

					ListPointer = 0;
					threadLock = false;
					progPercent = targPercent = 0.0f;
					return false;
				}
			}
		}	
	}

	return false;

}

bool LoadManager::unloadUpdate()
{
	if ((*unloadList).size() > ListPointer) {
		switch ((*unloadList)[ListPointer].type)
		{
		case RESOURCE_SOUND:
			break;

		case RESOURCE_IMAGE:
			IMAGEMAP->ReleaseMemory((*unloadList)[ListPointer].resourceKey);
			break;
		};
		++ListPointer;
	}

	float size = static_cast<float>(1.0f / static_cast<float>((*unloadList).size()));

	targPercent = (*unloadList).size() > 0 ?
		static_cast<float>(static_cast<float>(ListPointer) * size * static_cast<float>(WINSIZEX)) : static_cast<float>(WINSIZEX);

	if (progPercent < targPercent) {
		if (targPercent - progPercent > FLOAT_EPSILON)
			progPercent += (ListPointer == (*unloadList).size()) ? DELTA() * 900.0f : DELTA() * 250.0f;
		else
			progPercent = targPercent;
	}

	else {
		if ((*unloadList).size() == static_cast<UINT>(ListPointer))
		{
			for (auto& it : LoadList)
				SAFE_DELETE(it);
			LoadList.clear();

			if (freeThread()) {

				SCENE->changeScene(this->getNextScene(), autoInit);

				ListPointer = 0;
				threadLock = false;
				progPercent = targPercent = 0.0f;
				return true;
			}

			else
			{
				SCENE->changeScene(this->getNextScene(), autoInit);

				ListPointer = 0;
				threadLock = false;
				progPercent = targPercent = 0.0f;
				return false;
			}
		}
	}

	return false;
}

void LoadManager::insertUnLoadList(vector<LoadResourceData>& unloadList)
{
	//assert(unloadList != NULL);
	this->unloadList = &unloadList;
}
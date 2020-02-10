#include "stdafx.h"
#include "LoadManager.h"

LoadManager::LoadManager()
{
	curLoad = nullptr;
	ListPointer = 0;
	threadLock = false;
	autoInit = true;

	loadThreadPool = nullptr;
}

LoadManager::~LoadManager()
{
}

void LoadManager::Add_LoadTray(string _key, const char * _path, __in LOADRESOURCE_TYPE type)
{
	switch (type)
	{
	case RESOURCE_SOUND:
		if (SOUNDMANAGER->isValidKey(_key))
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
			if (SOUNDMANAGER->isValidKey(_key))
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

void LoadManager::update()
{
	if (loadThreadPool == nullptr)
	{
		loadThreadPool = new ThreadPool;
		loadThreadPool->SetThread(8);
	}

	else
	{

		if (!threadLock)
		{
			thread trd([&]() {

				while (LoadList.size() != ListPointer)
				{
					if (curLoad == nullptr && LoadList.size() > ListPointer)
					{
						curLoad = LoadList[ListPointer];

						switch (curLoad->type)
						{
						case RESOURCE_SOUND:
							SOUNDMANAGER->InsertSoundBianry(LoadList[ListPointer]->key, LoadList[ListPointer]->path);
							break;

						case RESOURCE_IMAGE:
							IMAGEMAP->InsertImageBinary(LoadList[ListPointer]->key, LoadList[ListPointer]->path, 
								LoadList[ListPointer]->frameX, LoadList[ListPointer]->frameY);
							break;

						case RESOURCE_MAP:
							break;

						case RESOURCE_EQUIP:
							break;

						case RESOURCE_TEXT:
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
			if (LoadList.size() == ListPointer)
			{
				for (auto& it : LoadList)
					SAFE_DEL(it);
				LoadList.clear();

				if (loadThreadPool != nullptr)
				{
					loadThreadPool->release();
					SAFE_DEL(loadThreadPool);
				}

				SCENE->Change_Scene(this->getNextScene());

				if (autoInit)
					SCENE->Init_Scene();

				ListPointer = 0;
				threadLock = false;
			}

		}

	}

}

#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::init()
{
	// Create Irrklang Engine Device
	SoundEngine = createIrrKlangDevice();


}

void SoundManager::release()
{
	if (SoundEngine)
		SoundEngine->stopAllSounds();

	for (auto& it : mSoundRes)
	{
		if (it.second != nullptr) {

			//if (it.second->resource)
			//	it.second->resource->drop();

			if (it.second->resInfo)
				it.second->resInfo->drop();

			SAFE_DEL(it.second);
		}
	}

	if (SoundEngine)	
		SoundEngine->drop();
}

void SoundManager::InsertSoundFile(string key, const char * _path)
{
	if (SoundEngine)
	{
		if (!mSoundRes.count(key))
		{
			SoundResource* add = new SoundResource();
			add->resource = SoundEngine->addSoundSourceFromFile(_path, ESM_NO_STREAMING);

			// 1 .Tokenize Sound Resource Type
			string path = _path;
			if (path.find_last_of(".") != string::npos)
			{
				path.erase(0, path.find_last_of(".")+1);

				if (path.find("mp3") != string::npos)
					add->type = SOUND_MP3;

				else if (path.find("wav") != string::npos)
					add->type = SOUND_WAV;
			}

			mSoundRes.insert(make_pair(key, add));
			mSoundRes[key]->resInfo = SoundEngine->play2D(mSoundRes[key]->resource, false, false, true);
		}
	}
}

void SoundManager::setVolum()
{
	if (KEYMANAGER->isKeyStayDown(VK_UP))
	{
		float curVol = SoundEngine->getSoundVolume();
		if (curVol < 1.0f)
			SoundEngine->setSoundVolume(curVol + 0.01f);

		mSoundRes.begin()->second->resInfo->setVolume(curVol);
	}

	if (KEYMANAGER->isKeyStayDown(VK_DOWN))
	{
		float curVol = SoundEngine->getSoundVolume();
		if (curVol > 0.0f)
			SoundEngine->setSoundVolume(curVol - 0.01f);

		mSoundRes.begin()->second->resInfo->setVolume(curVol);
	}
}

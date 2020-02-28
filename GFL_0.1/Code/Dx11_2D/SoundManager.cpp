#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
	this->release();
}

void SoundManager::init()
{
	// Create Irrklang Engine Device
	SoundEngine = createIrrKlangDevice();

	mChannel.insert(make_pair(CH_EFFECT, new SoundChannel()));
	mChannel.insert(make_pair(CH_VOICE, new SoundChannel()));
	mChannel.insert(make_pair(CH_SOUND1, new SoundChannel()));
	mChannel.insert(make_pair(CH_SOUND2, new SoundChannel()));
	mChannel.insert(make_pair(CH_SOUND3, new SoundChannel()));

	for (auto& it : mChannel)
		it.second->engine = createIrrKlangDevice();

	effectChannel.reserve(100);

}

void SoundManager::release()
{
	if (SoundEngine)
		SoundEngine->stopAllSounds();

	for (auto& it : mChannel)
	{
		if (it.second != nullptr)
			SAFE_DEL(it.second);
	}

	for (auto& it : mSoundRes)
	{
		if (it.second != nullptr)
			SAFE_DEL(it.second);
	}

	if (SoundEngine)
		SoundEngine->drop();
}

void SoundManager::update()
{
	for (auto& channel : mChannel)
	{
		if (channel.first == CH_VOICE)continue;

		else if (channel.first == CH_EFFECT)
		{
			for (size_t i = effectChannel.size() - 1; i < -1; --i)
			{
				if (effectChannel[i]->isFinished())
					effectChannel.erase(effectChannel.begin() + i);
				else
				{
					auto& soundPlay = effectChannel[i];
					soundPlay->setPlaybackSpeed(DeltaAcl);
				}
			}
		}

		else
		{
			for (auto& soundPlay : channel.second->playList)
				soundPlay.second->setPlaybackSpeed(DeltaAcl);
		}
	}
}

void SoundManager::InsertSoundBianry(string key, string _path)
{
	if (SoundEngine)
	{
		if (!mSoundRes.count(key))
		{
			SoundResource* add = new SoundResource();

			auto THREADPOOL = LOADMANAGER->pGetThreadPool();

			THREADPOOL->ClearBeforeStart();

			FILE* f = fopen(_path.c_str(), "rb");
			string fileName;
			int count = 0;
			fseek(f, 0, SEEK_END);
			int size = ftell(f) - 32;
			fseek(f, 0, SEEK_SET);

			THREADPOOL->ResizeBeforeStart(size);

			//Tokenize File Name
			{
				fileName = _path;

				if (fileName.find_last_of("/") != string::npos)
					fileName.erase(0, fileName.find_last_of("/") + 1);


				if (fileName.find_last_of("\\") != string::npos)
					fileName.erase(0, fileName.find_last_of("\\") + 1);


				if (fileName.find_last_of(".") != string::npos)
					fileName.erase(fileName.find_last_of("."), fileName.size());
			}

			if (NULL != f)
			{
				bool hashing_Vec = false;
				BYTE buffer = 0;

				//InitVector Insert
				fread(&THREADPOOL->InitVec[0], sizeof(BYTE), 32, f);

				// 512 Byte 분할 읽기
				int i = 0;
				while (true)
				{
					fread(&THREADPOOL->CipByte[i], sizeof(BYTE), 512, f);
					i += 512;

					if (i > size - 1)
						break;
				}

				for (auto& it : THREADPOOL->InitVec)
				{
					it ^= fileName[fileName.size() - 1];
					it ^= fileName[0];
				}

			}
			fclose(f);

			int onceSize = THREADPOOL->CipByte.size() * 0.03125f;
			int leftSize = THREADPOOL->CipByte.size() - (onceSize * 32);
			int lastIndex = onceSize * 32;

			THREADPOOL->getFileHeight() = onceSize;
			THREADPOOL->Cluck_AllThread();

			while (!THREADPOOL->isAllOver())
				this_thread::yield();

			for (int i = 0; i < leftSize; ++i)
			{
				BYTE value = THREADPOOL->CipByte[lastIndex + i] ^ THREADPOOL->InitVec[(lastIndex + i) % 32];
				value ^= 255;
				THREADPOOL->OrigByte[lastIndex + i] = value;
			}

			locker.lock();
			add->memory = &THREADPOOL->OrigByte[0];
			add->fileSize = THREADPOOL->OrigByte.size();

			add->resource = SoundEngine->addSoundSourceFromMemory(add->memory, add->fileSize, key.c_str());
			add->resource->setStreamMode(E_STREAM_MODE::ESM_STREAMING);

			// 1 .Tokenize Sound Resource Type
			string path = _path;
			if (path.find_last_of(".") != string::npos)
			{
				path.erase(0, path.find_last_of(".") + 1);

				if (path.find("mp3") != string::npos)
					add->type = SOUND_MP3;

				else if (path.find("wav") != string::npos)
					add->type = SOUND_WAV;

				else if (path.find("flac") != string::npos)
					add->type = SOUND_FLAC;
			}

			mSoundRes.insert(make_pair(key, add));
			locker.unlock();
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

		//mSoundRes.begin()->second->resInfo->setVolume(curVol);
	}

	if (KEYMANAGER->isKeyStayDown(VK_DOWN))
	{
		float curVol = SoundEngine->getSoundVolume();
		if (curVol > 0.0f)
			SoundEngine->setSoundVolume(curVol - 0.01f);

		//mSoundRes.begin()->second->resInfo->setVolume(curVol);
	}
}

void SoundManager::setVolume(SOUND_CHANNEL ch, float volume)
{
	if (volume < 0.0)
		volume = 0;

	mChannel[ch]->engine->setSoundVolume(volume);
}

void SoundManager::setVolume(string key, SOUND_CHANNEL ch, float volume)
{
	if (volume < 0.0)
		volume = 0;

	if ((miSoundRes = mSoundRes.find(key)) != mSoundRes.end())
		mChannel[ch]->playList[key]->setVolume(volume);
}

void SoundManager::Play_Effect(SOUND_CHANNEL ch, string key, float volume)
{
	if ((miSoundRes = mSoundRes.find(key)) != mSoundRes.end())
	{
		miSoundRes->second->resource->setDefaultVolume(volume);

		ISound* _new = mChannel[ch]->engine->play2D(miSoundRes->second->resource, false, true, true);

		_new->setVolume(volume);
		_new->setIsPaused(false);
		_new->setPlaybackSpeed();

		if (ch != CH_VOICE)
			effectChannel.push_back(_new);
	}

	//if ((miSoundRes = mSoundRes.find(key)) != mSoundRes.end())
	//{
	//	miSoundRes->second->resource->setDefaultVolume(volume);

	//	//if (mChannel[ch]->playList.count(key))
	//	//	mChannel[ch]->playList.erase(key);

	//	mChannel[ch]->playList.insert(make_pair(key, mChannel[ch]->engine->play2D(miSoundRes->second->resource, false, true, true)));
	//	

	//	auto& cur = mChannel[ch]->playList[key];

	//	cur->setVolume(volume);
	//	cur->setPlaybackSpeed(1.0f);
	//	//int i = cur->getPlayPosition();
	//	cur->setIsPaused(false);
	//}
}

void SoundManager::Play_Effect(SOUND_CHANNEL ch, string key, float volume, float playSpeed)
{
	if ((miSoundRes = mSoundRes.find(key)) != mSoundRes.end())
	{
		miSoundRes->second->resource->setDefaultVolume(volume);

		ISound* _new = mChannel[ch]->engine->play2D(miSoundRes->second->resource, false, true, true);

		_new->setVolume(volume);
		_new->setIsPaused(false);
		_new->setPlaybackSpeed();

		if (ch != CH_VOICE)
			effectChannel.push_back(_new);
	}

	/*if ((miSoundRes = mSoundRes.find(key)) != mSoundRes.end())
	{
		miSoundRes->second->resource->setDefaultVolume(volume);
		mChannel[ch]->playList.insert(make_pair(key, mChannel[ch]->engine->play2D(miSoundRes->second->resource, false, true, true)));

		auto& cur = mChannel[ch]->playList[key];

		cur->setVolume(volume);
		cur->setPlaybackSpeed(playSpeed);
		cur->setIsPaused(false);
	}*/
}

void SoundManager::Play_Effect2(SOUND_CHANNEL ch, string key, float volume)
{
	if ((miSoundRes = mSoundRes.find(key)) != mSoundRes.end())
	{
		miSoundRes->second->resource->setDefaultVolume(volume);

		ISound* _new = mChannel[ch]->engine->play2D(miSoundRes->second->resource, false, true, true);

		_new->setVolume(volume);
		_new->setIsPaused(false);
		_new->setPlaybackSpeed();

		if (ch != CH_VOICE)
			effectChannel.push_back(_new);
	} 
}
 
void SoundManager::Play_Sound(SOUND_CHANNEL ch, string key, float volume)
{
	if ((miSoundRes = mSoundRes.find(key)) != mSoundRes.end())
	{
		miSoundRes->second->resource->setDefaultVolume(volume);

		if (mChannel[ch]->playList.count(key))
		{
			if (mChannel[ch]->playList[key]->isFinished())
			{
				mChannel[ch]->playList[key] = mChannel[ch]->engine->play2D(miSoundRes->second->resource, false, true, true, true);
				auto& cur = mChannel[ch]->playList[key];

				//cur->setVolume(0.0);
				cur->setIsPaused(false);
			}

		}

		else
		{
			miSoundRes->second->resource->setDefaultVolume(volume);
			mChannel[ch]->playList.insert(make_pair(key, mChannel[ch]->engine->play2D(miSoundRes->second->resource, false, true, true, true)));

			auto& cur = mChannel[ch]->playList[key];

			//cur->setVolume(0.0);
			cur->setIsPaused(false);
		}

		/*if (!mChannel[ch]->engine->isCurrentlyPlaying(key.c_str()))
		{
			mChannel[ch]->engine->play2D(miSoundRes->second->resource);

			miSoundRes->second->resource->setDefaultVolume(volume);
			mChannel[ch]->playList.insert(make_pair(key, mChannel[ch]->engine->play2D(miSoundRes->second->resource, false, true, true)));

			auto& cur = mChannel[ch]->playList[key];

			cur->setVolume(volume);
			cur->setIsPaused(false);
		}*/

		/*miSoundRes->second->resource->setDefaultVolume(volume);
		mChannel[ch]->playList.push_back(mChannel[ch]->engine->play2D(miSoundRes->second->resource, false, true, true));

		auto& cur = mChannel[ch]->playList[mChannel[ch]->playList.size() - 1];

		cur->setVolume(volume);
		cur->setIsPaused(false);*/
	}
}

void SoundManager::Stop_Sound(SOUND_CHANNEL ch, string key)
{
	if ((miSoundRes = mSoundRes.find(key)) != mSoundRes.end())
	{
		if (mChannel[ch]->engine->isCurrentlyPlaying(key.c_str()))
			mChannel[ch]->engine->stopAllSounds();
	}
}

bool SoundManager::isPlay(SOUND_CHANNEL ch, string key)
{
	if ((miSoundRes = mSoundRes.find(key)) != mSoundRes.end())
		return mChannel[ch]->engine->isCurrentlyPlaying(key.c_str());	
	return false;
}

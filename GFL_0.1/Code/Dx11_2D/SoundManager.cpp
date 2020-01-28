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

			{
				//FILE* f = fopen(_path.c_str(), "rb");
				//vector<char> vBuffer;
				//vector<BYTE> vOrig;
				//vector<BYTE> InitVec;

				//string fileName;
				//int count = 0;
				//fseek(f, 0, SEEK_END);
				//int size = ftell(f) - 32;
				//fseek(f, 0, SEEK_SET);

				//InitVec.resize(32, 0);
				//vOrig.resize(size, 0);
				//vBuffer.resize(size, 0);

				////Tokenize File Name
				//{
				//	fileName = _path;

				//	if (fileName.find_last_of("/") != string::npos)
				//		fileName.erase(0, fileName.find_last_of("/") + 1);


				//	if (fileName.find_last_of("\\") != string::npos)
				//		fileName.erase(0, fileName.find_last_of("\\") + 1);


				//	if (fileName.find_last_of(".") != string::npos)
				//		fileName.erase(fileName.find_last_of("."), fileName.size());
				//}

				//if (NULL != f)
				//{
				//	bool hashing_Vec = false;
				//	BYTE buffer = 0;

				//	//InitVector Insert
				//	fread(&InitVec[0], sizeof(BYTE), 32, f);
				//	//fread(&CipByte[0], sizeof(BYTE), size, f);

				//	// 512 Byte 분할 읽기
				//	int i = 0;
				//	while (true)
				//	{
				//		fread(&vOrig[i], sizeof(BYTE), 512, f);
				//		i += 512;

				//		if (i > size - 1)
				//			break;
				//	}

				//	for (auto& it : InitVec)
				//	{
				//		it ^= fileName[fileName.size() - 1];
				//		it ^= fileName[0];
				//	}

				//}
				//fclose(f);

				////CipByte.erase(CipByte.begin() + CipByte.size() - 1);

				//int onceSize = vOrig.size() * 0.03125f;
				//int leftSize = vOrig.size() - (onceSize * 32);
				//int lastIndex = onceSize * 32;
				//int ThreadNum = 8;
				//mutex mtx;

				////	Eight Thread Running
				//for (int i = 0; i < 8;)
				//{
				//	thread trd([&, i]()
				//	{
				//		mtx.lock();

				//		int startP = i * 4;
				//		int lineNum = 0;
				//		int height = onceSize;
				//		BYTE value = 0;

				//		mtx.unlock();

				//		while (true)
				//		{
				//			// 0
				//			value = vOrig[startP + (lineNum * 32)] ^ InitVec[startP];
				//			value ^= 255;
				//			vBuffer[startP + (lineNum * 32)] = static_cast<char>(value);
				//			InitVec[startP] = value;

				//			// 1
				//			value = vOrig[startP + (lineNum * 32) + 1] ^ InitVec[startP + 1];
				//			value ^= 255;
				//			vBuffer[startP + (lineNum * 32) + 1] = static_cast<char>(value);
				//			InitVec[startP + 1] = value;

				//			// 2
				//			value = vOrig[startP + (lineNum * 32) + 2] ^ InitVec[startP + 2];
				//			value ^= 255;
				//			vBuffer[startP + (lineNum * 32) + 2] = static_cast<char>(value);
				//			InitVec[startP + 2] = value;

				//			// 3
				//			value = vOrig[startP + (lineNum * 32) + 3] ^ InitVec[startP + 3];
				//			value ^= 255;
				//			vBuffer[startP + (lineNum * 32) + 3] = static_cast<char>(value);
				//			InitVec[startP + 3] = value;

				//			++lineNum;

				//			if (lineNum == height)
				//				break;
				//		}

				//		mtx.lock();
				//		--ThreadNum;
				//		mtx.unlock();

				//	});

				//	mtx.lock();

				//	trd.detach();
				//	++i;
				//	mtx.unlock();
				//}

				//while (ThreadNum > 0)
				//	this_thread::yield();

				/*for (int i = 0; i < leftSize; ++i)
				{
					BYTE value = vOrig[lastIndex + i] ^ InitVec[(lastIndex + i) % 32];
					value ^= 255;
					vBuffer[lastIndex + i] = static_cast<char>(value);
				}*/
			}

			locker.lock();
			add->memory = &THREADPOOL->OrigByte[0];
			add->fileSize = THREADPOOL->OrigByte.size();

			add->resource = SoundEngine->addSoundSourceFromMemory(add->memory, add->fileSize, key.c_str());


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
	mChannel[ch]->engine->setSoundVolume(volume);

}

void SoundManager::setVolume(string key, float volume)
{
	if ((miSoundRes = mSoundRes.find(key)) != mSoundRes.end())
		miSoundRes->second->resource->setDefaultVolume(volume);
}

void SoundManager::Play_Effect(SOUND_CHANNEL ch, string key, float volume)
{
	if ((miSoundRes = mSoundRes.find(key)) != mSoundRes.end())
	{
		miSoundRes->second->resource->setDefaultVolume(volume);
		mChannel[ch]->playList.push_back(mChannel[ch]->engine->play2D(miSoundRes->second->resource, false, true, true));

		auto& cur = mChannel[ch]->playList[mChannel[ch]->playList.size() - 1];

		cur->setVolume(volume);
		cur->setPlaybackSpeed(1.0f);
		cur->setIsPaused(false);
	}
}

void SoundManager::Play_Effect(SOUND_CHANNEL ch, string key, float volume, float playSpeed)
{
	if ((miSoundRes = mSoundRes.find(key)) != mSoundRes.end())
	{
		miSoundRes->second->resource->setDefaultVolume(volume);
		mChannel[ch]->playList.push_back(mChannel[ch]->engine->play2D(miSoundRes->second->resource, false, true, true));

		auto& cur = mChannel[ch]->playList[mChannel[ch]->playList.size() - 1];

		cur->setVolume(volume);
		cur->setPlaybackSpeed(playSpeed);
		cur->setIsPaused(false);
	}
}

void SoundManager::Play_Sound(SOUND_CHANNEL ch, string key, float volume)
{
	if ((miSoundRes = mSoundRes.find(key)) != mSoundRes.end())
	{
		miSoundRes->second->resource->setDefaultVolume(volume);

		if (!mChannel[ch]->engine->isCurrentlyPlaying(key.c_str()))
		{
			mChannel[ch]->engine->play2D(miSoundRes->second->resource);
		}

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

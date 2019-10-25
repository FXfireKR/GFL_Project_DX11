#pragma once
#include "singleton.h"

class SoundManager : public singleton<SoundManager>
{
private:
	enum SOUNDRES_TYPE
	{
		SOUND_MP3 = 0,
		SOUND_WAV,
		SOUND_FLAC
	};

	struct SoundResource 
	{
		SOUNDRES_TYPE type;
		string name;
		ISoundSource* resource;
		ISound* resInfo;
	};

	struct SoundChannel
	{
		//ISound* cur
	};

private:
	map<string, SoundResource*> mSoundRes;
	//map<string, >

private:
	ISoundEngine* SoundEngine;

public:
	SoundManager();
	~SoundManager();

	void init();
	void release();

	void InsertSoundFile(__in string key, __in const char* _path);
	void setVolum();
};
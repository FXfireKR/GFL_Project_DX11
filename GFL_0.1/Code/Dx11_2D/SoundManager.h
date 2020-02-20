#pragma once
#include "singleton.h"

#include <thread>
#include <mutex>


enum SOUND_CHANNEL
{
	CH_EFFECT = 0,
	CH_VOICE,
	CH_SOUND1,
	CH_SOUND2,
	CH_SOUND3
};

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
		LPVOID memory;
		ik_s32 fileSize;
	};

	struct SoundChannel
	{
		ISoundEngine* engine;
		//vector<ISound*> playList;
		map<string, ISound*> playList;

		SoundChannel() {
			engine = createIrrKlangDevice();
		}
	};

private:
	map<string, SoundResource*> mSoundRes;
	map<string, SoundResource*>::iterator miSoundRes;

	map<SOUND_CHANNEL, SoundChannel*> mChannel;

	vector<ISound*> effectChannel;

private:
	ISoundEngine* SoundEngine;

public:
	SoundManager();
	~SoundManager();

	void init();
	void release();

	void update();

	void InsertSoundBianry(__in string key, __in string _path);
	void setVolum();
	void setVolume(__in SOUND_CHANNEL ch, __in float volume);
	void setVolume(__in string key, __in SOUND_CHANNEL ch, __in float volume);

	void Play_Effect(__in SOUND_CHANNEL ch, __in string key, __in float volume = 0.5f);
	void Play_Effect(__in SOUND_CHANNEL ch, __in string key, __in float volume, float playSpeed);
	void Play_Effect2(__in SOUND_CHANNEL ch, __in string key, __in float volume);

	void Play_Sound(__in SOUND_CHANNEL ch, __in string key, __in float volume = 0.5f);
	void Stop_Sound(__in SOUND_CHANNEL ch, __in string key);


	bool isPlay(__in SOUND_CHANNEL ch, __in string key);
	inline bool isValidKey(__in string key) 
	{ 
		return !mSoundRes.count(key);
	}
};
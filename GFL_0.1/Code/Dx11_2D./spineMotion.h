#pragma once
#include "spineBase.h"

typedef void(*MotionAct)(void* obj);

class spineMotion
{
private:
	const string SPINE_BASE_PATH = "../../_Assets/Characters/";

private:
	spAtlas*				atlas;
	spineBase*				base;
	spSkeleton*				skeleton;
	spSkeletonData*			skeletonData;

	string					imgPath;
	string					curMotion;
	string					pngKey;

	map<string, size_t>		mAniList;
	
	map<string, MotionAct>	mMotionActFnc;
	map<string, MotionAct>::iterator callMotionAct;
	void*					pObject;

public:
	spineMotion();
	~spineMotion();

	static void loadImage_FromFile(__in const char* _path);

	void loadSpine_FromJsonFile(__in const char* _path);
	void setMotionAction(string _key, void* _motionAct);
	void setMotionObject(void* _obj);
	void init();

	void pause();
	void pause(double _timer);
	void update(float delta);
	void render();

	
	void changeMotion(string _aniKey, bool loop = false, bool _mix = false, float _dur = 0.1f);
	void reload_SpineImage();

private:
	void changeMotion_mix_byName(string _aniKey, bool loop, float _duration);

public:

	inline const float getCurTime() const { return base->getTrackEntry()->time; }
	inline const float getEndTime() const { return base->getTrackEntry()->endTime; }
	inline const float getMixTime() const { return base->getTrackEntry()->mixTime; }

	inline bool isCurrent(string _key) { return curMotion.compare(_key) ? false : true; }
	inline const string getCurrentMotionKey() { return curMotion; }

	inline D3DXVECTOR3& p_getScale() { return base->p_getScale(); }
	inline const D3DXVECTOR3 getScale() { return base->getScale(); }

	inline D3DXVECTOR3& p_getRotate() { return base->p_getRotate(); }
	inline const D3DXVECTOR3 getRotate() { return base->getRotate(); }

	inline D3DXVECTOR3& p_getTrans() { return base->p_getTrans(); }
	inline const D3DXVECTOR3 getTrans() { return base->getTrans(); }
};
#pragma once
#include "Motion.h"

class MotionManager
{
private:
	string crtMotion;		//현재 모션
	string bfrMotion;		//방금 모션

	map<string, Motion*> mMotion;	//모션~	모든 기체에 static형으로 보유해야함. (여러대의 기체 생산시 메모리 낭비없게)

public:
	MotionManager();
	~MotionManager();

	HRESULT Create(string motionKey, string key, FLOAT _pWidth, FLOAT _pHeight, FLOAT fps, bool rev = false);
	HRESULT RegisterFunction(string key, void* fnc, void* ob);

	void Change_Motion(string key);

	void release();

	void update();
	void pause();
	void render(__in FLOAT x, __in FLOAT y);
	void render(__in FLOAT x, __in FLOAT y, __in FLOAT sx, __in FLOAT sy);
	void render(__in FLOAT x, __in FLOAT y, __in FLOAT sx, __in FLOAT sy, __in FLOAT alpha);
	void render(__in D3DXVECTOR2 t, __in D3DXVECTOR2 s, __in FLOAT alpha, __in FLOAT dgr);
	void render(__in D3DXVECTOR2 t, __in D3DXVECTOR2 s, __in D3DXCOLOR color, __in FLOAT dgr);

public:
	inline int getMotionMapSize() { return mMotion.size(); }

	inline Motion* getMotion(string key) { return mMotion.count(key) ? mMotion.find(key)->second : nullptr; }
	inline Motion* getCurrentMotion() { return mMotion.count(crtMotion) ? mMotion.find(crtMotion)->second : nullptr; }

	inline string getCurrentKey() { return crtMotion; }
	inline string getBeforeKey() { return bfrMotion; }

	//inline string getCurrentPath() { if (mMotion.count(crtMotion))return mMotion.find(crtMotion)->second->getPath(); }

	inline FMPT getCurrent_Frame() { if (mMotion.count(crtMotion))return mMotion.find(crtMotion)->second->getFrame(); }
	inline UINT getCurrent_FrameX() { return mMotion.count(crtMotion) ? mMotion.find(crtMotion)->second->getFrameX() : 0; }
	inline UINT getCurrent_FrameY() { return mMotion.count(crtMotion) ? mMotion.find(crtMotion)->second->getFrameY() : 0; }

	//inline FMPT getCurrent_maxFrame() { if (mMotion.count(crtMotion)) return mMotion.find(crtMotion)->second->getFrameMax(); }
	//inline UINT getCurrent_maxFrameX() { return mMotion.count(crtMotion) ? mMotion.find(crtMotion)->second->getFrameMaxX() : 0; }
	//inline UINT getCurrent_maxFrameY() { return mMotion.count(crtMotion) ? mMotion.find(crtMotion)->second->getFrameMaxY() : 0; }

	inline UINT getCurrent_Counter() { return mMotion.count(crtMotion) ? mMotion.find(crtMotion)->second->getCounter() : 0; }

	inline UINT getCurrent_FramePerSec() { return mMotion.count(crtMotion) ? mMotion.find(crtMotion)->second->getCounter() : 0; }

	inline bool isCurrent(string key) { return crtMotion.compare(key) ? false : true; }
};
#include "stdafx.h"
#include "spineMotion.h"

spineMotion::spineMotion()
{
	atlas = nullptr;
	base = nullptr;
	skeleton = nullptr;
	skeletonData = nullptr;
	curAnimation = nullptr;

	curMotion = "";
}

spineMotion::~spineMotion()
{
	spAtlas_dispose(atlas);
	spSkeletonData_dispose(skeletonData);
	SAFE_DELETE(base);

	skeleton = nullptr;
}

void spineMotion::loadImage_FromFile(const char * _path)
{
	string imgPath; imgPath = "Characters/";
	imgPath += _path; imgPath += "/"; imgPath += _path; imgPath += ".ab";
	string pngKey = _path;
	LOAD->Add_LoadTray(pngKey, imgPath.c_str(), RESOURCE_IMAGE);
}

void spineMotion::loadSpine_FromJsonFile(const char * _path)
{
	string atlasPath = SPINE_OTHER_PATH;
	string jsonPath = SPINE_OTHER_PATH;
	imgPath = SPINE_BASE_PATH;

	atlasPath += _path; atlasPath += "/"; atlasPath += _path; atlasPath += ".atlas";
	jsonPath += _path; jsonPath += "/"; jsonPath += _path; jsonPath += ".json";
	imgPath += _path; imgPath += "/"; imgPath += _path; imgPath += ".ab";
	pngKey = _path;

	//	image Load Tray Request
	if (IMAGEMAP->isValidKey(pngKey))
		LOAD->Add_LoadTray(pngKey, imgPath.c_str(), RESOURCE_IMAGE);

	atlas = spAtlas_createFromFile(atlasPath.c_str(), NULL);

	spSkeletonJson* json = spSkeletonJson_create(atlas);
	json->scale = 1.0f;

  	skeletonData = spSkeletonJson_readSkeletonDataFile(json, jsonPath.c_str());

	spSkeletonJson_dispose(json);

	base = new spineBase(skeletonData);
	base->timeScale = 1;

	base->setShaderResourceView(pngKey);

	//	Setting All Motion's MixTime	0.105
	//base->getStateData()->defaultMix = 0.0f;

	skeleton = base->getSkeleton();
	skeleton->flipX = false;
	skeleton->flipY = false;
	spSkeleton_setSkinByName(skeleton, "default");
	spSkeleton_setSlotsToSetupPose(skeleton);

	skeleton->x = 0;
	skeleton->y = 0;
	spSkeleton_updateWorldTransform(skeleton);

	size_t animationsCount = static_cast<UINT>(base->getSkeleton()->data->animationsCount);
	for (size_t i = 0; i < animationsCount; ++i)
		mMotionActFnc.insert(make_pair(base->getSkeleton()->data->animations[i]->name, nullptr));

}

void spineMotion::setMotionAction(string _key, void * _motionAct)
{
	if ((actIter = mMotionActFnc.find(_key)) != mMotionActFnc.end())
		actIter->second = (MotionAct)_motionAct;
}

void spineMotion::setMotionObject(void * _obj)
{
	pObject = _obj;
}

void spineMotion::init()
{
	base->changeMotion_byName("pseudo_setup_pose", true);
	base->changeMotion_byName("pseudo_setup_pose", true);
	base->changeMotion_byName("pseudo_setup_pose", true);

	base->changeMotion_byName("wait", true);
	curMotion = "wait";
	curAnimation = base->getTrackEntry()->animation;
}

void spineMotion::pause()
{
}

void spineMotion::pauseAt(double _timer)
{
	if (base->getTrackEntry() != nullptr)
		base->getTrackEntry()->time = static_cast<float>(_timer);
}

void spineMotion::pause(double _timer)
{
	//	Mixtime 을 반영해서 pauseTime을 적용합니다.
	if (base->getTrackEntry() != nullptr)
		if (base->getTrackEntry()->time > static_cast<float>(_timer) + base->getTrackEntry()->mixTime)
			base->getTrackEntry()->time = static_cast<float>(_timer) + base->getTrackEntry()->mixTime;
}

void spineMotion::update(float delta)
{
	base->update(delta);

	//함수포인터 실행
	if ((callMotionAct = mMotionActFnc.find(curMotion)) != mMotionActFnc.end())	{
		if (callMotionAct->second != nullptr)
			callMotionAct->second(pObject);
	}
}

void spineMotion::render()
{
	base->render();
}

void spineMotion::changeMotion(string _aniKey, bool loop, bool _mix, float _dur)
{
	if (_mix)
		changeMotion_mix_byName(_aniKey, loop, _dur);

	else
	{
		changeMotion_mix_byName(_aniKey, loop, 0.0f);
		//base->changeMotion_byName(_aniKey, loop);
		base->getState()->data->defaultMix = 0.0f;
		curMotion = _aniKey;
	}
}

void spineMotion::reload_SpineImage()
{
	if (IMAGEMAP->getTexture(pngKey) == nullptr)
	{
		LOAD->Add_LoadTray(pngKey, imgPath.c_str(), LOADRESOURCE_TYPE::RESOURCE_IMAGE);
		base->setShaderResourceView(pngKey);
	}
}

void spineMotion::changeMotion_mix_byName(string _aniKey, bool loop, float _duration)
{
	base->changeMotion_byName(_aniKey, loop);
	spAnimationStateData_setMixByName(base->getStateData(), curMotion.c_str(), _aniKey.c_str(), _duration);
	curMotion = _aniKey;
	curAnimation = base->getTrackEntry()->animation;
}
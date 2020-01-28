#pragma once
#include "Scene.h"

class MainLoadScene : public Scene
{
private:
	const float BGM_VOLUME_MAX = 0.2f;
	const float BGM_VOLUME_START = -1.5f;

	float bgmVolume;
	float bgImageAlpha;
	bool increase;

public:
	MainLoadScene();
	~MainLoadScene();

	// Scene을(를) 통해 상속됨
	void init() override;
	void release() override;
	void update() override;
	void render() override;
};
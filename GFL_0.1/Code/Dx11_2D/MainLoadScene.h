#pragma once
#include "Scene.h"
#include "uiAtlasLoader.h"

class MainLoadScene : public Scene
{
private:
	const float BGM_VOLUME_MAX = 0.25f;
	const float BGM_VOLUME_START = -0.01f;

	const float BUTTON_ALPHA_MIN = 0.4f;
	const float BUTTON_ALPHA_MAX = 0.7f;

	float		BGM_VOLUME_SPEED;
	float		ALPHA_SPEED;

	float		bgmVolume;
	float		bgImageAlpha;
	float		LogoAlpha;
	bool		increase;

public:
	MainLoadScene();
	~MainLoadScene();

	// Scene을(를) 통해 상속됨
	void init() override;
	void release() override;
	void update() override;
	void render() override;
};
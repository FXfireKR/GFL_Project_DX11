#pragma once
#include "Scene.h"

class LoadScene : public Scene
{
private:
	float percent;
	float toolCheckTimer;

public:
	LoadScene();
	~LoadScene();

	// Scene을(를) 통해 상속됨
	void init() override;
	void release() override;
	void update() override;
	void render() override;
};
#pragma once
#include "Scene.h"

class MainLoadScene : public Scene
{
private:
	mutex locker;

public:
	MainLoadScene();
	~MainLoadScene();

	// Scene을(를) 통해 상속됨
	void init() override;
	void release() override;
	void update() override;
	void ImageUpdate();
	void render() override;
};
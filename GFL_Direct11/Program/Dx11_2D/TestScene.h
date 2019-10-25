#pragma once
#include "Scene.h"

class TestScene : public Scene
{
private:


public:
	TestScene();
	~TestScene();

	// Scene을(를) 통해 상속됨
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};


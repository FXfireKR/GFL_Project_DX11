#pragma once
#include "Scene.h"

class TestScene : public Scene
{
private:


public:
	TestScene();
	~TestScene();

	// Scene��(��) ���� ��ӵ�
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};


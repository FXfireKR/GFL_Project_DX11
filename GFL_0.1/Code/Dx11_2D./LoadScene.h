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

	// Scene��(��) ���� ��ӵ�
	void init() override;
	void release() override;
	void update() override;
	void render() override;
};
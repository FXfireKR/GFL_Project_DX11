#pragma once
#include "Scene.h"

class MainLoadScene : public Scene
{
private:
	mutex locker;

public:
	MainLoadScene();
	~MainLoadScene();

	// Scene��(��) ���� ��ӵ�
	void init() override;
	void release() override;
	void update() override;
	void ImageUpdate();
	void render() override;
};
#pragma once
#include "Scene.h"
#include "Image.h"

class MainLoadScene : public Scene
{
private:
	Image* image;
	mutex locker;

public:
	MainLoadScene();
	~MainLoadScene();

	// Scene��(��) ���� ��ӵ�
	HRESULT init() override;
	void release() override;
	void update() override;
	void ImageUpdate();
	void render() override;
};


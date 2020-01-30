#pragma once
#include "Scene.h"

class ChapterScene : public Scene
{
private:
	map<string, Button>		mButton;

public:
	ChapterScene();
	~ChapterScene();

	// Scene��(��) ���� ��ӵ�
	void init() override;
	void release() override;
	void update() override;
	void render() override;
};
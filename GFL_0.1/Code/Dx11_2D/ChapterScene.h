#pragma once
#include "Scene.h"

class ChapterScene : public Scene
{
private:
	map<string, Button>		mButton;

public:
	ChapterScene();
	~ChapterScene();

	// Scene을(를) 통해 상속됨
	void init() override;
	void release() override;
	void update() override;
	void render() override;
};
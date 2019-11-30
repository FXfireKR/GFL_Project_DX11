#pragma once
#include "Scene.h"
#include "Image.h"


#define	ZORDER_REFRESH_TIME		0.25f

class TestScene : public Scene
{
private:
	Image* img;

	map<FLOAT, map<FLOAT, POINT>> mRendList;
	vector<POINT> vRendList;



public:
	TestScene();
	~TestScene();

	// Scene을(를) 통해 상속됨
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;

	void ZOrder_Sort();
};
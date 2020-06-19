#ifndef UNLOADSCENE_H
#define UNLOADSCENE_H
#include "Scene.hpp"

class UnLoadScene : public Scene
{
private:
	float percent;
	float destPercent;
	float toolCheckTimer;

public:
	UnLoadScene();
	~UnLoadScene();

	// Scene을(를) 통해 상속됨
	void init() override;
	void release() override;
	void update() override;
	void render() override;
};
#endif
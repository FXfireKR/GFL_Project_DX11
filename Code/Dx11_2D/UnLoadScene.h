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

	// Scene��(��) ���� ��ӵ�
	void init() override;
	void release() override;
	void update() override;
	void render() override;
};
#endif
#ifndef LOADSCENE_H
#define LOADSCENE_H
#include "Scene.hpp"

class LoadScene : public Scene
{
private:
	float percent;
	float destPercent;
	float toolCheckTimer;

public:
	LoadScene();
	~LoadScene();

	// Scene을(를) 통해 상속됨
	void init() override;
	void release() override;
	void update() override;
	void render() override;
};
#endif
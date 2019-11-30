#pragma once
#include "Scene.h"

class LoadScene : public Scene
{
private:


public:
	LoadScene();
	~LoadScene();

	// Scene을(를) 통해 상속됨
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};
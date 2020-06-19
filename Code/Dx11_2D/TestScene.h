#pragma once
#include "Scene.hpp"

#include "nAnOne.h"

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	virtual void init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

private:
	nAnOne* temp;
};
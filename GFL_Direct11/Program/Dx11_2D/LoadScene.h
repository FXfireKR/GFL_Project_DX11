#pragma once
#include "Scene.h"

class LoadScene : public Scene
{
private:


public:
	LoadScene();
	~LoadScene();

	// Scene��(��) ���� ��ӵ�
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};
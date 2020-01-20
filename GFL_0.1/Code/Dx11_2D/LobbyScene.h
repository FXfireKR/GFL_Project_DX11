#pragma once
#include "Scene.h"

class LobbyScene : public Scene
{
protected:
	map<string, Button>		mButton;

public:
	LobbyScene();
	~LobbyScene();

	// Scene을(를) 통해 상속됨
	void init() override;
	void release() override;
	void update() override;
	void render() override;

private:
	static void EquipButton(void* obj);				//	Goto EquipScene
	static void WorldMapButton(void* obj);			//	Goto [Unknown]Scene
	static void StoryButton(void* obj);
	static void WorldButton(void* obj);
};
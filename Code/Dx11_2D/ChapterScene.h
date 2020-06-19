#pragma once
#include "Scene.hpp"

class ChapterScene : public Scene
{
private:


private:
	map<string, Button>		mButton;

	//	Back Ground Image Slide
	string		curImageKey;
	D3DXVECTOR2	position;			// BackGroudImage Position
	float		moveDist;
	float		moveAngle;			// Dgree Angle
	float		bkAlpha;

protected:
	string		SceneKey;
	bool		isSceneChanged;

public:
	ChapterScene();
	~ChapterScene();

	// Scene을(를) 통해 상속됨
	void init() override;
	void release() override;
	void update() override;
	void render() override;

	void flow_Background_Image();

private:
	static void ReturnLobbyButton(void* obj);
	static void Chapter1Button(void* obj);
	static void Chapter2Button(void* obj);
	static void Chapter3Button(void* obj);
	static void Chapter4Button(void* obj);
	//static void 

};
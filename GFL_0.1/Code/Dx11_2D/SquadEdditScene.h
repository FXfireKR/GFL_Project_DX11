#pragma once
#include "Scene.h"

class SquadEdditScene : public Scene
{
private:
	const float ALL_BOX_WIDTH = 140.0f;
	const float ALL_BOX_HEIGH = 260.0f;

	const float ALL_BOX_H_WIDTH = 70.0f;
	const float ALL_BOX_H_HEIGH = 130.0f;

	const float SQUAD_BOX_WIDTH = 180.0f;
	const float SQUAD_BOX_HEIGHT = 456.0f;

	const float SQUAD_BOX_H_WIDTH = SQUAD_BOX_WIDTH * 0.5f;
	const float SQUAD_BOX_H_HEIGHT = SQUAD_BOX_HEIGHT * 0.5f;

protected:
	enum SHOWMODE
	{
		ALL = 0,
		SQUAD,
	};

protected:
	SHOWMODE mode;

	int FocusSquad;		//	���õ� �д�
	int FocusBox;		//	���õ� �ڽ�

	bool showEquip;

	map<string, Button>				mButton;
	vector<string>					vCharacter;
	vector<RECT>					vBox;
	vector<LoadResourceData>		vLoadList;


	bool changeScene;

public:
	SquadEdditScene();
	~SquadEdditScene();

	// Scene��(��) ���� ��ӵ�
	virtual void init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;


private:
	void Squad_Update();
	void Squad_Render();

	void Squad_render_Character();

	void All_Update();
	void All_Render();

	void KeyInputAction();
	void Allocate_Box();
	void Allocate_Box_All();

private:
	static void InsertSelect(void* obj);
	static void DeleteSelect(void* obj);

	static void ChangeSquad_Select(void* obj);
	static void ReturnBase_Select(void* obj);
};
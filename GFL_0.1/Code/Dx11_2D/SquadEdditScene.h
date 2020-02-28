#pragma once
#include "Scene.h"

class SquadEdditScene : public Scene
{
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

	map<string, Button>		mButton;
	vector<string>			vCharacter;
	vector<RECT>			vBox;

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

	void All_Update();
	void All_Render();

	void KeyInputAction();
	void Allocate_Box();
	void Allocate_Box_All();

private:
	static void InsertSelect(void* obj);
	static void DeleteSelect(void* obj);
	static void LeftSelect(void* obj);
	static void RightSelect(void* obj);

	static void ChangeSquad_Select(void* obj);
	static void ReturnBase_Select(void* obj);
};
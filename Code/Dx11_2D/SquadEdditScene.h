#pragma once
#include "Scene.hpp"

class SquadEdditScene : public Scene
{
public:
	SquadEdditScene();
	~SquadEdditScene();

	// Scene을(를) 통해 상속됨
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
	void Allocate_Box_All();

private:
	static void InsertSelect(void* obj);
	static void DeleteSelect(void* obj);

	static void ChangeSquad_Select(void* obj);
	static void ReturnBase_Select(void* obj);

private:
	const int WIDTH_COUNT = 5;

	const float CHARACTER_BOX_WID = 180.0f;
	const float CHARACTER_BOX_HEI = 350.0f;

	const float STATUS_RENDER_X = 1100.0f;
	const float STATUS_RENDER_Y = WINSIZEY * 0.5f + 25;

	const float CHARACTER_BLANK_WID = CHARACTER_BOX_WID + 50.0f;
	const float CHARACTER_BLANK_HEI = CHARACTER_BOX_HEI + 50.0f;

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

	enum class SBUTTONS {
		HOME_BACK = -1,
		TURN_BACK = 0,
		SELECT_SQUAD_1,
		SELECT_SQUAD_2,
		SELECT_SQUAD_3,
		SELECT_CHARA_1,
		SELECT_CHARA_2,
		SELECT_CHARA_3,
		SELECT_CHARA_4,
		SELECT_CHARA_5,
		END
	};

	struct selectBox {
		void*		adress;
		D2D_RECT_F	box;
		D3DXVECTOR2	pos;
	};

protected:
	SHOWMODE mode;
	const uiAtlas* atlas;

	int FocusSquad;		//	선택된 분대
	int FocusBox;		//	선택된 박스

	float						virtualHeight;
	float						virtualLimit;
	int							asixVirtual;
	bool						mouseDrag;

	bool showEquip;

	unordered_map<SBUTTONS, Button>		mButton;
	vector<string>						vCharacter;
	vector<selectBox>					vBox;
	vector<LoadResourceData>			vLoadList;

	bool changeScene;
};
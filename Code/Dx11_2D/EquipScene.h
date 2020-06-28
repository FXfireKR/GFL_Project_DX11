#pragma once
#include "Scene.hpp"

#define CHARA_SLIDER_LIMIT_MAX_X	300.0F
#define CHARA_SLIDER_LIMIT_MIN_X	-300.0F

#define CHARA_SLIDER_SPEED			400.0F

class EquipScene : public Scene
{
public:
	EquipScene();
	~EquipScene();

	// Scene을(를) 통해 상속됨
	void init() override;
	void release() override;
	void update() override;
	void render() override;

private:
	void State_MainUpdate();
	void State_MainRender();

	void State_EquipUpdate();
	void State_EquipRender();

	void State_CharacterUpdate();
	void State_CharacterRender();


private:
	static void CharacterSelect(void* obj);
	static void EquipmentSelect(void* obj);
	static void ReturnSelect(void* obj);
	static void ReturnHome(void* obj);

protected:
	enum EQUIPSCENE_STATE
	{
		ES_MAIN = 0,
		ES_CHARA,
		ES_EQUIP
	};

	enum class SBUTTONS {
		HOME_BACK = -1,
		TURN_BACK = 0,
		SELECT_CHARA,
		SELECT_EQUIP_1,
		SELECT_EQUIP_2,
		SELECT_EQUIP_3,
		SELECT_END
	};

	struct selectBox {
		void*		adress;
		D2D_RECT_F	box;
		D3DXVECTOR2	pos;
	};

protected:
	int EquipNum;
	bool sceneChange;

	static EquipScene*			objectPtr;

	EQUIPTYPE					SelectedEQ_Type;
	EQUIPTOTAL_TYPE				SelectedEQA_Type;

	BaseTaticDoll*				selectedDoll;
	SINT						selectedDollID;

	float						virtualHeight;
	float						virtualLimit;
	int							asixVirtual;
	bool						mouseDrag;

	EQUIPSCENE_STATE	 		state;
	map<SBUTTONS, Button>		mButton;
	vector<selectBox>			selBox;

protected:
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
};
#pragma once
#include "Scene.h"

#define CHARA_SLIDER_LIMIT_MAX_X	300.0F
#define CHARA_SLIDER_LIMIT_MIN_X	-300.0F

#define CHARA_SLIDER_SPEED			400.0F

class EquipScene : public Scene
{
protected:
	enum EQUIPSCENE_STATE
	{
		ES_MAIN = 0,
		ES_CHARA,
		ES_EQUIP
	};

protected:
	//typedef void(*ButtonPointer)(void* obj);

	struct CharacterSlider
	{
		D3DXVECTOR2		axisPos;
		FLOAT			Opacity;		//	�����̴��� ��ü Alpha Value
		int				InfoDollID;		//	����Ϸ��� ���������� ID
		bool			isOpen;			//	���������� ����������
		bool			isMoving;		//	�����̴������� �ƴ���
	};

protected:
	CharacterSlider			CharaSlider;		//ĳ���� �����̴� ����
	D2D_RECT_F				CharaInfoBox;		//ĳ���� ��������â

	EQUIPTYPE				SelectedEQ_Type;
	EQUIPTOTAL_TYPE			SelectedEQA_Type;

	int EquipNum;

	bool sceneChange;

	D3DXVECTOR2				virtualCamera;

	EQUIPSCENE_STATE	 	state;
	map<string, Button>		mButton;

public:
	EquipScene();
	~EquipScene();

	// Scene��(��) ���� ��ӵ�
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
	static void SlideOpener(void* obj);
	static void CharacterSelect(void* obj);
	static void EquipmentSelect(void* obj);
	static void ReturnSelect(void* obj);
};
#ifndef LOBBYSCENE_H
#define LOBBYSCENE_H

class LobbyScene : public Scene
{
public:
	LobbyScene();
	virtual ~LobbyScene();

	virtual void init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

	void updateNone();
	void renderNone();
	void updateNormal();
	void renderNormal();
	void updateAide();
	void renderAide();
	void updateSetting();
	void renderSetting();
	void updateDevlopers();
	void renderDevlopers();

private:
	static void CombatButton(void* obj);
	static void FactoryButton(void* obj);
	static void FormationButton(void* obj);
	static void ReserchButton(void* obj);
	static void AideConverButton(void* obj);

	static void ChangeAideButton(void* obj);
	static void SettingButton(void* obj);
	static void ViewButton(void* obj);
	static void MakerButton(void* obj);

	static void ReturnSelect(void* obj);

private:
	const D3DXVECTOR2 AXIS_COMBAT_BUTTON = D3DXVECTOR2(950, 500);
	const D3DXVECTOR2 AXIS_FACTORY_BUTTON = D3DXVECTOR2(1150, 400);
	const D3DXVECTOR2 AXIS_RESEARCH_BUTTON = D3DXVECTOR2(950, 400);
	const D3DXVECTOR2 AXIS_FORMATION_BUTTON = D3DXVECTOR2(1150, 500);
	const D3DXVECTOR2 AXIS_AIDECOV_BUTTON = D3DXVECTOR2(WINSIZEX*0.4f, WINSIZEY*0.8f);

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
	enum class STATE {
		NONE = -1,
		NORMAL = 0,
		AIDE,
		SETTING,
		DEVLOPERS
	};

	enum class SBUTTONS {
		START = -1,
		COMBAT = 0,
		FORMATION,
		RESEARCH,
		FACTORY,
		AIDECONV,
		SELECT_AIDE,
		SELECT_SET,
		SELECT_DEV,
		SELECT_VIEW,
		TURN_BACK,
		END
	};

	struct selectBox {
		void*		adress;
		D2D_RECT_F	box;
		D3DXVECTOR2	pos;
	};

protected:
	BaseTaticDoll*	aideDoll;
	SINT		saveAideID;

	STATE		sceneState;

	string		curConvKey;
	float		ConvAlpha;
	bool		isConvers;

	vector<selectBox>			selBox;
	float						virtualHeight;
	float						virtualLimit;
	int							asixVirtual;
	bool						mouseDrag;

	string		SceneKey;
	bool		isSceneChanged;

	unordered_map<SBUTTONS, Button>		mButton;

	static LobbyScene* object;
};

#endif // !LOBBYSCENE_H
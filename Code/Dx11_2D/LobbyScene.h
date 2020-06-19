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

private:
	const D3DXVECTOR2 AXIS_COMBAT_BUTTON = D3DXVECTOR2(950, 500);
	const D3DXVECTOR2 AXIS_FACTORY_BUTTON = D3DXVECTOR2(1150, 400);
	const D3DXVECTOR2 AXIS_RESEARCH_BUTTON = D3DXVECTOR2(950, 400);
	const D3DXVECTOR2 AXIS_FORMATION_BUTTON = D3DXVECTOR2(1150, 500);
	const D3DXVECTOR2 AXIS_AIDECOV_BUTTON = D3DXVECTOR2(WINSIZEX*0.4f, WINSIZEY*0.8f);

protected:
	BaseTaticDoll*	aideDoll;

	string		curConvKey;
	float		ConvAlpha;
	bool		isConvers;

	string		SceneKey;
	bool		isSceneChanged;

	unordered_map<string, Button>		mButton;

private:
	static void CombatButton(void* obj);
	static void FactoryButton(void* obj);
	static void FormationButton(void* obj);
	static void ReserchButton(void* obj);
	static void AideConverButton(void* obj);

};

#endif // !LOBBYSCENE_H
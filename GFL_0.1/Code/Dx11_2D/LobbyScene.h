#pragma once
#include "Scene.h"

class LobbyScene : public Scene
{
private:
	const D3DXVECTOR2 AXIS_COMBAT_BUTTON =		D3DXVECTOR2(950, 500);
	const D3DXVECTOR2 AXIS_FACTORY_BUTTON =		D3DXVECTOR2(1150, 400);
	const D3DXVECTOR2 AXIS_RESEARCH_BUTTON =	D3DXVECTOR2(950, 400);
	const D3DXVECTOR2 AXIS_FORMATION_BUTTON =	D3DXVECTOR2(1150, 500);
	const D3DXVECTOR2 AXIS_AIDECOV_BUTTON =		D3DXVECTOR2(WINSIZEX*0.4f, WINSIZEY*0.8f);

protected:
	TaticDoll*	aideDoll;

	string		curConvKey;
	float		ConvAlpha;
	bool		isConvers;

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
	static void CombatButton(void* obj);
	static void FactoryButton(void* obj);
	static void FormationButton(void* obj);
	static void ReserchButton(void* obj);
	static void AideConverButton(void* obj);
};
#ifndef MAINGAME_H
#define MAINGAME_H

#include "SceneList.hpp"
#include "GameNode.h"

class MainGame : public GameNode
{
private: // 조명 설정에 필요한 변수
	DirectionalLight	m_stDirLight;
	ID3D11Buffer*		m_pDirLightBuffer;
	ID3D11Buffer*		m_pCameraPosBuffer;
	float				m_fLightRotZ;

private:
	ID3D11BlendState*	NormalBlendState;
	ID3D11BlendState*	AlphaBlendState;

	

private:
	void settingMaxtrix();
	void settingBlendOp();
	void settingDepth();

public:
	MainGame();
	~MainGame();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

};

#endif // !MAINGAME_H
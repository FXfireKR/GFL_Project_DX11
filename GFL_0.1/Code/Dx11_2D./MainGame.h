#pragma once
#include "GameNode.h"
#include "AllScene.h"

class MainGame : public GameNode
{
private: // ���� ������ �ʿ��� ����
	DirectionalLight	m_stDirLight;
	ID3D11Buffer*		m_pDirLightBuffer;
	ID3D11Buffer*		m_pCameraPosBuffer;
	float				m_fLightRotZ;

private:
	ID3D11Buffer*		vpBuffer;	// ��x�������� ��Ʈ���� ����
	ViewProjMatrix*		vpMatrix;	// ��x�������� ����

	ID3D11BlendState*	NormalBlendState;
	ID3D11BlendState*	AlphaBlendState;

public:
	MainGame();
	~MainGame();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};


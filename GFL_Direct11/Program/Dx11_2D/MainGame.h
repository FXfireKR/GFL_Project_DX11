#pragma once
#include "GameNode.h"
#include "SceneBind.h"

class MainGame : public GameNode
{
private:
	ID3D11Buffer*		vpBuffer;	// ��x�������� ��Ʈ���� ����
	tagVPMatrix*		vpMatrix;	// ��x�������� ����

	ID3D11BlendState*	NormalBlendState;
	ID3D11BlendState*	AlphaBlendState;

	float FramePerSecond;
	
public:
	MainGame();
	~MainGame();

	void ShaderInsert();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	
};
#pragma once
#include "singleton.h"

typedef D3DXVECTOR2				DV2;
typedef D3DXVECTOR3				DV3;
typedef D3DXCOLOR				DCR;

class Image : public singleton<Image>
{
private:	// Define Typedef

	struct tagNoiseBuffer
	{
	public:
		float startY;
		float limitAlpha;
		D3DXVECTOR2 _padding;
	};


private:	// Define Value Ref
	D3DXMATRIX				worldMatrix;
	
	D3DXVECTOR2				imgSize;
	D3DXVECTOR3				Scale;
	D3DXVECTOR3				Rotate;
	D3DXVECTOR2				Trans;
	
	PTVertex				arrVertex[4];
	PTVertex				mutableVertex[4];
	D3DXCOLOR				color;
	tagNoiseBuffer			NoiseStruct;
	
	ID3D11Buffer*			worldBuffer;
	ID3D11Buffer*			colorBuffer;
	ID3D11Buffer*			vertexBuffer;
	ID3D11Buffer*			NoiseBuffer;


public:		// Define Fnc
	Image();
	Image(const Image& other) = delete;						// disable copy construction
	Image &operator = (const  Image &  other) = delete;		// disable assignment
	~Image();

	void init() ;
	void release();
	void update();
	void render(const char* srvKey, DV2 _scale, DV2 _trans, DCR _color = DCR(1, 1, 1, 1), DV3 _rotate = DV3(0, 0, 0));
	void render(string srvKey, DV2 _scale, DV2 _trans, DCR _color = DCR(1, 1, 1, 1), DV3 _rotate = DV3(0, 0, 0), bool worldAlpha = true);
	void NoiseRender(string srvKey, DV2 _scale, DV2 _trans, DCR _color = DCR(1, 1, 1, 1), DV3 _rotate = DV3(0, 0, 0));
	void render(string srvKey, DV2 _scale, DV3 _trans, DCR _color = DCR(1, 1, 1, 1), DV3 _rotate = DV3(0, 0, 0));
	void render(string srvKey, const D3DXMATRIX& worldMatrix, ID3D11Buffer*const* vertexBuffer);
	void render(string normalKey, string alphaKey, DV2 _scale, DV2 _trans, DCR _color = DCR(1, 1, 1, 1), DV3 _rotate = DV3(0, 0, 0));
	void render(string normalKey, string alphaKey, DV2 _scale, DV2 _trans, DV2 minTexCord, DV2 maxTexCord, DCR _color = DCR(1, 1, 1, 1), DV3 _rotate = DV3(0, 0, 0));
};
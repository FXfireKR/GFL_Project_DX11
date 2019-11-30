#pragma once

class FrameImage
{
private:
	struct PTVertex
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR2 Texcoord;

		PTVertex() {}
		PTVertex(D3DXVECTOR3 p, D3DXVECTOR2 t) : Position(p), Texcoord(t) {}
	};

private:
	string					srvKey;
	Shader*					shaderManager;

	D3DXMATRIX				worldMatrix;

	D3DXVECTOR2				imgSize;
	D3DXVECTOR2				Scale;
	D3DXVECTOR3				Rotate;
	D3DXVECTOR2				Trans;

	D3DXCOLOR				color;

	PTVertex				arrVertex[5];

	ID3D11ShaderResourceView* srv;

	vector<PTVertex>		VertexVec;

	ID3D11Buffer*			worldBuffer;
	ID3D11Buffer*			colorBuffer;
	ID3D11Buffer*			vertexBuffer;
	ID3D11InputLayout*		vertexLayout;


public:
	FrameImage(string key);
	~FrameImage();

	void init();
	void release();
	void render(FLOAT sx, FLOAT ex, FLOAT sy, FLOAT ey);
	void render(D3DXVECTOR2 s, D3DXVECTOR3 r, D3DXVECTOR2 t, FLOAT sx, FLOAT ex, FLOAT sy, FLOAT ey);

public:
	inline D3DXVECTOR2 getImageSize() const { return imgSize; }

	inline D3DXVECTOR2 getScale() const { return Scale; }
	inline void setScale(D3DXVECTOR2 s) { Scale = s; }

	inline D3DXVECTOR3 getRotate() const { return Rotate; }
	inline void setRotate(D3DXVECTOR3 r) { Rotate = r; }

	inline D3DXVECTOR2 getTrans()const { return Trans; }
	inline void setTrans(D3DXVECTOR2 t) { Trans = t; }

	void setTextureKey(string key);
	inline string getTextureKey() const { return srvKey; }

	inline D3DXCOLOR getColor() const { return color; }
	inline void setColor(D3DXCOLOR c) { color = c; }

	inline FLOAT getAlpha() const { return color.a; }
	inline void setAlpha(FLOAT a) { color.a = a; }
};
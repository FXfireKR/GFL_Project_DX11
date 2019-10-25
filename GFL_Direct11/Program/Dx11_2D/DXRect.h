#pragma once

/*

						>> == 수정필요사항 == <<

	 - Scale, TransPos를 float[]이 아닌 VECTOR2를 사용해서 변동할것.

*/

class DXRect
{
private:
	struct PCVertex
	{
		D3DXVECTOR3	Position;
		D3DXCOLOR	Color;

		PCVertex() {}
		PCVertex(D3DXVECTOR3 p, D3DXCOLOR c) : Position(p), Color(c) {}
	};

private:
	Shader*				shaderManager;

	D3DXMATRIX*			worldMatrix;
	ID3D11Buffer*		worldBuffer;

	ID3D11Buffer*		vertexBuffer;
	ID3D11InputLayout*	vertexLayout;

	PCVertex			arrVertex[5];
	D3DXCOLOR			Color;

	float					Scale[2];
	float				Rotate[3];
	float				Transpos[2];

	float				Angle;

public:
	DXRect();
	~DXRect();

	HRESULT init();
	void release();
	void update();
	void render();

	bool AABB_Collition(const DXRect& rc);

public:
	inline int getScaleX() const { return Scale[0]; }
	inline int getScaleY() const { return Scale[1]; }

	inline void setScaleX(float x) { Scale[0] = x; }
	inline void setScaleY(float y) { Scale[1] = y; }

	inline float getTransX() const { return Transpos[0]; }
	inline float getTransY() const { return Transpos[1]; }

	inline void setTransX(float x) { Transpos[0] = x; }
	inline void setTransY(float y) { Transpos[1] = y; }

	inline float getRotateX() const { return Rotate[0]; }
	inline float getRotateY() const { return Rotate[1]; }
	inline float getRotateZ() const { return Rotate[2]; }

	inline void setRotateX(float x) { Rotate[0] = x; }
	inline void setRotateY(float y) { Rotate[1] = y; }
	inline void setRotateZ(float z) { Rotate[2] = z; }

	inline float& getAngle() { return Angle; }
	inline void setAngle(float a) { Angle = a; }


	inline int*		getScaleArr() { return (int*)&Scale; }
	inline float*	getTransArr() { return (float*)&Transpos; }
	inline float*	getRotateArr() { return (float*)&Rotate; }
	inline float*	getColorArr() { return (float*)Color; }

	inline PCVertex getVertexArr(int aNum) { return aNum > 3 ? arrVertex[aNum - 4] : arrVertex[aNum]; }
	inline D3DXMATRIX* getWorldMatrix() { return worldMatrix; }
};
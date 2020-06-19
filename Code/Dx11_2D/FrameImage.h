#pragma once
class FrameImage
{
private:
	vector<PTVertex>	vertex;
	ID3D11Buffer*		vertexBuffer;
	ID3D11Buffer*		colorBuffer;
	ID3D11Buffer*		worldBuffer;

public:
	FrameImage();
	~FrameImage();

	void render(string srvKey, VEC2 _scale, VEC2 _trans, VEC2 _minTexCoord, 
		VEC2 _maxTexCoord, VEC3 _rotate = VEC3(0, 0, 0), COLR _color = COLR(1, 1, 1, 1));
};
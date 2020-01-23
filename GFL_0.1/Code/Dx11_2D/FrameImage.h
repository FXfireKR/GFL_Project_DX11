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

	void render(string srvKey, DV2 _scale, DV2 _trans, DV2 _minTexCoord, DV2 _maxTexCoord, DV3 _rotate = DV3(0, 0, 0), DCR _color = DCR(1, 1, 1, 1));
}; 
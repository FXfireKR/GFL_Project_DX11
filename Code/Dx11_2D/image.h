#ifndef IMAGE_H
#define IMAGE_H

#include "singleton.hpp"

class Image : public singleton<Image>
{
private:	// Define Typedef

	struct tagNoiseBuffer
	{
		float	startY;
		float	limitAlpha;
		VEC2	_padding;
	};


private:	// Define Value Ref
	MATRIX				worldMatrix;

	VEC2				imgSize;
	VEC3				Scale;
	VEC3				Rotate;
	VEC2				Trans;

	PTVertex			arrVertex[4];
	PTVertex			mutableVertex[4];
	COLR				color;
	tagNoiseBuffer		NoiseStruct;

	DBuffer*			worldBuffer;
	DBuffer*			colorBuffer;
	DBuffer*			vertexBuffer;
	DBuffer*			NoiseBuffer;


public:		// Define Fnc
	Image();
	Image(const Image& other) = delete;						// disable copy construction
	Image &operator = (const  Image &  other) = delete;		// disable assignment
	~Image();

	void release();

	void NoiseRender(string srvKey, VEC2 _scale, VEC2 _trans, COLR _color = COLR(1, 1, 1, 1), VEC3 _rotate = VEC3(0, 0, 0));

	void render(const char* srvKey, VEC2 _scale, VEC2 _trans, COLR _color = COLR(1, 1, 1, 1), VEC3 _rotate = VEC3(0, 0, 0));
	void render(string srvKey, VEC2 _scale, VEC2 _trans, COLR _color = COLR(1, 1, 1, 1), VEC3 _rotate = VEC3(0, 0, 0), bool worldAlpha = true);
	void render(string srvKey, VEC2 _scale, VEC3 _trans, COLR _color = COLR(1, 1, 1, 1), VEC3 _rotate = VEC3(0, 0, 0));
	void render(string srvKey, const MATRIX& _wordMat, DBuffer* const* vertexBuffer);
	void render(string normalKey, string alphaKey, VEC2 _scale, VEC2 _trans, 
		COLR _color = COLR(1, 1, 1, 1), VEC3 _rotate = VEC3(0, 0, 0));
	void render(string normalKey, string alphaKey, VEC2 _scale, VEC2 _trans, 
		VEC2 minTexCord, VEC2 maxTexCord, COLR _color = COLR(1, 1, 1, 1), VEC3 _rotate = VEC3(0, 0, 0));

private:
	void update(VEC2 _scale, VEC2 _trans, VEC3 _rotate);
};

#endif
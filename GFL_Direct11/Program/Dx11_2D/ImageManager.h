#pragma once
#include "singleton.h"

enum IMAGE_TYPE
{
	JPG = 0,
	PNG,
	BMP
};

class ImageManager : public singleton<ImageManager>
{
private:
	struct ImageResource
	{
		IMAGE_TYPE type;
		string fileName;
		LPVOID memory;
		SIZE_T fileSize;
		ID3D11ShaderResourceView* texture;
		unsigned int Width, Height;
	};

private:
	map<string, ImageResource*> mResourece;

public:
	ImageManager();
	~ImageManager();

	void release();

	void InsertImageFile(__in string key, __in const char* _path);
	void InsertImageBinary(__in string key, __in string _path);

public:
	ID3D11ShaderResourceView* getTexture(__in string key) 
	{ 
		if (mResourece.count(key))
			return mResourece.find(key)->second->texture; 
	}

	ImageResource* getImageInfo(__in string key)
	{
		if (mResourece.count(key))
			return mResourece.find(key)->second;
	}
};
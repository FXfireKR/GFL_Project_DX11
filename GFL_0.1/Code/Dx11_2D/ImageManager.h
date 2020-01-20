#pragma once
#include "singleton.h"

class ThreadPool;

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
		UINT Width, Height;
	};

private:
	map<string, ImageResource*> mResourece;

public:
	ImageManager();
	~ImageManager();

	void release();

	void InsertImageFile(__in string key, __in const char* _path);
	void InsertImageBinary(__in string key, __in string _path);
	void InsertImageBinary(__in ThreadPool* _trdPol , __in string key, __in string _path);

	void ReleaseMemory(__in string key);

public:
	ID3D11ShaderResourceView* getTexture(__in string key)
	{
		if (mResourece.count(key))
			return mResourece.find(key)->second->texture;
		else
			return nullptr;
	}

	ImageResource* getImageInfo(__in string key)
	{
		if (mResourece.count(key))
			return mResourece.find(key)->second;
	}

	inline bool isValidKey(__in string key)
	{
		//if key was already Exist, return false
		return !mResourece.count(key);
	}
};
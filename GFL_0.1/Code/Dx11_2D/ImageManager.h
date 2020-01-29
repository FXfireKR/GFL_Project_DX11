#pragma once
#include "singleton.h"

class ThreadPool;

struct uiAtlas
{
	string name;
	string textureKey;
	string alphaTexKey;
	D3DXVECTOR2 mixTexCoord;
	D3DXVECTOR2 maxTexCoord;
};

enum IMAGE_TYPE
{
	JPG = 0,
	PNG,
	BMP
};

struct ImageResource
{
	IMAGE_TYPE type;
	string fileName;
	LPVOID memory;
	SIZE_T fileSize;
	ID3D11ShaderResourceView* texture;
	UINT Width, Height;
	D3DXVECTOR2 perImageSize;				//	프레임 이미지 X, Y 칸당 사이즈
	POINT Frame;							//	프레임 이미지 X, Y 총 프레임 갯수
};

class ImageManager : public singleton<ImageManager>
{
private:
	map<string, ImageResource*> mResourece;
	map<string, ImageResource*>::iterator miResourece;

	map<string, uiAtlas>* mUiAtlas;

public:
	ImageManager();
	~ImageManager();

	void release();

	void InsertImageFile(__in string key, __in const char* _path);
	void InsertImageBinary(__in string key, __in string _path, __in SINT _frameX = 0, __in SINT _frameY = 0);
	void InsertImageBinary(__in ThreadPool* _trdPol , __in string key, __in string _path);

	void ReleaseMemory(__in string key);

public:
	inline ID3D11ShaderResourceView* getTexture(__in string key)
	{
		if ((miResourece = mResourece.find(key)) != mResourece.end())
			return miResourece->second->texture;
		return nullptr;
	}

	inline ImageResource* getImageInfo(__in string key)
	{
		if ((miResourece = mResourece.find(key)) != mResourece.end())
			return miResourece->second;
		return nullptr;
	}

	inline map<string, uiAtlas>* getUiAtlas()
	{
		return mUiAtlas;
	}

	inline uiAtlas getUiAtlas(string key)
	{
		map<string, uiAtlas>::iterator mIter;
		return (mIter = mUiAtlas->find(key)) != mUiAtlas->end() ? mIter->second : uiAtlas();
	}

	inline bool isValidKey(__in string key)
	{
		//if key was already Exist, return false
		return !mResourece.count(key);
	}
};
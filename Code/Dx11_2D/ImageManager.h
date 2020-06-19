#ifndef IMANAGER_H
#define IMANAGER_H
#include "singleton.hpp"

class ThreadPool;

enum IMAGE_TYPE{
	JPG = 0,
	PNG,
	BMP
};

struct ImageResource{
	IMAGE_TYPE				type;
	string					fileName;
	LPVOID					memory;
	SIZE_T					fileSize;
	ShaderResourceView*		texture;
	UINT					Width, Height;
	VEC2					perImageSize;		//	프레임 이미지 X, Y 칸당 사이즈
	POINT					Frame;				//	프레임 이미지 X, Y 총 프레임 갯수
};

class ImageManager : public singleton<ImageManager>
{
private:
	unordered_map<string, uiAtlas*>*					mUiAtlas;

	unordered_map<string, ImageResource*>				mResourece;
	unordered_map<string, ImageResource*>::iterator		miResourece;

public:
	ImageManager();
	~ImageManager();

	void InsertImageFile(__in string key, __in const char* _path);

	void InsertImageBinary(__in string key, __in string _path, __in SINT _frameX = 0, __in SINT _frameY = 0);
	void InsertImageBinary(__in ThreadPool* _trdPol, __in string key, __in string _path);

	void ReleaseMemory(__in string key);

private:
	void release();	

public:
	inline ID3D11ShaderResourceView* getTexture(__in string key){
		if ((miResourece = mResourece.find(key)) != mResourece.end())
			return miResourece->second->texture;
		return nullptr;
	}

	inline ImageResource* getImageInfo(__in string key){
		if ((miResourece = mResourece.find(key)) != mResourece.end())
			return miResourece->second;
		return nullptr;
	}

	inline unordered_map<string, uiAtlas*>* getUiAtlas() { return mUiAtlas; }

	inline const uiAtlas* getUiAtlas(__in string key){
		unordered_map<string, uiAtlas*>::iterator mIter;
		return (mIter = mUiAtlas->find(key)) != mUiAtlas->end() ? mIter->second : nullptr;
	}

	inline bool isValidKey(__in string key) { return !mResourece.count(key); }

};
#endif
#pragma once

#include "TextBase.h"
#include "singleton.h"

class TextManager : public singleton <TextManager>
{
private:
	map<string, TextBase*> mText;
	DXGI_SWAP_CHAIN_DESC swapDesc;

private:
	HRESULT DWFactoryInit(IDWriteFactory** _dwFactory);

public:
	TextManager();
	~TextManager();

	HRESULT DWInit();

	// If You Use the Text, You Must Create Field First.
	HRESULT Create_TextField(string key, LPCWSTR fontName, string t, float size, DWRITE_FONT_WEIGHT height = DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH strech = DWRITE_FONT_STRETCH_NORMAL);

	void Change_Text(string _key, string _text);
	void Change_Text(string _key, const char* _text);
	void Change_TextSize(string _key, float _size);
	void Change_TextWeight(string _key, DWRITE_FONT_WEIGHT _weight);
	void Change_TextStyle(string _key, DWRITE_FONT_STYLE _style);
	void Change_TextStretch(string _key, DWRITE_FONT_STRETCH _stretch);
	void Change_TextFontName(string _key, LPCWSTR _fntName);

	void release();					//All DataClear
	void release_at(string key);	//Select DataClear

	void SettingColor(string key, D2D1::ColorF c);
	void SettingColor(string key, FLOAT r, FLOAT g, FLOAT b, FLOAT alpha = 1.0F);
	void SettingColor(string _key, D3DXCOLOR c);

	void SetDelayText(string key, UINT count);

	void TextRender(string key, FLOAT x, FLOAT y);
	void TextRender(string key, FLOAT x, FLOAT y, D2D1::ColorF c);
	void TextRender(string key, FLOAT x, FLOAT y, FLOAT width, FLOAT height);
	void TextRender(string key, FLOAT x, FLOAT y, FLOAT width, FLOAT height, D2D1::ColorF c);
	void TextRender(string key, FLOAT x, FLOAT y, FLOAT width, FLOAT height, ColorF c, DWRITE_TEXT_ALIGNMENT textAlignment);


public:
	inline TextBase* getProperty(string key) { return mText.count(key) ? mText.find(key)->second : nullptr; }
	inline int getTextFieldSize() { return mText.size(); }
};
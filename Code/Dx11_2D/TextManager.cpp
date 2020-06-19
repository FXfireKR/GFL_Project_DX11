#include "stdafx.h"
#include "TextManager.h"

#pragma warning(disable : 4996)

TextManager::TextManager()
{
	this->DWInit();
}

TextManager::~TextManager()
{
	this->release();
}

HRESULT TextManager::DWFactoryInit(IDWriteFactory ** _dwFactory)
{
	HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(*_dwFactory), (IUnknown**)_dwFactory);
	return FAILED(hr) ? hr : hr;
}

HRESULT TextManager::DWInit()
{
	FLOAT dpiX, dpiY;
	HRESULT hr = S_OK;

	//Get SwapChain Buffer
	SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	//Direct 2D Factory Initialize
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
	assert(SUCCEEDED(hr));

	//Get Desktop DPI Size
	d2dFactory->GetDesktopDpi(&dpiX, &dpiY);

	//Make Render Properties
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties
	(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		dpiX,
		dpiY
	);


	//Create DXGI SurfaceRenderTarget
	hr = d2dFactory->CreateDxgiSurfaceRenderTarget(pBackBuffer, &props, &d2Rtg);
	assert(SUCCEEDED(hr));


	//Create DirectWrite Factory
	hr = DWFactoryInit(&dwFactory);
	assert(SUCCEEDED(hr));

	hr = SwapChain->GetDesc(&swapDesc);
	assert(SUCCEEDED(hr));

	return S_OK;
}

HRESULT TextManager::Create_TextField(string key, LPCWSTR fontName, string t, float size,
	DWRITE_FONT_WEIGHT height, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH strech)
{
	// if There was no TextField Key
	if ((iterText = mText.find(key)) == mText.end())
	{
		TextBase* pTextBase = new TextBase();

		pTextBase->CreateTextBase(fontName, size, height, style, strech);
		pTextBase->InsertText(t);
		pTextBase->setboxWidth(WINSIZEX);
		pTextBase->setboxHeight(WINSIZEY);

		mText.insert(make_pair(key, pTextBase));
		return S_OK;
	}

	// If Same Key of Text Field was Exist in map<>, Just Change the Properies
	else
	{
		iterText->second->CreateTextBase(fontName, size, height, style, strech);
		iterText->second->InsertText(t);
		iterText->second->setboxWidth(WINSIZEX);
		iterText->second->setboxHeight(WINSIZEY);
		return S_OK;
	}

	return E_FAIL;
}

void TextManager::Change_Text(string _key, string _text)
{
	if ((iterText = mText.find(_key)) != mText.end())
		iterText->second->InsertText(_text);

}

void TextManager::Change_Text(string _key, const char * _text)
{
	if ((iterText = mText.find(_key)) != mText.end())
		iterText->second->InsertText(_text);
}

void TextManager::ChangeText(string _key, const string _text, ...)
{
	if ((iterText = mText.find(_key)) != mText.end()) 
	{
		va_list args;

		va_start(args, _text);
		size_t len = vsnprintf(NULL, 0, _text.c_str(), args);
		va_end(args);

		vector<char> vec(len + 1);

		va_start(args, _text);
		vsnprintf(&vec[0], len + 1, _text.c_str(), args);
		va_end(args);

		iterText->second->InsertText(&vec[0]);
	}
}

void TextManager::Change_TextSize(string _key, float _size)
{
	if ((iterText = mText.find(_key)) != mText.end()) {
		iterText->second->SetSize(_size);
		iterText->second->ReLoadTextBase(&dwFactory, iterText->second->getFontName());
	}
}

void TextManager::Change_TextWeight(string _key, DWRITE_FONT_WEIGHT _weight)
{
	if ((iterText = mText.find(_key)) != mText.end()) {
		iterText->second->SetWeight(_weight);
		iterText->second->ReLoadTextBase(&dwFactory, iterText->second->getFontName());
	}
}

void TextManager::Change_TextStyle(string _key, DWRITE_FONT_STYLE _style)
{
	if ((iterText = mText.find(_key)) != mText.end()) {
		iterText->second->SetStyle(_style);
		iterText->second->ReLoadTextBase(&dwFactory, iterText->second->getFontName());
	}
}

void TextManager::Change_TextStretch(string _key, DWRITE_FONT_STRETCH _stretch)
{
	if ((iterText = mText.find(_key)) != mText.end()){
		iterText->second->SetStretch(_stretch);
		iterText->second->ReLoadTextBase(&dwFactory, iterText->second->getFontName());
	}
}

void TextManager::Change_TextFontName(string _key, LPCWSTR _fntName)
{
	if ((iterText = mText.find(_key)) != mText.end())
		iterText->second->ReLoadTextBase(&dwFactory, _fntName);
}

void TextManager::release()
{
	for (auto& it : mText)
		SAFE_DELETE(it.second);

	mText.clear();
}

void TextManager::release_at(string key)
{
	if ((iterText = mText.find(key)) != mText.end()){
		SAFE_DELETE(iterText->second);
		mText.erase(iterText);
	}
}

void TextManager::SettingColor(string key, D2D1::ColorF c)
{
	if ((iterText = mText.find(key)) != mText.end())
		iterText->second->SetColor(c);
}

void TextManager::SettingColor(string key, FLOAT r, FLOAT g, FLOAT b, FLOAT alpha)
{
	if ((iterText = mText.find(key)) != mText.end())
		iterText->second->SetColor(D2D1::ColorF(r, g, b, alpha));
}

void TextManager::SettingColor(string key, D3DXCOLOR c)
{
	if ((iterText = mText.find(key)) != mText.end())
		iterText->second->SetColor(c);
}

void TextManager::SetDelayText(string key, UINT count)
{
	if ((iterText = mText.find(key)) != mText.end())
		iterText->second->DelayText(count);
}

void TextManager::TextRender(string key, FLOAT x, FLOAT y)
{
	if ((iterText = mText.find(key)) != mText.end()){
		//it->second->setboxWidth(WINDOWSIZE_X);
		//it->second->setboxHeight(WINDOWSIZE_Y);
		d2Rtg->BeginDraw();
		iterText->second->render(x, y);
		d2Rtg->EndDraw();
	}
}

void TextManager::TextRender(string key, FLOAT x, FLOAT y, D2D1::ColorF c)
{
	if ((iterText = mText.find(key)) != mText.end()) {
		//it->second->setboxWidth(WINDOWSIZE_X);
		//it->second->setboxHeight(WINDOWSIZE_Y);
		d2Rtg->BeginDraw();
		iterText->second->render(x, y, c);
		d2Rtg->EndDraw();
	}
}

void TextManager::TextRender(string key, FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
	if ((iterText = mText.find(key)) != mText.end()) {
		iterText->second->setboxWidth(width);
		iterText->second->setboxHeight(height);

		d2Rtg->BeginDraw();
		iterText->second->render(x, y);
		d2Rtg->EndDraw();
	}
}

void TextManager::TextRender(string key, FLOAT x, FLOAT y, FLOAT width, FLOAT height, D2D1::ColorF c)
{
	if((iterText = mText.find(key)) != mText.end()) {
		iterText->second->setboxWidth(width);
		iterText->second->setboxHeight(height);

		d2Rtg->BeginDraw();
		iterText->second->render(x, y, c);
		d2Rtg->EndDraw();
	}
}

void TextManager::TextRender(string key, FLOAT x, FLOAT y, FLOAT width, FLOAT height, ColorF c, DWRITE_TEXT_ALIGNMENT textAlignment)
{
	if ((iterText = mText.find(key)) != mText.end()) {
		iterText->second->setboxWidth(width);
		iterText->second->setboxHeight(height);

		d2Rtg->BeginDraw();
		iterText->second->render(x, y, c, textAlignment);
		d2Rtg->EndDraw();
	}
}
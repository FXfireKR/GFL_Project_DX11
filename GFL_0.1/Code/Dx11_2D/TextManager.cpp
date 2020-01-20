#include "stdafx.h"
#include "TextManager.h"

TextManager::TextManager()
{
}

TextManager::~TextManager()
{
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
	if (!mText.count(key))
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
		auto& TextField = mText.find(key)->second;

		TextField->CreateTextBase(fontName, size, height, style, strech);
		TextField->InsertText(t);
		TextField->setboxWidth(WINSIZEX);
		TextField->setboxHeight(WINSIZEY);
	}


	return E_FAIL;
}

void TextManager::Change_Text(string _key, string _text)
{
	if (mText.count(_key))
		mText.find(_key)->second->InsertText(_text);
	
}

void TextManager::Change_Text(string _key, const char * _text)
{
	if (mText.count(_key))
		mText.find(_key)->second->InsertText(_text);
}

void TextManager::Change_TextSize(string _key, float _size)
{
	if (mText.count(_key))
	{
		auto& it = mText.find(_key)->second;
		it->SetSize(_size);
		it->ReLoadTextBase(&dwFactory, it->getFontName());
	}
}

void TextManager::Change_TextWeight(string _key, DWRITE_FONT_WEIGHT _weight)
{
	if (mText.count(_key))
	{
		auto& it = mText.find(_key)->second;
		it->SetWeight(_weight);
		it->ReLoadTextBase(&dwFactory, it->getFontName());
	}
}

void TextManager::Change_TextStyle(string _key, DWRITE_FONT_STYLE _style)
{
	if (mText.count(_key))
	{
		auto& it = mText.find(_key)->second;
		it->SetStyle(_style);
		it->ReLoadTextBase(&dwFactory, it->getFontName());
	}
}

void TextManager::Change_TextStretch(string _key, DWRITE_FONT_STRETCH _stretch)
{
	if (mText.count(_key))
	{
		auto& it = mText.find(_key)->second;
		it->SetStretch(_stretch);
		it->ReLoadTextBase(&dwFactory, it->getFontName());
	}
}

void TextManager::Change_TextFontName(string _key, LPCWSTR _fntName)
{
	if (mText.count(_key))
		mText.find(_key)->second->ReLoadTextBase(&dwFactory, _fntName);
	
	
}

void TextManager::release()
{
	for (auto& it : mText)
		SAFE_DEL(it.second);

	mText.clear();
}

void TextManager::release_at(string key)
{
	if (mText.count(key))
	{
		SAFE_DEL(mText.find(key)->second);
		mText.erase(mText.find(key));
	}
}

void TextManager::SettingColor(string key, D2D1::ColorF c)
{
	if (mText.count(key))
	{
		auto it = mText.find(key);
		it->second->SetColor(c);
	}
}

void TextManager::SettingColor(string key, FLOAT r, FLOAT g, FLOAT b, FLOAT alpha)
{
	if (mText.count(key))
	{
		auto it = mText.find(key);
		it->second->SetColor(D2D1::ColorF(r, g, b, alpha));
	}
}

void TextManager::SettingColor(string _key, D3DXCOLOR c)
{
	if (mText.count(_key))
		mText.find(_key)->second->SetColor(c);
	
}

void TextManager::SetDelayText(string key, UINT count)
{
	if (mText.count(key))
	{
		auto it = mText.find(key);
		it->second->DelayText(count);
	}
}

void TextManager::TextRender(string key, FLOAT x, FLOAT y)
{
	if (mText.count(key))
	{
		auto it = mText.find(key);

		//it->second->setboxWidth(WINDOWSIZE_X);
		//it->second->setboxHeight(WINDOWSIZE_Y);
		d2Rtg->BeginDraw();
		it->second->render(x, y);
		d2Rtg->EndDraw();
	}
}

void TextManager::TextRender(string key, FLOAT x, FLOAT y, D2D1::ColorF c)
{
	if (mText.count(key))
	{
		auto it = mText.find(key);

		//it->second->setboxWidth(WINDOWSIZE_X);
		//it->second->setboxHeight(WINDOWSIZE_Y);
		d2Rtg->BeginDraw();
		it->second->render(x, y, c);
		d2Rtg->EndDraw();
	}
}

void TextManager::TextRender(string key, FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
	if (mText.count(key))
	{
		auto it = mText.find(key);

		it->second->setboxWidth(width);
		it->second->setboxHeight(height);

		d2Rtg->BeginDraw();
		it->second->render(x, y);
		d2Rtg->EndDraw();
	}
}

void TextManager::TextRender(string key, FLOAT x, FLOAT y, FLOAT width, FLOAT height, D2D1::ColorF c)
{
	if (mText.count(key))
	{
		auto it = mText.find(key);

		it->second->setboxWidth(width);
		it->second->setboxHeight(height);

		d2Rtg->BeginDraw();
		it->second->render(x, y, c);
		d2Rtg->EndDraw();
	}
}

void TextManager::TextRender(string key, FLOAT x, FLOAT y, FLOAT width, FLOAT height, ColorF c, DWRITE_TEXT_ALIGNMENT textAlignment)
{
	if (mText.count(key))
	{
		auto it = mText.find(key);

		it->second->setboxWidth(width);
		it->second->setboxHeight(height);

		d2Rtg->BeginDraw();
		it->second->render(x, y, c, textAlignment);
		d2Rtg->EndDraw();
	}
}
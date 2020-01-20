#include "stdafx.h"
#include "TextBase.h"

TextBase::TextBase() : pTextLayout(nullptr), pTextFormat(nullptr), pSolidBrush(nullptr)
{
	fontSize = 0;
	boxWidth = WINSIZEX;
	boxHeight = WINSIZEY;
	bufRender = false;
}

TextBase::~TextBase()
{
}

HRESULT TextBase::Init(LPCWSTR fontname, float size, IDWriteFactory ** factory, IDWriteTextFormat ** format, DWRITE_FONT_WEIGHT height, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH strech)
{
	return (*factory)->CreateTextFormat(fontname, 0, height, style, strech, size, L"ko", format);
}

HRESULT TextBase::CreateTextBase(LPCWSTR fntName, float size, DWRITE_FONT_WEIGHT height, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH strech)
{
	this->Init(fntName, size, &dwFactory, &pTextFormat, height, style, strech);

	fontSize = size;
	fontName = fntName;

	weight = height;
	this->style = style;
	this->stretch = strech;

	return S_OK;
}

HRESULT TextBase::ReLoadTextBase(IDWriteFactory **factory, LPCWSTR _fntName)
{
	if (dwFactory != nullptr)
	{
		if (pTextFormat != nullptr && pTextLayout != nullptr)
		{
			SAFE_REL(pTextFormat);
			Init(_fntName, fontSize, &dwFactory, &pTextFormat, weight, style, stretch);
		}
	}

	return S_OK;
}

HRESULT TextBase::InsertText(string t)
{
	text = t;
	return S_OK;
}

HRESULT TextBase::InsertText(const char * t)
{
	text = t;
	return S_OK;
}

void TextBase::release()
{
	SAFE_REL(pTextLayout);
	SAFE_REL(pTextFormat);
	SAFE_REL(pSolidBrush);
}

HRESULT TextBase::DelayText(UINT cnt)
{
	provLen = 0;

	count = 0;
	maxCount = cnt;

	bufRender = true;
	delayRendOver = false;

	return S_OK;
}

void TextBase::SetColor(ColorF c)
{
	d2Rtg->CreateSolidColorBrush(c, &pSolidBrush);
}

void TextBase::SetColor(D3DXCOLOR c)
{
	d2Rtg->CreateSolidColorBrush(ColorF(c.r, c.g, c.b, c.a), &pSolidBrush);
}

void TextBase::SetWeight(DWRITE_FONT_WEIGHT _weight)
{
	weight = _weight;
}

void TextBase::SetStyle(DWRITE_FONT_STYLE _style)
{
	style = _style;
}

void TextBase::SetStretch(DWRITE_FONT_STRETCH _stretch)
{
	stretch = _stretch;
}

void TextBase::SetSize(float _size)
{
	fontSize = _size;
}

void TextBase::render(FLOAT x, FLOAT y)
{
	if (!bufRender)
	{
		USES_CONVERSION;
		rendText = A2W(text.c_str());

		if(pTextLayout != nullptr)
			SAFE_REL(pTextLayout);

		dwFactory->CreateTextLayout(rendText, wcslen(rendText), pTextFormat, boxWidth, boxHeight, &pTextLayout);
		d2Rtg->DrawTextLayout(D2D1::Point2F(x, y), pTextLayout, pSolidBrush);
	}

	else
	{
		string buf;

		if (!delayRendOver)
		{
			if (count >= maxCount)
			{
				count = 0;
				provLen += 3;

				if (text.size() <= provLen)
				{
					provLen = text.size();
					delayRendOver = true;
				}
			}
			else
				++count;
		}
		else
			count = 0;

		buf.assign(text, 0, provLen);

		USES_CONVERSION;
		LPCWCHAR str = A2W(buf.c_str());

		rendText = str;
		dwFactory->CreateTextLayout(rendText, wcslen(rendText), pTextFormat, boxWidth, boxHeight, &pTextLayout);
		d2Rtg->DrawTextLayout(D2D1::Point2F(x, y), pTextLayout, pSolidBrush);
	}
}

void TextBase::render(FLOAT x, FLOAT y, ColorF c)
{
	if (!bufRender)
	{
		USES_CONVERSION;
		rendText = A2W(text.c_str());

		dwFactory->CreateTextLayout(rendText, wcslen(rendText), pTextFormat, boxWidth, boxHeight, &pTextLayout);

		d2Rtg->CreateSolidColorBrush(c, &pSolidBrush);
		d2Rtg->DrawTextLayout(D2D1::Point2F(x, y), pTextLayout, pSolidBrush);
	}

	else
	{
		string buf;

		if (!delayRendOver)
		{

			if (count >= maxCount)
			{
				count = 0;
				provLen += 2;

				if (text.size() <= provLen)
				{
					provLen = text.size();
					delayRendOver = true;
				}
			}
			else
				++count;

			buf.assign(text, 0, provLen);
		}
		else
			count = 0;

		USES_CONVERSION;
		LPCWCHAR str = A2W(buf.c_str());

		rendText = str;
		dwFactory->CreateTextLayout(rendText, wcslen(rendText), pTextFormat, boxWidth, boxHeight, &pTextLayout);
		d2Rtg->CreateSolidColorBrush(c, &pSolidBrush);
		d2Rtg->DrawTextLayout(D2D1::Point2F(x, y), pTextLayout, pSolidBrush);
	}
}

void TextBase::render(FLOAT x, FLOAT y, ColorF c, DWRITE_TEXT_ALIGNMENT textAlignment)
{
	if (!bufRender)
	{
		USES_CONVERSION;
		rendText = A2W(text.c_str());

		dwFactory->CreateTextLayout(rendText, wcslen(rendText), pTextFormat, boxWidth, boxHeight, &pTextLayout);
		pTextLayout->SetTextAlignment(textAlignment);

		d2Rtg->CreateSolidColorBrush(c, &pSolidBrush);
		d2Rtg->DrawTextLayout(D2D1::Point2F(x, y), pTextLayout, pSolidBrush);
	}

	else
	{
		string buf;

		if (!delayRendOver)
		{

			if (count >= maxCount)
			{
				count = 0;
				provLen += 2;

				if (text.size() <= provLen)
				{
					provLen = text.size();
					delayRendOver = true;
				}
			}
			else
				++count;

			buf.assign(text, 0, provLen);
		}
		else
			count = 0;

		USES_CONVERSION;
		LPCWCHAR str = A2W(buf.c_str());

		rendText = str;
		dwFactory->CreateTextLayout(rendText, wcslen(rendText), pTextFormat, boxWidth, boxHeight, &pTextLayout);
		d2Rtg->CreateSolidColorBrush(c, &pSolidBrush);
		d2Rtg->DrawTextLayout(D2D1::Point2F(x, y), pTextLayout, pSolidBrush);
	}
}

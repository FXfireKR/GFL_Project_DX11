#pragma once


class TextBase
{
private:
	LPCWSTR					fontName;				//글꼴 이름입니다.
	float					fontSize;				//font의 사이즈입니다
	string					text;					//text에 들어갈 내용입니다.	(멀티바이트형식)
	FLOAT					boxWidth, boxHeight;	//텍스트가 랜딩될 박스의 크기입니다.


//DelayRendering Property
	LPCWCHAR				rendText;			//랜더링할 텍스트입니다.
	FLOAT					count, maxCount;	//얼마나 지연시킬것인지 결정
	UINT					provLen;			//진행된 길이만큼을 보내줌
	bool					bufRender;			//지연렌더링을 할것인지 물어봅니다.
	bool					delayRendOver;		//랜더링이 끝났음을 리턴해줌

	IDWriteTextLayout*		pTextLayout;
	IDWriteTextFormat*		pTextFormat;
	ID2D1SolidColorBrush*	pSolidBrush;

	DWRITE_FONT_WEIGHT		weight;
	DWRITE_FONT_STYLE		style;
	DWRITE_FONT_STRETCH		stretch;

public:
	TextBase();
	~TextBase();

	HRESULT Init(LPCWSTR fontname, float size,
		IDWriteFactory **factory, IDWriteTextFormat **format, DWRITE_FONT_WEIGHT height = DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH strech = DWRITE_FONT_STRETCH_NORMAL);

	HRESULT CreateTextBase(LPCWSTR fntName, float size, DWRITE_FONT_WEIGHT height = DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH strech = DWRITE_FONT_STRETCH_NORMAL);

	HRESULT ReLoadTextBase(IDWriteFactory **factory, LPCWSTR _fntName);

	//멀티바이트 형식으로 문자열을 로드해온다.
	HRESULT InsertText(string t);
	HRESULT InsertText(const char* t);

	void release();

	HRESULT DelayText(UINT cnt);

	void SetColor(ColorF c);
	void SetColor(D3DXCOLOR c);

	void SetWeight(DWRITE_FONT_WEIGHT _weight);
	void SetStyle(DWRITE_FONT_STYLE _style);
	void SetStretch(DWRITE_FONT_STRETCH _stretch);
	void SetSize(float _size);

	void render(FLOAT x, FLOAT y);																//x, y좌표에 기존에존재하는 brush로 진행
	void render(FLOAT x, FLOAT y, ColorF c);													//x, y좌표에 ColorF로 출력
	void render(FLOAT x, FLOAT y, ColorF c, DWRITE_TEXT_ALIGNMENT textAlignment);				//x, y좌표에 ColorF로 출력


public:
	inline string getText() const { return text; }

	// get LayOut of this Text
	inline IDWriteTextLayout* getLayout() { return pTextLayout; }
	inline IDWriteTextFormat* getFormat() { return pTextFormat; }

	inline LPCWSTR getFontName() const { return fontName; }

	inline bool getbufRender() const { return bufRender; }
	inline void setbufRender() { bufRender ? bufRender = false : bufRender = true; }

	inline bool getdelayRendOver() const { return delayRendOver; }

	inline FLOAT getboxWidth() const { return boxWidth; }
	inline FLOAT getboxHeight() const { return boxHeight; }

	inline void setboxWidth(FLOAT width) { boxWidth = width; }
	inline void setboxHeight(FLOAT height) { boxHeight = height; }
};
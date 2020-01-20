#pragma once


class TextBase
{
private:
	LPCWSTR					fontName;				//�۲� �̸��Դϴ�.
	float					fontSize;				//font�� �������Դϴ�
	string					text;					//text�� �� �����Դϴ�.	(��Ƽ����Ʈ����)
	FLOAT					boxWidth, boxHeight;	//�ؽ�Ʈ�� ������ �ڽ��� ũ���Դϴ�.


//DelayRendering Property
	LPCWCHAR				rendText;			//�������� �ؽ�Ʈ�Դϴ�.
	FLOAT					count, maxCount;	//�󸶳� ������ų������ ����
	UINT					provLen;			//����� ���̸�ŭ�� ������
	bool					bufRender;			//������������ �Ұ����� ����ϴ�.
	bool					delayRendOver;		//�������� �������� ��������

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

	//��Ƽ����Ʈ �������� ���ڿ��� �ε��ؿ´�.
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

	void render(FLOAT x, FLOAT y);																//x, y��ǥ�� �����������ϴ� brush�� ����
	void render(FLOAT x, FLOAT y, ColorF c);													//x, y��ǥ�� ColorF�� ���
	void render(FLOAT x, FLOAT y, ColorF c, DWRITE_TEXT_ALIGNMENT textAlignment);				//x, y��ǥ�� ColorF�� ���


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
#pragma once

class VisualBar
{
private:
	void* currentValue;
	void* maxValue;
	int Width, Height;

public:
	VisualBar();
	~VisualBar();

	HRESULT Create_VisualBar(void* current, void* max, int wid = 100, int hei = 10);
	void Render_VisualBar(FLOAT x, FLOAT y, ColorF c = { 0.0F, 0.0F, 0.0F, 255.0F });
};
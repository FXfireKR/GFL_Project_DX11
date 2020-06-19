#pragma once

class Effect
{
private:
	string key;

	float fps;
	float count;

	int FrameX, FrameY;
	int maxFrameX, maxFrameY;

	FLOAT OPC;

	D3DXVECTOR2 Scale;
	D3DXVECTOR3 Rotate;
	D3DXVECTOR2 Trans;

	D3DXVECTOR2	perSize;
	D3DXVECTOR2	minTexCoord;
	D3DXVECTOR2	maxTexCoord;

	bool EffectEnd;
	bool render;
	bool Flip;				// 0이 정방향, 1이 역방향

public:
	Effect();
	~Effect();

	HRESULT Create_Effect(string _key, D3DXVECTOR2 _pos, D3DXVECTOR2 _perSize, POINT _maxFrame, float _fps, float _scl = 1.0f, float _opc = 1.0f);
	void update();

private:
	void resetEffect();
	void updateEffect();

public:
	inline string getKey() { return key; }

	inline bool isEffectEnd() { return EffectEnd; }
	inline bool isRender() { return render; }

	inline int getFrameX() { return FrameX; }
	inline int getFrameY() { return FrameY; }
	inline void setFrameX(int x) { FrameX = x; }
	inline void setFrameY(int y) { FrameY = y; }

	inline bool isFlip() const { return Flip; }
	inline void setFlip(bool f) { Flip = f; }

	inline int getFrameMaxX() { return maxFrameX; }
	inline int getFrameMaxY() { return maxFrameY; }

	inline D3DXVECTOR2 getEffectScale() { return Scale; }
	inline D3DXVECTOR3 getEffectRotate() { return Rotate; }
	inline D3DXVECTOR2 getEffectTrans() { return Trans; }

	inline D3DXVECTOR2 getMinTexcoord() { return minTexCoord; }
	inline D3DXVECTOR2 getMaxTexcoord() { return maxTexCoord; }

	inline FLOAT getEffectOpc() { return OPC; }

	inline int getCounter() { return static_cast<int>(count); }
	inline float fgetCounter() { return count; }

	inline int getFramePerSec() { return static_cast<int>(fps); }
	inline float fgetFramePerSec() { return fps; }

	inline void setFramePerSec(int _fps) { fps = static_cast<float>(_fps); }
	inline void setFramePerSec(float _fps) { fps = _fps; }

	
};
#pragma once

class Effect
{
private:
	string key;

	float fps;
	float count;

	int FrameX, FrameY;
	int maxFrameX, maxFrameY;

	FLOAT posX, posY;
	FLOAT ACL, OPC;

	D3DXVECTOR2 Scale;
	D3DXVECTOR3 Rotate;
	D3DXVECTOR2 Trans;

	D3DXVECTOR2	perSize;
	D3DXVECTOR2	perSize_2;

	bool EffectEnd;
	bool render;
	bool Flip;				// 0이 정방향, 1이 역방향

public:
	Effect();
	~Effect();

	HRESULT Init_Effect();
	void Release_Effect();


	HRESULT Create_Effect(string k, int maxX, int maxY, float _fps, FLOAT acl = 1.0F, FLOAT opc = 1.0F);
	HRESULT Get_Rendorder(FLOAT x, FLOAT y);
	void Update_Effect();

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

	inline FLOAT getEffectAcl() { return ACL; }
	inline FLOAT getEffectOpc() { return OPC; }

	inline FLOAT getEffectPositionX() { return posX; }
	inline FLOAT getEffectPositionY() { return posY; }
	inline void setEffectPositionX(float x) { posX = x; }
	inline void setEffectPositionY(float y) { posY = y; }

	inline int getCounter() { return count; }

	inline int getFramePerSec() { return fps; }
	inline void setFramePerSec(int _fps) { fps = _fps; }

};
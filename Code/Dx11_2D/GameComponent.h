#ifndef GAMECOMP_H
#define GAMECOMP_H

struct uiAtlas
{
	string name;
	string textureKey;
	string alphaTexKey;
	D3DXVECTOR2 mixTexCoord;
	D3DXVECTOR2 maxTexCoord;
};

enum LOADRESOURCE_TYPE
{
	RESOURCE_SOUND = 0,
	RESOURCE_IMAGE,
	RESOURCE_MAP,
	RESOURCE_EQUIP,
	RESOURCE_TEXT
};

// 전역 함수
inline bool AABB(float x1, float y1, int sx1, int sy1,
	float x2, float y2, int sx2, int sy2)
{
	float L1 = x1 - sx1 * 0.5f;
	float R1 = x1 + sx1 * 0.5f;
	float T1 = y1 + sy1 * 0.5f;
	float B1 = y1 - sy1 * 0.5f;

	float L2 = x2 - sx2 * 0.5f;
	float R2 = x2 + sx2 * 0.5f;
	float T2 = y2 + sy2 * 0.5f;
	float B2 = y2 - sy2 * 0.5f;

	if (((L2 >= L1 && L2 <= R1) || (R2 >= L1 && R2 <= R1)) &&
		((B2 >= B1 && B2 <= T1) || (T2 >= B1 && T2 <= T1)))
		return true;

	return false;
}

inline bool CircleCollision(float x1, float y1, int sx1, int sy1,
	float x2, float y2, int sx2, int sy2)
{
	float disX = x1 - x2;
	float disY = y1 - y2;
	float distance = sqrtf(disX * disX + disY * disY);

	int radius1 = static_cast<int>((sx1 + sy1) * 0.25f);
	int radius2 = static_cast<int>((sx2 + sy2) * 0.25f);

	if (distance < radius1 + radius2)
		return true;

	return false;
}

inline bool PtInObject(D3DXVECTOR2 pos, D3DXVECTOR2 size, POINT pt)
{
	RECT rt;
	rt.left = static_cast<long>(pos.x - size.x * 0.5f);
	rt.right = static_cast<long>(pos.x + size.x * 0.5f);
	rt.top = static_cast<long>(pos.y + size.y * 0.5f);
	rt.bottom = static_cast<long>(pos.y - size.y * 0.5f);

	if (pt.x >= rt.left && pt.x <= rt.right &&
		pt.y >= rt.bottom && pt.y <= rt.top)
		return true;

	return false;
}

// 선형 보간
inline void LinearInterpolation(OUT float& x, OUT float& y,
	IN float fromX, IN float fromY, IN float toX, IN float toY,
	IN float t)
{
	x = fromX * (1.0f - t) + toX * t;
	y = fromY * (1.0f - t) + toY * t;
}

// 베지어 곡선 보간 (좌표 3개 사용 : 출발점, 계산점, 도착점)
inline void BezierInterpolation(OUT float& x, OUT float& y,
	IN float fromX, IN float fromY, IN float toX, IN float toY,
	IN float viaX, IN float viaY, IN float t)
{
	float x1, y1; // 1차 보간 지점
	float x2, y2; // 2차 보간 지점

	LinearInterpolation(x1, y1, fromX, fromY, viaX, viaY, t);
	LinearInterpolation(x2, y2, viaX, viaY, toX, toY, t);
	LinearInterpolation(x, y, x1, y1, x2, y2, t);
}

// 베지어 곡선 보간 (좌표 4개 사용 : 출발점, 계산점, 도착점)
inline void BezierInterpolation2(OUT float& x, OUT float& y,
	IN float fromX, IN float fromY, IN float toX, IN float toY,
	IN float via1X, IN float via1Y,
	IN float via2X, IN float via2Y, IN float t)
{
	float x1, y1; // 1차 보간 지점
	float x2, y2; // 2차 보간 지점
	float x3, y3; // 2차 보간 지점

	LinearInterpolation(x1, y1, fromX, fromY, via1X, via1Y, t);
	LinearInterpolation(x2, y2, via1X, via1Y, via2X, via2Y, t);
	LinearInterpolation(x3, y3, via2X, via2Y, toX, toY, t);

	LinearInterpolation(x1, y1, x1, y1, x2, y2, t);
	LinearInterpolation(x2, y2, x2, y2, x3, y3, t);

	LinearInterpolation(x, y, x1, y1, x2, y2, t);
}

#define GRAVITY 3.0f
#define JUMP_POWER 40.0f

// 이넘문
enum class Ani { Idle, Walk_F, Walk_B, Jump, Max };

inline bool SphereCollision(D3DXVECTOR3& dir, D3DXVECTOR3 p1, float r1, D3DXVECTOR3 p2, float r2)
{
	dir = p2 - p1;

	// 두점 사이의 거리
	float dist = D3DXVec3Length(&dir);

	if (r1 + r2 > dist)
	{
		D3DXVec3Normalize(&dir, &dir);
		return true;
	}

	return false;
}

#endif
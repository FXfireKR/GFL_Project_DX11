#pragma once
#include "spineMotion.h"

class Helicopter
{
protected:
	static bool				flip;
	static bool				deploy;
	static D2D1_RECT_F		hitBox;

	static float			rAngle;
	static float			moveSpeed;

	static Vector2			targetPosition;
	static Vector2			position;
	static Vector2			drawPosition;

	static spineMotion*		heliMotion[2];

	static size_t			curHelicopterKey;

private:
	Helicopter() {}
	~Helicopter() {}

public:
	static void init();
	static void release();
	static void CallHelicopter(Vector2 _position, bool isThermal = false);
	static void UpdateHelicopter();
	static void RenderHelicopter();

	static bool getDeploy() { return deploy; }

protected:
	static const int a = 10;
};
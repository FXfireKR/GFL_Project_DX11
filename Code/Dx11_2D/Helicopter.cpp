#include "stdafx.h"
#include "Helicopter.h"

bool Helicopter::flip = false;

D2D1_RECT_F Helicopter::hitBox = D2DRectMake(0, 0, 0, 0);

float Helicopter::rAngle = 0.0f;
float Helicopter::moveSpeed = 0.0f;

Vector2 Helicopter::targetPosition = Vector2(0, 0);
Vector2 Helicopter::position = Vector2(0, 0);
Vector2 Helicopter::drawPosition = Vector2(0, 0);

spineMotion* Helicopter::heliMotion[2] = { nullptr, };


void Helicopter::init()
{
	if (heliMotion[0] == NULL || heliMotion[0] == nullptr) {
		heliMotion[0] = new spineMotion;
		heliMotion[0]->loadSpine_FromJsonFile("Helicopter2");
	}

	if (heliMotion[1] == NULL || heliMotion[1] == nullptr) {
		heliMotion[1] = new spineMotion;
		heliMotion[1]->loadSpine_FromJsonFile("Helicopter3");
	}

	flip = false;
	hitBox = D2DRectMake(0, 0, 0, 0);

	rAngle = 0.0f;
	moveSpeed = 0.0f;
}

void Helicopter::release()
{
}

void Helicopter::CallHelicopter(Vector2 _position, bool isThermal)
{
	
}
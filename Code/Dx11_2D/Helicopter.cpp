#include "stdafx.h"
#include "Helicopter.h"

bool Helicopter::flip = false;
bool Helicopter::deploy = false;

D2D1_RECT_F Helicopter::hitBox = D2DRectMake(0, 0, 0, 0);

float Helicopter::rAngle = 0.0f;
float Helicopter::moveSpeed = 0.0f;
float Helicopter::distance = 0.0f;
float Helicopter::altitude = 0.0f;

size_t Helicopter::curHelicopterKey = 100;

Vector2 Helicopter::targetPosition = Vector2(0, 0);
Vector2 Helicopter::position = Vector2(0, 0);
Vector2 Helicopter::drawPosition = Vector2(0, 0);

spineMotion* Helicopter::heliMotion[2] = { nullptr, };


void Helicopter::init()
{
	if (heliMotion[0] == nullptr) {
		heliMotion[0] = new spineMotion;
		heliMotion[0]->loadSpine_FromJsonFile("Helicopter2");
	}

	if (heliMotion[1] == nullptr) {
		heliMotion[1] = new spineMotion;
		heliMotion[1]->loadSpine_FromJsonFile("Helicopter3");
	}

	flip = false;
	deploy = false;
	hitBox = D2DRectMake(0, 0, 0, 0);

	rAngle = 0.0f;
	moveSpeed = 0.0f;

	curHelicopterKey = 0;
	position = Vector2(0, 0);
	targetPosition = Vector2(0, 0);
}

void Helicopter::release()
{
	if (heliMotion[0] != nullptr) {
		delete heliMotion[0];
		heliMotion[0] = nullptr;
	}

	if (heliMotion[1] != nullptr) {
		delete heliMotion[1];
		heliMotion[1] = nullptr;
	}
}

void Helicopter::CallHelicopter(Vector2 _position, bool isThermal)
{
	targetPosition = _position;
	position = Vector2(-1000, -1000);
	distance = getDistance(position, targetPosition);

	moveSpeed = DELTA() * 100.0f;
	altitude = 1;
	deploy = false;
	flip = false;

	curHelicopterKey = isThermal ? 1 : 0;

	if (heliMotion[curHelicopterKey]->getCurrentMotionKey().compare("fly") != 0)
		heliMotion[curHelicopterKey]->changeMotion("fly", true);
}

void Helicopter::UpdateHelicopter()
{
	if (curHelicopterKey < 2) {

		if (distance > 50.0f) {
			moveSpeed += DELTA() * 1.0f;
			if (moveSpeed > 50.0f)
				moveSpeed = 50.0f;
		}

		else {
			moveSpeed -= DELTA() * 1.0f;
			if (moveSpeed < 0.0f)
				moveSpeed = 0.0f;

			if (ptInRect(hitBox, targetPosition)) {
				deploy = true;
			}
		}

		rAngle = getAngle(position, targetPosition);

		position.x += cosf(rAngle) * moveSpeed;
		position.y -= sinf(rAngle) * moveSpeed;

		heliMotion[curHelicopterKey]->p_getRotate().z = DGR(rAngle);
		heliMotion[curHelicopterKey]->p_getTrans().x = position.x;
		heliMotion[curHelicopterKey]->p_getTrans().y = position.y;

		heliMotion[curHelicopterKey]->update(DELTA());
	}
}

void Helicopter::RenderHelicopter()
{
	if (curHelicopterKey < 2) 
		heliMotion[curHelicopterKey]->render();
}
#include "stdafx.h"
#include "TestScene.h"

TestScene::TestScene()
	: temp(nullptr)
{
	if (temp == nullptr) {
		temp = new nAnOne;
		temp->LoadTray_SoundList();
	}
}

TestScene::~TestScene()
{
	if (temp != nullptr) {
		temp->release();
		SAFE_DELETE(temp);
	}
}

void TestScene::init()
{
	temp->LoadTray_ImageList();
	temp->init();

	LOAD->setAutoInit(false);
	LOAD->setNextScene("TEST");
	LOAD->setLoadImageKey("Logo");
	LOAD->mallocThread();

	SCENE->changeScene("LOAD");
}

void TestScene::release()
{
}

void TestScene::update()
{
	temp->update();

	// test Spine Logic
	static spineMotion* motion = temp->getMotion();
	static int motionCounter = 0;
	static int motionLimit = motion->base->getSkeleton()->data->animationsCount - 1;
	static float mixTimer = 0.0f;
	static bool isMix = false;
	static bool isLoop = false;
	static bool changeMotion = false;
	static float spineDelta = DELTA();

	//temp->Update_DrawPos();
	//motion->update(spineDelta);

	ImGui::Text("CurrentTime %.3f", motion->getCurTime());
	ImGui::Text("EndTime %.3f", motion->getEndTime());
	ImGui::Text("TimeScale %.3f", motion->base->getTrackEntry()->timeScale);

	ImGui::Text("%s", motion->base->getSkeleton()->data->animations[motionCounter]->name);
	ImGui::SameLine(150);
	ImGui::InputInt(" ", &motionCounter);

	if (motionCounter < 0)
		motionCounter = 0;
	else if (motionCounter > motionLimit)
		motionCounter = motionLimit;

	ImGui::Checkbox("spine Mix", &isMix);
	ImGui::Checkbox("spine Loop", &isLoop);

	ImGui::Checkbox("ChangeMotion", &changeMotion);

	ImGui::DragFloat("spine Delta", &spineDelta, 0.01f, 0.001f, 0.8f);
	ImGui::DragFloat("spine mixTimer", &mixTimer, 0.01f, 0.001f, 0.2f);

	if (changeMotion) {
		motion->changeMotion(motion->base->getSkeleton()->data->animations[motionCounter]->name, isLoop, isMix, mixTimer);
		changeMotion = !changeMotion;
	}
}

void TestScene::render()
{
	temp->render_Motion();
}
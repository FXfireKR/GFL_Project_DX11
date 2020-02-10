#include "stdafx.h"
#include "MainScene.h"

MainScene::MainScene()
{
	loader = new BTM_Loader;
	btm = loader->LoadBTM("../../_Assets/bg2.btm");

	delete loader;
}

MainScene::~MainScene()
{
	SAFE_DELETE(ak12);
	SAFE_DELETE(gua91);
}

void MainScene::init() 
{
	ak12 = new spineMotion;
	ak12->loadSpine_FromJsonFile("ak12");

	ak12->p_getScale().x = 0.8;
	ak12->p_getScale().y = 0.8;
	ak12->p_getTrans().x = 80.5f;
	ak12->p_getTrans().y = 20.5f;

	ak12->setMotionAction("attack", attackMotion);
	ak12->setMotionObject(this);
	ak12->init();

	gua91 = new spineMotion;
	gua91->loadSpine_FromJsonFile("9A91");

	gua91->p_getScale().x = 0.8;
	gua91->p_getScale().y = 0.8;
	gua91->p_getTrans().x = 2.5f;
	gua91->p_getTrans().x = 150.5f;
	gua91->p_getTrans().y = 20.5f;

	gua91->init();

	IMAGEMAP->InsertImage_FromeFile("bg2", "../../_Assets/bg2.png");

	aimTime = AK12_AIM_TIME;
}

void MainScene::release()
{
}

void MainScene::update()
{	
	if (g_pKeyManger->isStayKeyDown('W'))
		ak12->changeMotion("attack", true, true, 0.125f);

	if (g_pKeyManger->isStayKeyDown('D'))
		ak12->changeMotion("attack2", true, true, 0.125f);
	
	else if (g_pKeyManger->isStayKeyDown('A'))
	{
		ak12->changeMotion("wait", true, true, 0.125f);
		aimTime = AK12_AIM_TIME;
	}
	else if (g_pKeyManger->isStayKeyDown('S'))
	{
		ak12->changeMotion("move", true, true, 0.125f);
		aimTime = AK12_AIM_TIME;
	}

	btm->update();

	if (ak12->isCurrent("attack") || ak12->isCurrent("attack2"))
	{
		if (aimTime > 0)
		{
			ak12->pause(0);
			aimTime -= static_cast<double>(Delta);
		}
	}

	ak12->update(Delta);
	gua91->update(Delta);
}

void MainScene::render()
{
	btm->render();
	gua91->render();
	ak12->render();
}

void MainScene::attackMotion(void * obj)
{
	MainScene* object = (MainScene*)obj;

	if (object->ak12->getCurTime() > object->ak12->getEndTime())
		object->aimTime = object->AK12_AIM_TIME;
}
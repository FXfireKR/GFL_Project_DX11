#include "stdafx.h"
#include "BattleScene.h"

BattleScene::BattleScene()
{
	BDATA->init();
	//PLAYER->test_setting();
}

BattleScene::~BattleScene()
{
	BDATA->delInstance();
}

void BattleScene::init()
{
	//PLAYER->test_create();
	BULLET->init();
	PLAYER->test_setting();

	for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())		//	get List of Player's TacticalDoll
	{
		it.second->LoadTray_SoundList();
		it.second->LoadTray_ImageList();
	}

	LOADMANAGER->setAutoInit(false);
	LOADMANAGER->setNextScene("BATTLE");
	SCENE->Change_Scene("LOAD");
}

void BattleScene::release()
{
	BULLET->release();
}

void BattleScene::update()
{
	CAMERA->setCameraFix(false);

	PLAYER->update();
	BTLMAP->update();
	BULLET->update();

	for (TaticDoll* iter : BDATA->getCurrUnits())
		iter->update();

	for (auto& iter : PLAYER->getPlayerSquad(PLAYER->getCurrentSquad())->mSquad)
		iter.second->update();
	
	ZOrder_Sort();

	EFFECT->update();
}

void BattleScene::render()
{
	//BTLMAP->testRender(vRendList);

	BULLET->render();

	for (auto& it : vRendList)
	{
		if (it.x < 100)
			PLAYER->getIOPdoll_crntSquad(it.y)->render_Ellipse();
		else
			BDATA->getObject(it.y)->render_Ellipse();
	}

	for (auto& it : vRendList)
	{
		if (it.x < 100)
		{
			PLAYER->getIOPdoll_crntSquad(it.y)->render();
			PLAYER->getIOPdoll_crntSquad(it.y)->render_Motion();
		}
		else
		{
			BDATA->getObject(it.y)->render();
			BDATA->getObject(it.y)->render_Motion();
			BDATA->getObject(it.y)->render_VisualBar();
		}
	}

	PLAYER->render();

	EFFECT->render();
}

void BattleScene::ZOrder_Sort()
{
	static float count = 0.0f;

	if (count < FLOAT_EPSILON)
	{
		mRendList.clear();
		vRendList.clear();

		//�׸����д� ����
		for (auto& grf : PLAYER->getPlayerSquad(PLAYER->getCurrentSquad())->mSquad)
		{
			//if (!grf.second->getAlive())continue;

			POINT Sqd;	//x�� �д�, y�� ���
			Sqd.x = PLAYER->getCurrentSquad();
			Sqd.y = grf.first;

			if (!mRendList.count(grf.second->getCharacterPos().y))
			{
				map<FLOAT, POINT> temp;
				temp.insert(make_pair(grf.second->getCharacterPos().x, Sqd));
				mRendList.insert(make_pair(grf.second->getCharacterPos().y, temp));
			}

			else
				mRendList.find(grf.second->getCharacterPos().y)->second.insert(make_pair(grf.second->getCharacterPos().x, Sqd));
		}

		//ö���д� ����
		for (size_t i = 0; i < BDATA->getCurrUnits().size(); ++i)
		{
			TaticDoll* snv = BDATA->getObject(i);

			POINT Sqd;	//x�� �д�, y�� ��� [ö���д�� ���� x���� 100�� ���Ѵ�]
			Sqd.x = 100;
			Sqd.y = i;

			if (!mRendList.count(snv->getCharacterPos().y))
			{
				map<FLOAT, POINT> temp;
				temp.insert(make_pair(snv->getCharacterPos().x, Sqd));
				mRendList.insert(make_pair(snv->getCharacterPos().y, temp));
			}

			else
				mRendList.find(snv->getCharacterPos().y)->second.insert(make_pair(snv->getCharacterPos().x, Sqd));

		}

		for (auto& it : mRendList)
			for (auto& it2 : it.second)
				vRendList.push_back(it2.second);

		count = ZORDER_REFRESH_TIME;
	}
	else
		count -= DELTA;
}

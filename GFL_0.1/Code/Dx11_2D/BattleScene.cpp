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
	DAMAGE->loadImageList();
	DAMAGE->AllocateMemory();

	LOADMANAGER->Add_LoadTray("arSound", "../../_SoundSource/Battle_AR.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray("mgSound1", "../../_SoundSource/Battle_170.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray("mgSound2", "../../_SoundSource/Battle_171.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray("srSound1", "../../_SoundSource/Battle_179.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray("srSound2", "../../_SoundSource/Battle_180.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOADMANAGER->Add_LoadTray("srSound3", "../../_SoundSource/Battle_181.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	LOADMANAGER->Add_LoadTray(BTLMAP->imgKey, BTLMAP->imgPath, LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	//PLAYER->test_create();
	BULLET->init();
	PLAYER->test_setting();

	for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())		//	get List of Player's TacticalDoll
	{
		it.second->LoadTray_SoundList();
		it.second->LoadTray_ImageList();
		PLAYER->insertTacDolToSquad(it.first, 1);
	}

	Pyxis* test = new Pyxis;
	test->init();
	test->getID()->SquadMem_ID = 0;
	BDATA->insertObject(test);

	Strelet* test2 = new Strelet;
	test2->init();
	test2->getID()->SquadMem_ID = 1;
	BDATA->insertObject(test2);

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
	SOUNDMANAGER->update();
	DAMAGE->update();
}

void BattleScene::render()
{
	BTLMAP->testRender(vRendList);

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
			PLAYER->getIOPdoll_crntSquad(it.y)->render_VisualBar();
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

	DAMAGE->render();
}

void BattleScene::ZOrder_Sort()
{
	static float count = 0.0f;

	if (count < FLOAT_EPSILON)
	{
		mRendList.clear();
		vRendList.clear();

		//그리폰분대 정렬
		for (auto& grf : PLAYER->getPlayerSquad(PLAYER->getCurrentSquad())->mSquad)
		{
			//if (!grf.second->getAlive())continue;

			POINT Sqd;	//x는 분대, y는 멤버
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

		//철혈분대 정렬
		for (size_t i = 0; i < BDATA->getCurrUnits().size(); ++i)
		{
			TaticDoll* snv = BDATA->getObject(i);

			POINT Sqd;	//x는 분대, y는 멤버 [철혈분대는 기존 x값에 100을 더한다]
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

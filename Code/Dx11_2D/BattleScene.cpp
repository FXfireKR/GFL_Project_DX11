#include "stdafx.h"
#include "BattleScene.h"

BattleScene::BattleScene()
{
	BDATA->init();
	//PLAYER->test_setting();
}

BattleScene::~BattleScene()
{
	BDATA->releaseInstance();
}

void BattleScene::init()
{
	DAMAGE->loadImageList();
	DAMAGE->AllocateMemory();

	LOAD->Add_LoadTray("arSound", "../../_SoundSource/Battle_AR.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("mgSound1", "../../_SoundSource/Battle_170.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("mgSound2", "../../_SoundSource/Battle_171.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("mgReload", "../../_SoundSource/mgReload.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("srSound1", "../../_SoundSource/Battle_179.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("srSound2", "../../_SoundSource/Battle_180.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("srSound3", "../../_SoundSource/Battle_181.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("sgSound", "../../_SoundSource/SG_shot.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("sgReload", "../../_SoundSource/SG_re.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	LOAD->Add_LoadTray("SG_BLT", "../../_Assets/Texture2D/fire1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE, 5, 1);
	LOAD->Add_LoadTray("sgmm", "../../_Assets/Texture2D/slugmm.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("AR_BLT", "../../_Assets/Texture2D/riflebullet.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("MG_BLT", "../../_Assets/Texture2D/mgBullet.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	LOAD->Add_LoadTray(BTLMAP->imgKey, BTLMAP->imgPath, LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	//PLAYER->test_create();
	BULLET->init();

	//	get List of Player's TacticalDoll
	for (auto& it : PLAYER->getPlayerTaticDoll().getAllDolls())		
	{
		it.second->LoadTray_SoundList();
		it.second->LoadTray_ImageList();
	//	PLAYER->insertTacDolToSquad(it.first, 1);
	}

	for (auto& it : BDATA->getCurrUnits())
	{
		it->LoadTray_SoundList();
		it->LoadTray_ImageList();
	}

	vRendList.clear();

	LOAD->setAutoInit(false);
	LOAD->setNextScene("BATTLE");

	LOAD->mallocThread();
	SCENE->changeScene("LOAD");

	resultTimer = 0.0f;
	battleOver = false;
}

void BattleScene::release()
{
	BULLET->release();
}

void BattleScene::update()
{
	CAMERA->setCameraFix(false);

	BTLMAP->update();
	BULLET->update();

	for (BaseTaticDoll* iter : BDATA->getCurrUnits())
		iter->update();

	for (auto& iter : PLAYER->getPlayerSquad(PLAYER->getCurrentSquad())->squadMember)
		iter.second->update();

	if (!battleOver)
	{
		PLAYER->update();
		ZOrder_Sort();
		check_BattlePlag();
	}

	else
	{
		resultTimer -= DELTA();
		if (resultTimer - DELTA() < DELTA())
		{
			resultTimer = 0.0f;

			DAMAGE->release();
			BULLET->release();
			BDATA->getSquadSNV()->callSquad(BDATA->getEngageSquadID())->AllDead = true;
			SCENE->changeScene("WORLD");
			SCENE->initScene("WORLD");
		}
	}

	EFFECT->update();
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

		// 아군 분대 정렬
		for (auto& grf : PLAYER->getPlayerSquad(PLAYER->getCurrentSquad())->squadMember)
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

		// 적군 분대 정렬
		for (size_t i = 0; i < BDATA->getCurrUnits().size(); ++i)
		{
			BaseTaticDoll* snv = BDATA->getObject(i);

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
		count -= DELTA();
}

void BattleScene::check_BattlePlag()
{
	//	Check Battle Plag
	if (MAP->getMissionFlag().battlePlag & ANNIHILATION && MAP->getMissionFlag().protectObjectNumber == 0)
	{
		bool enemyDie = true;
		for (auto enemy : BDATA->getCurrUnits())
		{
			if (enemy->getAlive())
			{
				enemyDie = false;
				break;
			}
		}

		if (enemyDie)
		{
			battleOver = true;
			resultTimer = 240.0f * DELTA();	
		}

		if (!battleOver)
		{
			for (auto& ali : PLAYER->getPlayerSquad(PLAYER->getCurrentSquad())->squadMember)
			{
				if (ali.second->getAlive())
				{
					enemyDie = false;
					break;
				}
			}

			if (enemyDie)
			{
				SCENE->changeScene("WORLD");
				SCENE->initScene("WORLD");
			}
		}
	}
}

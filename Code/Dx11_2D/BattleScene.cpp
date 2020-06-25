#include "stdafx.h"
#include "BattleScene.h"

BattleScene::BattleScene()
{
	BDATA->init();
	DWRITE->Create_TextField("CONQUER", L"맑은고딕", "N/A", 40, DWRITE_FONT_WEIGHT_BOLD);

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

	LOAD->Add_LoadTray("arSound", "Battle_AR.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("mgSound1", "Battle_170.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("mgSound2", "Battle_171.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("mgReload", "mgReload.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("srSound1", "Battle_179.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("srSound2", "Battle_180.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("srSound3", "Battle_181.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("sgSound", "SG_shot.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	LOAD->Add_LoadTray("sgReload", "SG_re.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	LOAD->Add_LoadTray("SG_BLT", "Texture2D/fire1.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE, 5, 1);
	LOAD->Add_LoadTray("sgmm", "Texture2D/slugmm.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("AR_BLT", "Texture2D/riflebullet.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOAD->Add_LoadTray("MG_BLT", "Texture2D/mgBullet.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

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

	if (conquerArea.size() > 0) {
		for (auto& iter : conquerArea) {
			if (iter != nullptr) {
				iter->Release_Ellipse();
				SAFE_DELETE(iter);
			}
		}
		conquerArea.clear();
	}

	size_t protectObjSize;
	if ((protectObjSize = MAP->getMissionFlag().protectObjectNumber) != 0) {

		conquerArea.reserve(protectObjSize);

		for (size_t i = 0; i < MAP->getMissionFlag().protectObjectNumber; ++i) {
			conquerArea.push_back(new EllipseBase(&(MAP->pGetMissionFlag().protectArea[i].AreaCenter.x), &(MAP->pGetMissionFlag().protectArea[i].AreaCenter.y),
				MAP->pGetMissionFlag().protectArea[i].AreaRaius.x, MAP->pGetMissionFlag().protectArea[i].AreaRaius.y));
		}
	}
	conquerTimer = MAP->getMissionFlag().battlePlag & PROTECT_AREA ? MAX_CONQUER_TIME : 0.0f;

	LOAD->setAutoInit(false);
	LOAD->setNextScene("BATTLE");

	LOAD->mallocThread();
	SCENE->changeScene("LOAD");

	resultTimer = 0.0f;
	battleOver = false;

	floatingTimer = 0.0f;
	highLightScale = 0.0f;
	nextFloatTimer = 0.0f;
	highlight = false;
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

	//	점령일경우 활성화
	if (conquerArea.size() > 0) {
		floatingTimer = floatingTimer > 0.0f ? floatingTimer - DELTA() : 0.0f;
		highLightScale = highLightScale > 0.0f ? highLightScale - 0.5f : 0.0f;

		float val = 40 + highLightScale;
		string str = DWRITE->getProperty("CONQUER")->getText();
		DWRITE->Create_TextField("CONQUER", L"맑은고딕", str, val, DWRITE_FONT_WEIGHT_BOLD);
	}
}

void BattleScene::render()
{
	BTLMAP->testRender(vRendList);

	for (auto& iter : conquerArea) {
		iter->Rend_Ellipse(D3DXCOLOR(0, 1, 0, 0.5));
	}

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

	if (MAP->getMissionFlag().battlePlag & CONQUER_AREA || MAP->getMissionFlag().battlePlag & PROTECT_AREA) {
		
		if (conquerTimer > 0.0f)
			conquerColor = ColorF(0.0f, 0.5f, 0.5f);

		else if (conquerTimer < 0.0f)
			conquerColor = ColorF(0.8f, 0.0f, 0.0f);

		D2D->renderRect(D2DRectMake(WINSIZEX*0.5, 150, conquerTimer, 50), conquerColor, true);

		if (conquerState == CONQUERSTATE::ENEMY_CONQUERING ||
			conquerState == CONQUERSTATE::REPLACE_CONQUER)
			DWRITE->TextRender("CONQUER", 0, 10, WINSIZEX, 200, conquerTextColor, DWRITE_TEXT_ALIGNMENT_CENTER);
		
	}
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
	//	======== Check Battle Plag ========  //


	//	@ 섬멸
	if (MAP->getMissionFlag().battlePlag & ANNIHILATION)
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


	//	@ 지역 방어
	if (MAP->getMissionFlag().battlePlag & PROTECT_AREA)
	{
		if (conquerTimer <= M_MAX_CONQUER_TIME) {
			battleOver = true;
			resultTimer = 240.0f * DELTA();
		}

		else {
			int conq = 0;
			bool replacing = false;
			conquerState = CONQUERSTATE::NON_CONQUERING;
			DWRITE->ChangeText("CONQUER", " ");

			//	적군이 방어지역을 점령중인가?
			for (auto& iter : conquerArea) {
				for (auto& enemy : BDATA->getCurrUnits()) {
					if (!enemy->getAlive()) continue;
					if (iter->PointCollision_Check(enemy->getCharacterPos().x, 
						enemy->getCharacterPos().y)) {
						++conq;
						replacing = true;
					}
				}

				for (auto& alli : PLAYER->getPlayerSquad(PLAYER->getCurrentSquad())->squadMember) {
					if (!alli.second->getAlive()) continue;
					if (iter->PointCollision_Check(alli.second->getCharacterPos().x, 
						alli.second->getCharacterPos().y)) {
						--conq;
						replacing = true;
					}
				}
			}

			if (conq >= static_cast<int>(conquerArea.size())) {

				conquerState = CONQUERSTATE::ENEMY_CONQUERING;
				DWRITE->ChangeText("CONQUER", "적군이 점령중입니다 !");
				conquerTextColor = ColorF(0.9, 0.1, 0, 1);
				if (highLightScale < DELTA())
					highLightScale = 20.0f;

				conquerTimer -= DELTA() * conq * 10.0f;
			}

			else {
				if (conq < 0) {
					if (conquerTimer < MAX_CONQUER_TIME) {
						conquerTimer += DELTA() * fabsf(static_cast<float>(conq)) * 10.0f;
						conquerState = CONQUERSTATE::FRIENDLY_CONQUERING;
					}
					else {
						conquerTimer = MAX_CONQUER_TIME;
						conquerState = CONQUERSTATE::FRIENDLY_CONQ_COMPLETE;
					}
				}
				else {
					if (replacing) {
						conquerState = CONQUERSTATE::REPLACE_CONQUER;
						DWRITE->ChangeText("CONQUER", "점령구역 대치중입니다 !");
						conquerTextColor = ColorF(0.9, 0.9, 0, 1);
						if (highLightScale < DELTA())
							highLightScale = 20.0f;
					}
				}
			}
		}
	}


	//	@ 지역 점령



	//	@ 유닛 보호
}

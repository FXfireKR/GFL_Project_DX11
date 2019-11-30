#include "stdafx.h"
#include "TestScene.h"

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
}

HRESULT TestScene::init()
{
	PLAYER->testinst();

	Jaeger* t = new Jaeger;
	t->init();
	BDATA->getSquadSNV()->Insert_SquadMember(1, t);

	img = new Image("bk_1");
	img->init();
	//img->setScale(D3DXVECTOR2(1, 0.8));
	img->setTrans(D3DXVECTOR2(960,540));
	
	//motion = new Motion;
	////motion->Create_Motion("NTW20_Attack", 340, 250, 0.05f);

	//ntw20 = new NTW_20;
	//ntw20->init();

	//c = new DXCircle;
	//eb = new EllipseBase;
	//eb->Create_Ellipse(&ntw20->getPosX(), &ntw20->getPosY(), 450, 150);

	return S_OK;
}

void TestScene::release()
{
	if (img != nullptr)
	{
		img->release();
		SAFE_DEL(img);
	}

	/*if (motion != nullptr)
	{
		motion->release();
		SAFE_DEL(motion);
	}

	if (ntw20 != nullptr)
	{
		ntw20->release();
		SAFE_DEL(ntw20);
	}

	if (c != nullptr)
	{
		c->release();
		SAFE_DEL(c);
	}

	if (eb != nullptr)
	{
		eb->Release_Ellipse();
		SAFE_DEL(eb);
	}*/
}

void TestScene::update()
{

	ImGui::Text("FPS : %.2f", FPSTIMER->getFPS());
	if (KEYMANAGER->isKeyStayDown(VK_LEFT))
		VPOS->x -= 450 * DELTA;

	else if (KEYMANAGER->isKeyStayDown(VK_RIGHT))
		VPOS->x += 450 * DELTA;

	

	img->update();

	D3DXCOLOR c = img->getColor();

	ImGui::DragFloat("back_R", &c.r, 0.01F, 0.0F, 1.0F);
	ImGui::DragFloat("back_G", &c.g, 0.01F, 0.0F, 1.0F);
	ImGui::DragFloat("back_B", &c.b, 0.01F, 0.0F, 1.0F);
	ImGui::DragFloat("back_A", &c.a, 0.01F, 0.0F, 1.0F);

	img->setColor(c);

	//motion->Update_Motion();
	//ntw20->update();
	//c->update();
	PLAYER->update();

	for (auto& it : (BDATA->getEnemySquad(BDATA->getEngageSquadID()))->mSquad)
		it.second->update();

	BEFCT->update();
	EFFECT->update();

	ZOrder_Sort();
}

void TestScene::render()
{
	img->render();
	PLAYER->render();

	/*for (auto& it : (BDATA->getEnemySquad(BDATA->getEngageSquadID()))->mSquad)
		it.second->render();*/

	for (auto& it : vRendList)
	{
		//Ã¶Ç÷ ºÐ´ëÀÎ°¡
		if (it.x > 100)
			BDATA->getEnemyMember(it.x - 100, it.y)->render_Ellipse();

		else
			PLAYER->getIOPdoll_crntSquad(it.y)->render_Ellipse();
	}

	for (auto& it : vRendList)
	{
		//Ã¶Ç÷ ºÐ´ëÀÎ°¡
		if (it.x > 100) 
		{
			BDATA->getEnemyMember(it.x - 100, it.y)->render_Motion();
			BDATA->getEnemyMember(it.x - 100, it.y)->render_VisualBar();
		}

		else
		{
			PLAYER->getIOPdoll_crntSquad(it.y)->render_Motion();
			PLAYER->getIOPdoll_crntSquad(it.y)->render_VisualBar();
		}
	}

	//for (auto& it : vRendList)
	//{
	//	//Ã¶Ç÷ ºÐ´ëÀÎ°¡
	//	if (it.x > 100)
	//		

	//	else
	//		
	//}

	BEFCT->render();
	EFFECT->render();
}

void TestScene::ZOrder_Sort()
{
	static float count = 0.0f;

	if (count < FLOAT_EPSILON)
	{
		mRendList.clear();
		vRendList.clear();

		//±×¸®ÆùºÐ´ë Á¤·Ä
		for (auto& grf : PLAYER->getPlayerSquad(PLAYER->getCurrentSquad())->mSquad)
		{
			//if (!grf.second->getAlive())continue;

			POINT Sqd;	//x´Â ºÐ´ë, y´Â ¸â¹ö
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

		//Ã¶Ç÷ºÐ´ë Á¤·Ä
		for (auto& snv : BDATA->getEnemySquad(BDATA->getEngageSquadID())->mSquad)
		{
			//if (!snv.second->getAlive())continue;

			POINT Sqd;	//x´Â ºÐ´ë, y´Â ¸â¹ö [Ã¶Ç÷ºÐ´ë´Â ±âÁ¸ x°ª¿¡ 100À» ´õÇÑ´Ù]
			Sqd.x = BDATA->getEngageSquadID() + 100;
			Sqd.y = snv.first;

			if (!mRendList.count(snv.second->getCharacterPos().y))
			{
				map<FLOAT, POINT> temp;
				temp.insert(make_pair(snv.second->getCharacterPos().x, Sqd));
				mRendList.insert(make_pair(snv.second->getCharacterPos().y, temp));
			}

			else
				mRendList.find(snv.second->getCharacterPos().y)->second.insert(make_pair(snv.second->getCharacterPos().x, Sqd));
		}

		for (auto& it : mRendList)
			for (auto& it2 : it.second)
				vRendList.push_back(it2.second);

		count = ZORDER_REFRESH_TIME;
	}
	else
		count -= DELTA;

	ImGui::DragFloat("counter", &count);
}
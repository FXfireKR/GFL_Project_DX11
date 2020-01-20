#include "stdafx.h"
#include "MainLoadScene.h"

MainLoadScene::MainLoadScene()
{
	//image = new Image("LoadBK");
	//image->setTrans(D3DXVECTOR2(WINSIZEX*0.5f, WINSIZEY*0.5f));
	//image->setScale(D3DXVECTOR2());
	
}

MainLoadScene::~MainLoadScene()
{
	//if (image != nullptr)
	//{
	//	image->release();
	//	SAFE_DEL(image);
	//}
}

void MainLoadScene::init()
{
	//To Loading List
	//LOADMANAGER->setAutoInit(false);

	LOADMANAGER->setAutoInit(true);
	LOADMANAGER->setNextScene("LOBBY");
}

void MainLoadScene::release()
{
}

void MainLoadScene::update()
{
	{
		LOADMANAGER->update();
		image->update();

		if (!LOADMANAGER->getAutoInit())
		{
			if (LOADMANAGER->getLoadListSize() < 1)
			{
				SCENE->Change_Scene(LOADMANAGER->getNextScene());
				SCENE->Init_Scene();
			}
		}
	}

}

void MainLoadScene::ImageUpdate()
{

}

void MainLoadScene::render()
{
	DRAW->render("LoadBK", DV2(WINSIZEX, WINSIZEY), DV2(WINSIZEX*0.5f, WINSIZEY*0.5f));
}
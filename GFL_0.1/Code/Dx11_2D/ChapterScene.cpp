#include "stdafx.h"
#include "ChapterScene.h"

ChapterScene::ChapterScene()
{
}

ChapterScene::~ChapterScene()
{
}

void ChapterScene::init()
{
}

void ChapterScene::release()
{
}

void ChapterScene::update()
{
	if (worldColor.a < 1.0f)
	{
		worldColor.a += DELTA;
		SOUNDMANAGER->setVolume(SOUND_CHANNEL::CH_SOUND1, worldColor.a < 0.15f ? worldColor.a : 0.15f);
	}

	else
	{
		worldColor.a = 1.0f;

		if (KEYMANAGER->isKeyDown(VK_LBUTTON))
		{
			for (auto& it : mButton)
			{
				if (ptInRect(it.second.box, g_ptMouse))
					it.second.ClickAction(this);
			}
		}
	}
}

void ChapterScene::render()
{
}

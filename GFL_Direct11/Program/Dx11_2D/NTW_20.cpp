#include "stdafx.h"
#include "NTW_20.h"

NTW_20::NTW_20()
{
	motion = new MotionManager;
}

NTW_20::~NTW_20()
{
}

void NTW_20::LoadTrayList()
{
	//SOUND RESOURCE
	LOADMANAGER->Add_LoadTray("FIRE_SR1", "../../_SoundSource/Battle_181.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);


	//IMAGE RESOURCE
	LOADMANAGER->Add_LoadTray("NTW20", "../../_Textures/NTW20/NTW20.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20_IDLE", "../../_Textures/NTW20/Idle.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20_ATTACK", "../../_Textures/NTW20/Attack.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20_DEATH", "../../_Textures/NTW20/Death.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20_SKILL", "../../_Textures/NTW20/Skill.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20_WALK", "../../_Textures/NTW20/Walk.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20_VICTORY", "../../_Textures/NTW20/Victory.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	LOADMANAGER->Add_LoadTray("NTW20_VICLOOP", "../../_Textures/NTW20/VicLoop.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
}

void NTW_20::init()
{
	motion->Create("IDLE", "NTW20_IDLE", 390, 340, 0.001F);
	motion->Create("WALK", "NTW20_WALK", 390, 340, 0.0F);
	motion->getMotion("WALK")->setCountGrav(5.0F);

	motion->Create("ATTACK", "NTW20_ATTACK", 390, 340, 0.001F);
	motion->Create("SKILL", "NTW20_SKILL", 390, 340, 0.015F);
	motion->Create("DEATH", "NTW20_DEATH", 390, 340, 0.015F);
	motion->Create("VICTORY", "NTW20_VICTORY", 390, 340, 0.015F);
	motion->Create("VICLOOP", "NTW20_VICLOOP", 390, 340, 0.015F);

	motion->RegisterFunction("ATTACK", NTW20_AttackAction, this);
	motion->RegisterFunction("VICTORY", NTW20_VictoryAction, this);

	motion->Change_Motion("IDLE");

	angle = 0.0f;
	//pos.x = pos.y = 0.0f;
	pos.x = WINSIZEX * 0.5F;
	pos.y = WINSIZEY * 0.5F;
}

void NTW_20::release()
{
	if (motion != nullptr)
	{
		motion->release();
		SAFE_DEL(motion);
	}
}

void NTW_20::update()
{
	if (KEYMANAGER->isKeyDown(VK_SPACE))
		motion->Change_Motion("ATTACK");

	if (KEYMANAGER->isKeyDown('C'))
		motion->Change_Motion("VICTORY");

	if (KEYMANAGER->isKeyDown('Z'))
		motion->Change_Motion("DEATH");

	if (KEYMANAGER->isKeyDown('X'))
		motion->Change_Motion("SKILL");

	if (KEYMANAGER->isKeyDown(VK_RIGHT))
	{
		angle = 0.0f;
		motion->Change_Motion("WALK");
	}

	if (KEYMANAGER->isKeyDown(VK_LEFT))
	{
		angle = 180.0f;
		motion->Change_Motion("WALK");
	}
	
	if (motion->getCurrentMotion()->getIsOver() && motion->isCurrent("ATTACK"))
	{
		motion->Change_Motion("IDLE");
	}

	motion->update();
}

void NTW_20::render()
{
	motion->render(pos, D3DXVECTOR2(1.5f, 1.5f), 1.0F, angle);
}

void NTW_20::NTW20_AttackAction(void * ob)
{
	NTW_20* Object = (NTW_20*)ob;
	int frame = Object->getMotionManager().getCurrent_Frame().x;

	switch (frame)
	{
	case 3:
		//if (Object->getMotionManager().getCurrentMotion()->getIsOnce())
			//SOUNDMANAGER->Play_Sound(SOUND_CHANNEL::CH_EFFECT, "FIRE_SR1", 0.15f);	
		break;
	}
}

void NTW_20::NTW20_VictoryAction(void * ob)
{
	NTW_20* Object = (NTW_20*)ob;

	if (Object->getMotionManager().getCurrentMotion()->getIsOver())
		Object->getMotionManager().Change_Motion("VICLOOP");
}
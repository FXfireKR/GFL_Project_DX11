#include "stdafx.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()
{
	//	Plag Setting
	curTacdollState = 0x0000;

	Initialize = false;
	lockCameraMove = true;
	lockConv = false;
	questComplete = false;

	curTacDoll = nullptr;

}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::init()
{
	if (!Initialize)
	{
		PLAYER->test_setting();
		BULLET->init();

		SerifOv = false;
		Next = false;
  		textCount = 0;
		opc = 0.0f;

		vCon.clear();
		curSound = "STOP";
		bkKey = "";

		TEXT->Change_Text("CONV", "");
		TEXT->Change_Text("CONV_fix", "");
		TEXT->Change_Text("NAME", "");

		LOADMANAGER->setStoryScriptPath("TutorialScript");
		readScript();

		LOADMANAGER->Add_LoadTray("arSound", "../../_SoundSource/Battle_AR.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
		LOADMANAGER->Add_LoadTray("mgSound1", "../../_SoundSource/Battle_170.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
		LOADMANAGER->Add_LoadTray("mgSound2", "../../_SoundSource/Battle_171.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);
	
		Drone::LoadImageList();

		//	Setting 
		if (curTacDoll == nullptr)
		{
			for (auto& it : PLAYER->getPlayerTaticDoll().getAllTacDoll())
			{
				if (it.second->getName().compare("M4SopMod") == 0)
				{
					it.second->init();
					curTacDoll = it.second;
					break;
				}
			}
		}

		curTacDoll->LoadTray_ImageList();

		PLAYER->insertTacDolToSquad(curTacDoll->getID()->All_ID, 1);

		curTacDoll->p_getCharacterPos()->x = 150;
		curTacDoll->p_getCharacterPos()->y = WINSIZEY * 0.5f;

		LOADMANAGER->Add_LoadTray(BTLMAP->imgKey, BTLMAP->imgPath, LOADRESOURCE_TYPE::RESOURCE_IMAGE);

		//	UnLoad List
		vLoadList.clear();
		for (auto& it : LOADMANAGER->getLoadList())
			vLoadList.push_back(LoadResourceData(it->key, it->type));

		LOADMANAGER->setAutoInit(false);
		LOADMANAGER->setNextScene("TUTORIAL");
		SCENE->Change_Scene("LOAD");

  		Initialize = true;

		curTacdollState = 0x0000;
		lockCameraMove = true;
		lockConv = false;
		questComplete = true;
		PLAYER->reset();
	}
}

void TutorialScene::release()
{
}

void TutorialScene::update()
{
	CAMERA->setCameraFix(true);
	CAMERA->CameraReset();

	if (Initialize)
	{		
		curTacDoll->update();
		questUpdate();

		if (curTacdollState & TATICDOOL_CONTROLL)
		{
			PLAYER->update();

			if (curTacdollState & TATICDOOL_MOVE)
			{
				curTacDoll->setMoveAble(true);
				PLAYER->clearAlianceList();
			}

			else
				curTacDoll->setMoveAble(false);
			

			if (!(curTacdollState & TATICDOOL_SKILL))
				curTacDoll->pGetSklColTime() = 5.0f;
			else
			{
				curTacDoll->setMoveAble(false);
				curTacDoll->p_getCharacterPos()->x = WINSIZEX * 0.45f;
				curTacDoll->p_getCharacterPos()->y = WINSIZEY * 0.6f;
			}
		}

		else
		{
			if (PLAYER->getAlianceList().size() < 1)
				PLAYER->setAliance(ALIANCE_NONE);
		}
		

		//PLAYER->update();
		BTLMAP->update();
		BULLET->update();

		for (TaticDoll* iter : BDATA->getCurrUnits())
			iter->update();

		ZOrder_Sort();

		EFFECT->update();

		scriptUpdate();

		DAMAGE->update();
	}
}

void TutorialScene::render()
{
	if (Initialize)
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

		scriptRender();
	}
}

void TutorialScene::questUpdate()
{
	if (questComplete)
	{
		curTacdollState = TATICDOOL_NONE;
		lockConv = false;
		lockCameraMove = true;
	}

	else
	{
		lockConv = true;
		lockCameraMove = false;

		if (str_compare(Quest, "Drag"))
		{
			if (PLAYER->getCurFocusDoll() != nullptr)
				questComplete = true;
		}

		else if (str_compare(Quest, "Move"))
		{
			if (curTacDoll->getMotion()->isCurrent("move"))
				questComplete = true;
		}

		else if (str_compare(Quest, "Destory"))
		{
			bool allDead = true;
			for (size_t i = 0; i < BDATA->getObjectVector().size(); ++i)
			{
				if (BDATA->getObject(i)->getAlive())
				{
					allDead = false;
					break;
				}
			}

			if (allDead)
				questComplete = true;
		}
	}
}

void TutorialScene::unLoad()
{
	BULLET->release();

	for (auto& it : vLoadList)
	{
		switch (it.type)
		{
		case RESOURCE_IMAGE:
			IMAGEMAP->ReleaseMemory(it.resourceKey);
			break;
		}
	}

	Initialize = false;

	//	delete Squad Member
	PLAYER->deleteTacDolToSquad(curTacDoll->getID()->SquadMem_ID, curTacDoll->getID()->Squad_ID);

	BDATA->deleteAll();
	vRendList.clear();
}

void TutorialScene::scriptUpdate()
{
	if (!lockConv)
	{
		if (textCount < vCon.size() - 1)
		{
			if (TEXT->getProperty("CONV")->getdelayRendOver())
			{
				if (KEYMANAGER->isKeyUp(VK_SPACE))
				{
					if (textCount > 0 && textCount < vCon.size())
					{
						if (vCon.at(textCount).SpeakName.compare(vCon.at(textCount - 1).SpeakName))
							opc = 0.0f;
					}

					if (vCon.at(textCount).bkKey.compare("N/A") != 0)
						bkKey = vCon.at(textCount).bkKey;
					else
						bkKey = "";

					TEXT->Create_TextField("NAME", TUTORIAL_TEXT_FONT, vCon.at(textCount).SpeakName, 30.0f);
					TEXT->Create_TextField("CONV", TUTORIAL_TEXT_FONT, vCon.at(textCount).text, 20.0f);
					TEXT->Create_TextField("CONV_fix", TUTORIAL_TEXT_FONT, vCon.at(textCount).text, 20.0f);

					TEXT->getProperty("CONV")->setbufRender();
					TEXT->getProperty("CONV")->DelayText(1);

					if (vCon.at(textCount).curSound.size() > 0)
					{
						if (vCon.at(textCount).curSound.compare("STOP") != 0)
							//	SOUNDMANAGER->Stop_All();

							//else
						{
							curSound = vCon.at(textCount).curSound;
							SOUNDMANAGER->Play_Sound(SOUND_CHANNEL::CH_SOUND2, curSound, 0.15f);
						}
					}

					else
					{
						if (curSound.size() > 0)
						{
							SOUNDMANAGER->Play_Sound(SOUND_CHANNEL::CH_SOUND2, curSound, 0.15f);
						}
					}

					

					if (vQuest.size() > 0)
					{
						if (textCount == vQuest.begin()->convIndex + 1)
						{
							curTacdollState = vQuest.begin()->plagCode;
							Quest = vQuest.begin()->Quest;
							vQuest.erase(vQuest.begin());
							questComplete = false;
						}
					}

					if (vSpawn.size() > 0)
					{
						if (textCount == vSpawn.begin()->convIndex + 1)
						{
							BDATA->deleteAll();

							for (vSpawn.begin()->spawnNum; vSpawn.begin()->spawnNum > 0; --vSpawn.begin()->spawnNum)
							{
								Drone* _new = new Drone;
								_new->init();
								_new->p_getCharacterPos()->x = vSpawn.begin()->coord[vSpawn.begin()->spawnNum - 1].x;
								_new->p_getCharacterPos()->y = vSpawn.begin()->coord[vSpawn.begin()->spawnNum - 1].y;
								BDATA->insertObject(_new);
							}

							vSpawn.erase(vSpawn.begin());
						}
					}

					++textCount;
						
				}
			}


		}

		else
		{
			if (KEYMANAGER->isKeyUp(VK_SPACE))
			{
				unLoad();

				LOADMANAGER->setAutoInit(true);
				LOADMANAGER->setStoryScriptPath("ChapterScript_0_1");
				LOADMANAGER->setNextScene("STORY");
				SCENE->Change_Scene("LOAD");
				Initialize = false;

				SOUNDMANAGER->Stop_Sound(SOUND_CHANNEL::CH_SOUND2, curSound);
			}
		}

		opc = opc < 1.0F ? opc += DELTA * 2.0f : opc = 1.0F;
	}
}

void TutorialScene::scriptRender()
{
	if (!lockConv)
	{
		if (bkKey.size() > 0)
			DRAW->render(bkKey, DV2(WINSIZEX*0.5f, WINSIZEY*0.5f), D3DXVECTOR2(WINSIZEX*0.5F, WINSIZEY*0.5F));

		if (textCount != 0)
		{
			if (vCon.at(textCount - 1).vImageKey.size() > 0)
			{
				string speaker = vCon.at(textCount - 1).vImageKey.at(0);
				string speaker_alpha;

				if (speaker.find("pic_") != string::npos)
				{
					speaker_alpha.append(speaker, 0, speaker.find("("));
					speaker_alpha.append("_alpha");
				}

				else
				{
					string isValid = speaker;
					isValid.append("_alpha");
					if (IMAGEMAP->isValidKey(isValid))
						speaker_alpha = "alpha";
					else
						speaker_alpha = isValid;
				}

				DRAW->render(speaker, speaker_alpha, DV2(512 * 0.7f, 512 * 0.7f),
					DV2(WINSIZEX * 0.5F, WINSIZEY * 0.7F), D3DXCOLOR(1, 1, 1, opc));

			}
		}

		uiAtlas atlas = IMAGEMAP->getUiAtlas("ConvBackImage");
		DRAW->render(atlas.textureKey, atlas.alphaTexKey, DV2(375, 100), DV2(TUTORIAL_BOX_POS_X + 375, TUTORIAL_BOX_POS_Y + 80),
			atlas.mixTexCoord, atlas.maxTexCoord, D3DXCOLOR(1, 1, 1, 0.8f));

		TEXT->TextRender("NAME", TUTORIAL_BOX_POS_X + 20 + 10, TUTORIAL_BOX_POS_Y - 15 + 10, { 255.0F, 255.0F, 0.0F });

		//DRAWMANAGER->renderRect(D2D_RectMake(TEXTBOX_POSX + 20, TEXTBOX_POSY + 50, TEXTBOX_POSX + 20 + TEXTBOX_WIDTH - 40, TEXTBOX_POSY + 50 + TEXTBOX_HEIGH), ColorF(1, 0, 0));

		if (!TEXT->getProperty("CONV")->getdelayRendOver())
		{
			TEXT->TextRender("CONV", TUTORIAL_BOX_POS_X + 20, TUTORIAL_BOX_POS_Y + 50,
				TUTORIAL_TEXT_WIDTH - 40, TUTORIAL_TEXT_HEIGHT, { 255.0F, 255.0F, 255.0F });

			SOUNDMANAGER->Play_Sound(SOUND_CHANNEL::CH_SOUND1, "type", 0.3F);
		}

		else
		{
			TEXT->TextRender("CONV_fix", TUTORIAL_BOX_POS_X + 20, TUTORIAL_BOX_POS_Y + 50,
				TUTORIAL_TEXT_WIDTH - 40, TUTORIAL_TEXT_HEIGHT, { 255.0F, 255.0F, 255.0F });

			SOUNDMANAGER->Stop_Sound(SOUND_CHANNEL::CH_SOUND1, "type");
		}
	}
}

void TutorialScene::readScript()
{
	ifstream file;
	string path;
	path.append("../../_TextTable/");
	path.append(LOADMANAGER->getStoryScriptPath());
	path.append(".txt");

	file.open(path.c_str());


	string buffer;
	wstring buf;

	while (!file.eof())
	{
		buffer.clear();

		Convers temp;
		temp.bkKey = "N/A";
		getline(file, buffer);

		if (buffer.size() < 2)continue;

		//	메인 대상 찾기
		if (buffer.find("<Speaker>") != string::npos)
		{
			int pos = buffer.find("<Speaker>") + 9;
			int pos2 = buffer.find("</Speaker>") - pos;
			temp.SpeakName.append(buffer, pos, pos2);
		}

		//	배경 찾기
		if (buffer.find("<BIN>") != string::npos)
		{
			temp.bkKey.clear();

			string path;
			int pos = buffer.find("<BIN>") + 5;
			int pos2 = buffer.find("</BIN>") - pos;
			temp.bkKey.append(buffer, pos, pos2);

			/*path = "../RemakeResource/Resource/";
			path.append(temp.bkKey);
			path += ".png";*/

			path = "../../_Assets/CG/";
			path.append(temp.bkKey);
			path += ".ab";

			LOADMANAGER->Add_LoadTray(temp.bkKey, path, LOADRESOURCE_TYPE::RESOURCE_IMAGE);

			//if (DRAW->getBitmap(temp.bkKey) == nullptr)
			//	DRAW->LoadImage_d2d(temp.bkKey, path);
		}

		//	BGM
		if (buffer.find("<BGM>") != string::npos)
		{
			temp.curSound.clear();

			string path;
			int pos = buffer.find("<BGM>") + 5;
			int pos2 = buffer.find("</BGM>") - pos;
			temp.curSound.append(buffer, pos, pos2);

			if (temp.curSound.compare("STOP") != 0)
			{
				path = "../../_SoundSource/";
				path.append(temp.curSound);
				path += ".ab";

				LOADMANAGER->Add_LoadTray(temp.curSound, path, LOADRESOURCE_TYPE::RESOURCE_SOUND);

				//sound->Insert_Sound(temp.curSound, path.c_str());
			}
			//sound->Play_Sound(temp.bkKey, 0.15f);
		}
		else
			temp.curSound = "STOP";

		//	화자 이미지 찾기
		if (buffer.find("(") != string::npos)
		{
			int pos = buffer.find("(");
			if (pos < 1)	//()옆에 뭐가없다는뜻
				temp.vImageKey.clear();
			else
			{
				string key, path;

				if (buffer.find("(0)") != string::npos)
				{
					string imgKey = "pic_";
					key.append(buffer, 0, buffer.find(")") + 1);
					imgKey.append(buffer, 0, buffer.find("("));
					temp.vImageKey.push_back(imgKey);

					path = "../../_Assets/Characters/";
					path.append(key, 0, key.find("("));
					path += "/";
					path.append(imgKey, 0, imgKey.size());
					path += ".ab";
					LOADMANAGER->Add_LoadTray(imgKey, path, LOADRESOURCE_TYPE::RESOURCE_IMAGE);

					imgKey.append("_alpha");
					path.insert(path.size() - 3, "_Alpha");
					LOADMANAGER->Add_LoadTray(imgKey, path, LOADRESOURCE_TYPE::RESOURCE_IMAGE);
				}

				else
				{
					key.append(buffer, 0, buffer.find(")") + 1);
					temp.vImageKey.push_back(key);

					path = "../../_Assets/Characters/";
					path.append(key, 0, key.find("("));
					path += "/";
					path.append(key, 0, key.size());
					path += ".ab";
					LOADMANAGER->Add_LoadTray(key, path, LOADRESOURCE_TYPE::RESOURCE_IMAGE);
				}


				//if (DRAW->getBitmap(key) == nullptr)
				//	DRAW->LoadImage_d2d(key, path);
			}
		}

		//대사찾기
		if (buffer.find(":") != string::npos)
		{
			int pos = buffer.find(":") + 1;
			string AllText;
			AllText.append(buffer, pos, buffer.size());

			//같은 조건으로 만들어버리는게 존재한다면
			if (AllText.find("+") != string::npos)
			{
				while (AllText.find("+") != string::npos)
				{
					int pos2 = AllText.find("+");
					temp.text.clear();
					temp.text.append(AllText, 0, pos2);
					vCon.push_back(temp);

					AllText.erase(0, pos2 + 1);
				}

				int pos2 = AllText.find("+");
				temp.text.clear();
				temp.text.append(AllText, 0, pos2);
			}

			else
				temp.text = AllText;
		}

		//	Story모드 종료시 행동 요령
		if (buffer.find("《") != string::npos)
		{
			//	1. 다음 Scene 배정
			if (buffer.find("scene_"))
			{
				nextScene.clear();
				nextScene.append(buffer, buffer.find_first_of("_") + 1, buffer.find_last_of('\n'));
			}

			//	2. 읽어들일 FileName 배정
			else if (buffer.find("map_"))
			{
				nextReadFile.clear();
				nextReadFile = "../../_TextTable/";
				nextReadFile.append(buffer, buffer.find_first_of("_") + 1, buffer.find_last_of('\n'));
				nextReadFile.append("txt");
			}
		}


		//	Tutorial 모드 Plag와 Quest 설정
		else if (buffer.find("》") != string::npos)
		{		
			string value;

			//	1. Plag 상태 변경
			if (buffer.find("plag :") != string::npos)
			{
				TutorialConvExtra newConv;

				value.append(buffer, buffer.find(":") + 1, buffer.size());
 				newConv.plagCode = atoi(value.c_str());
				newConv.convIndex = vCon.size() - 1;
				vQuest.push_back(newConv);
			}

			//	2. 퀘스트 배정
			else if (buffer.find("quest :") != string::npos)
			{
				value.append(buffer, buffer.find(":") + 1, buffer.size());
				if (value.find("\n") != string::npos)
					value.erase(value.find("\n"), value.size());
				vQuest[vQuest.size() - 1].Quest = value;
			}

			//	3. 적군 배치 & 편성
			else if (buffer.find("add :") != string::npos)
			{
				TutorialSpawn spawn;
				spawn.convIndex = vCon.size() - 1;
				spawn.coord.reserve(spawn.convIndex);

				//	3-1. 소환 갯수
				value.append(buffer, buffer.find(":") + 1, buffer.size());
				string temp; temp.append(value, 0, value.find_first_of(","));
				value.erase(0, value.find_first_of(",") + 1);
				spawn.spawnNum = atoi(temp.c_str());

				for (int i = 0; i < spawn.spawnNum; ++i)
				{
					//	3-2. X, Y 좌표
					D3DXVECTOR2 spawnPos;

					temp.clear();
					temp.append(value, 0, value.find(","));
					value.erase(0, value.find_first_of(",") + 1);
					spawnPos.x = atoi(temp.c_str());

					temp.clear();
					temp.append(value, 0, value.find(","));
					value.erase(0, value.find_first_of(",") + 1);
					spawnPos.y = atoi(temp.c_str());

					spawn.coord.push_back(spawnPos);
				}

				vSpawn.push_back(spawn);
			}
		}

		else
			vCon.push_back(temp);
	}
}

void TutorialScene::ZOrder_Sort()
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

#include "stdafx.h"
#include "StoryScene.h"

StoryScene::StoryScene()
{
	DWRITE->Create_TextField("NAME", TEXT_FONT, " ", 25.0f);
	DWRITE->Create_TextField("CONV", TEXT_FONT, " ", 20.0f);
	DWRITE->Create_TextField("CONV_fix", TEXT_FONT, " ", 20.0f);

	DWRITE->getProperty("CONV")->setbufRender();
	DWRITE->getProperty("CONV")->DelayText(1);

	//LOADMANAGER->Add_LoadTray("bk_Speak", "../../_Textures/bk_speak.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	//LOADMANAGER->Add_LoadTray("bk_Speak", "../../_Textures/DialogueBorder.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);
	//LOADMANAGER->Add_LoadTray("type", "../../_SoundSource/UI_numberChange.ab", LOADRESOURCE_TYPE::RESOURCE_SOUND);

	Initialize = false;
}

StoryScene::~StoryScene()
{
}

void StoryScene::init()
{
	if (!Initialize)
	{
		SerifOv = false;
		Next = false;
		textCount = 0;
		opc = 0.0f;

		vCon.clear();
		curSound = "STOP";
		bkKey = "";

		DWRITE->Change_Text("CONV", "");
		DWRITE->Change_Text("CONV_fix", "");
		DWRITE->Change_Text("NAME", "");

		nextReadFile.clear();
		nextScene.clear();

		ReadScript();

		//	UnLoad List
		vLoadList.clear();
		for (auto& it : LOAD->getLoadList())
			vLoadList.push_back(LoadResourceData(it->key, it->type));

		LOAD->setAutoInit(false);

		LOAD->mallocThread();
		SCENE->changeScene("LOAD");
		Initialize = false;
	}
}

void StoryScene::release()
{
}

void StoryScene::update()
{
	if (worldColor.a < 1.0f)
		worldColor.a += DELTA();

	else
	{
		worldColor.a = 1.0f;

		if (textCount < vCon.size() - 1)
		{
			if (DWRITE->getProperty("CONV")->getdelayRendOver())
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

					DWRITE->Create_TextField("NAME", TEXT_FONT, vCon.at(textCount).SpeakName, 30.0f);
					DWRITE->Create_TextField("CONV", TEXT_FONT, vCon.at(textCount).text, 20.0f);
					DWRITE->Create_TextField("CONV_fix", TEXT_FONT, vCon.at(textCount).text, 20.0f);

					DWRITE->getProperty("CONV")->setbufRender();
					DWRITE->getProperty("CONV")->DelayText(1);

					if (vCon.at(textCount).curSound.size() > 0)
					{
						if (vCon.at(textCount).curSound.compare("STOP") != 0)
							//	SOUNDMANAGER->Stop_All();

							//else
						{
							curSound = vCon.at(textCount).curSound;
							SOUND->Play_Sound(SOUND_CHANNEL::CH_SOUND2, curSound, 0.15f);
						}
					}

					else
					{
						if (curSound.size() > 0)
							SOUND->Play_Sound(SOUND_CHANNEL::CH_SOUND2, curSound, 0.15f);
						
					}

					++textCount;
				}

				//스토리 스킵
				/*if (KEYMANAGER->isKeyUp('S'))
				{
					SCENE->Change_Scene("EQUIP");
					SCENE->Init_Scene();
					SOUNDMANAGER->Stop_Sound(SOUND_CHANNEL::CH_SOUND2, curSound);
				}*/
			}

		}

		else
		{
			if (KEYMANAGER->isKeyUp(VK_SPACE))
			{
				//LOADMANAGER->setAutoInit(true);
				//LOADMANAGER->setNextScene(nextScene);

				unLoad();
				SCENE->changeScene(nextScene);
				SCENE->initScene(nextScene);

				/*LOADMANAGER->setAutoInit(true);
				LOADMANAGER->setStoryScriptPath("TestScript2");
				LOADMANAGER->setNextScene("STORY");
				LOADMANAGER->setLoadImageKey("bg1");
				SCENE->Change_Scene("LOAD");*/

				/*SCENE->Change_Scene("EQUIP");
				SCENE->Init_Scene();*/
				SOUND->Stop_Sound(SOUND_CHANNEL::CH_SOUND2, curSound);
			}
		}

		opc = opc < 1.0F ? opc += DELTA() * 2.0f : opc = 1.0F;
	}
}

void StoryScene::render()
{
	DRAW->render(bkKey, VEC2(WINSIZEX, WINSIZEY), D3DXVECTOR2(WINSIZEX*0.5F, WINSIZEY*0.5F));

	if (textCount != 0)
	{
		if (vCon.at(textCount - 1).vImageKey.size() > 0)
		{
			string speaker = vCon.at(textCount - 1).vImageKey.at(0);
			string speaker_alpha;
			if (speaker.find("pic_") != string::npos)
			{
				//speaker_alpha = "pic_";
				speaker_alpha.append(speaker, 0, speaker.find("("));
				speaker_alpha.append("_alpha");
			}
			else
				speaker_alpha = "alpha";

			DRAW->render(speaker, speaker_alpha, VEC2(1024 * TEXT_SPEAKER_ACL, 1024 * TEXT_SPEAKER_ACL),
				VEC2(WINSIZEX * 0.5F, WINSIZEY * 0.7F), D3DXCOLOR(1, 1, 1, opc));

		}
	}

	//DRAW->render("bk_Speak", DV2(750, 200), D3DXVECTOR2(TEXTBOX_POSX + 375, TEXTBOX_POSY + 80), DV3(0, 0, 0), D3DXCOLOR(1, 1, 1, 0.8F));
	//DRAW->render("bk_Speak", TEXTBOX_POSX, TEXTBOX_POSY, 1.0F, 0.8F);

	const uiAtlas* atlas = IMAGEMAP->getUiAtlas("ConvBackImage");
	DRAW->render(atlas->textureKey, atlas->alphaTexKey, VEC2(375, 100), VEC2(TEXTBOX_POSX + 375, TEXTBOX_POSY + 80),
		atlas->mixTexCoord, atlas->maxTexCoord, D3DXCOLOR(1, 1, 1, worldColor.a < 0.8f ? worldColor.a : 0.8f));

	DWRITE->TextRender("NAME", TEXTBOX_POSX + 20 + 10, TEXTBOX_POSY - 15 + 10, { 255.0F, 255.0F, 0.0F });

	//DRAWMANAGER->renderRect(D2D_RectMake(TEXTBOX_POSX + 20, TEXTBOX_POSY + 50, TEXTBOX_POSX + 20 + TEXTBOX_WIDTH - 40, TEXTBOX_POSY + 50 + TEXTBOX_HEIGH), ColorF(1, 0, 0));

	if (!DWRITE->getProperty("CONV")->getdelayRendOver())
	{
		DWRITE->TextRender("CONV", TEXTBOX_POSX + 20, TEXTBOX_POSY + 50, TEXTBOX_WIDTH - 40, TEXTBOX_HEIGH, { 255.0F, 255.0F, 255.0F });
		SOUND->Play_Sound(SOUND_CHANNEL::CH_SOUND1, "type", 0.3F);
	}

	else
	{
		DWRITE->TextRender("CONV_fix", TEXTBOX_POSX + 20, TEXTBOX_POSY + 50, TEXTBOX_WIDTH - 40, TEXTBOX_HEIGH, { 255.0F, 255.0F, 255.0F });
		SOUND->Stop_Sound(SOUND_CHANNEL::CH_SOUND1, "type");
	}
}

void StoryScene::unLoad()
{
	for (auto& it : vLoadList)
	{
		switch (it.type)
		{
		case RESOURCE_IMAGE:
			IMAGEMAP->ReleaseMemory(it.resourceKey);
			break;
		}
	}
}

void StoryScene::ReadScript()
{
	ifstream file;
	string path;
	path.append("_TextTable/");
	path.append(LOAD->getStoryScriptPath());
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

			path = "CG/";
			path.append(temp.bkKey);
			path += ".ab";

			LOAD->Add_LoadTray(temp.bkKey, path, LOADRESOURCE_TYPE::RESOURCE_IMAGE);

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
				path = "";
				path.append(temp.curSound);
				path += ".ab";

				LOAD->Add_LoadTray(temp.curSound, path, LOADRESOURCE_TYPE::RESOURCE_SOUND);

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

					path = "Characters/";
					path.append(key, 0, key.find("("));
					path += "/";
					path.append(imgKey, 0, imgKey.size());
					path += ".ab";
					LOAD->Add_LoadTray(imgKey, path, LOADRESOURCE_TYPE::RESOURCE_IMAGE);

					imgKey.append("_alpha");
					path.insert(path.size() - 3, "_Alpha");
					LOAD->Add_LoadTray(imgKey, path, LOADRESOURCE_TYPE::RESOURCE_IMAGE);
				}

				else
				{
					key.append(buffer, 0, buffer.find(")") + 1);
					temp.vImageKey.push_back(key);

					path = "Characters/";
					path.append(key, 0, key.find("("));
					path += "/";
					path.append(key, 0, key.size());
					path += ".ab";
					LOAD->Add_LoadTray(key, path, LOADRESOURCE_TYPE::RESOURCE_IMAGE);
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
			if (buffer.find("scene_") != string::npos)
			{
				nextScene.clear();
				nextScene.append(buffer, buffer.find_first_of("_") + 1, buffer.find_last_of('\n'));
			}

			//	2. 읽어들일 FileName 배정
			else if (buffer.find("map_") != string::npos)
			{
				nextReadFile.clear();
				nextReadFile = "_TextTable/";
				nextReadFile.append(buffer, buffer.find_first_of("_") + 1, buffer.find_last_of('\n'));
				nextReadFile.append("txt");
			}

			else if (buffer.find("script_") != string::npos)
			{
				nextReadFile.clear();
				nextReadFile = "_TextTable/";
				nextReadFile.append(buffer, buffer.find_first_of("_") + 1, buffer.find_last_of('\n'));
				nextReadFile.append("txt");
			}
		}
		else
			vCon.push_back(temp);
	}
}

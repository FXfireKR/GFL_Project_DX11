#include "stdafx.h"
#include "StoryScene.h"

StoryScene::StoryScene()
{
	TEXT->Create_TextField("NAME", TEXT_FONT, " ", 25.0f);
	TEXT->Create_TextField("CONV", TEXT_FONT, " ", 20.0f);
	TEXT->Create_TextField("CONV_fix", TEXT_FONT, " ", 20.0f);

	TEXT->getProperty("CONV")->setbufRender();
	TEXT->getProperty("CONV")->DelayText(1);

	LOADMANAGER->Add_LoadTray("bk_Speak", "../../_Textures/bk_speak.ab", LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	img = new Image("NULL");

	Initialize = false;
}

StoryScene::~StoryScene()
{
}

HRESULT StoryScene::init()
{
	if (!Initialize) 
	{
		SerifOv = false;
		Next = false;
		textCount = 0;

		//sound->Insert_Sound("type", "../SoundData/UI_numberChange.wav");
		//DRAW->LoadImage_d2d("bk_Speak", "../RemakeResource/bk_speak.png");

		ReadScript();
		

		LOADMANAGER->setAutoInit(false);
		SCENE->Change_Scene("FirstLoad");
		Initialize = true;
	}
	return S_OK;
}

void StoryScene::release()
{
	Initialize = false;
}

void StoryScene::update()
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

				TEXT->Create_TextField("NAME", TEXT_FONT, vCon.at(textCount).SpeakName, 30.0f);
				TEXT->Create_TextField("CONV", TEXT_FONT, vCon.at(textCount).text, 20.0f);
				TEXT->Create_TextField("CONV_fix", TEXT_FONT, vCon.at(textCount).text, 20.0f);

				TEXT->getProperty("CONV")->setbufRender();
				TEXT->getProperty("CONV")->DelayText(1);

				if (vCon.at(textCount).curSound.size() > 0)
				{
					//if (vCon.at(textCount).curSound.compare("STOP") == 0)
						//SOUNDMANAGER->->Stop_All();

					//else
					//{
						//curSound = vCon.at(textCount).curSound;
						//sound->Play_Sound(curSound, 0.15f);
					//}
				}

				else
				{
					//if (curSound.size() > 0)
					//{
						//sound->Play_Sound(curSound, 0.15f);
					//}
				}

				++textCount;
			}

			//스토리 스킵
			//if (KEY->KeyUp('S'))
			//	SCENE->Change_Scene("Squad_Edit");
		}

	}

	else
	{
		//if (KEYMANAGER->isKeyUp(VK_SPACE))
		//	SCENE->Change_Scene("Squad_Edit");
	}

	opc = opc < 1.0F ? opc += DELTA * 0.8F : opc = 1.0F;

	ImGui::DragFloat("OPC", &opc);
}

void StoryScene::render()
{

	img->setTextureKey(bkKey);
	img->render(D3DXVECTOR2(WINSIZEX, WINSIZEY), D3DXVECTOR3(0, 0, 0), D3DXVECTOR2(WINSIZEX*0.5F, WINSIZEY*0.5F), D3DXCOLOR(1, 1, 1, 1));


	//if (DRAWMANAGER->getBitmap(bkKey) != nullptr)

	//	DRAWMANAGER->render(bkKey, 0, 0, WINSIZEX, WINSIZEY);

	if (textCount != 0)
	{
		if (vCon.at(textCount - 1).vImageKey.size() > 0)
		{
			img->setTextureKey(vCon.at(textCount - 1).vImageKey.at(0));
			img->render(D3DXVECTOR2(1024 * TEXT_SPEAKER_ACL, 1024 * TEXT_SPEAKER_ACL), D3DXVECTOR3(0, 0, 0), D3DXVECTOR2(WINSIZEX * 0.5F, WINSIZEY * 0.7F), D3DXCOLOR(1, 1, 1, opc));
			//if (DRAW->getBitmap(vCon.at(textCount - 1).vImageKey.at(0)) != nullptr)
			//	DRAW->render(vCon.at(textCount - 1).vImageKey.at(0), WINSIZEX * 0.1428F, WINSIZEY * 0.2F, TEXT_SPEAKER_ACL, opc);
		}
	}

	img->setTextureKey("bk_Speak");
	img->render(D3DXVECTOR2(750, 200), D3DXVECTOR3(0, 0, 0), D3DXVECTOR2(TEXTBOX_POSX + 375, TEXTBOX_POSY + 80), D3DXCOLOR(1, 1, 1, 0.8F));
	//DRAW->render("bk_Speak", TEXTBOX_POSX, TEXTBOX_POSY, 1.0F, 0.8F);
	TEXT->TextRender("NAME", TEXTBOX_POSX + 20, TEXTBOX_POSY - 15, { 255.0F, 255.0F, 0.0F });

	if (!TEXT->getProperty("CONV")->getdelayRendOver())
	{
		TEXT->TextRender("CONV", TEXTBOX_POSX + 20, TEXTBOX_POSY + 50, TEXTBOX_WIDTH - 40, TEXTBOX_HEIGH, { 255.0F, 255.0F, 255.0F });
		//sound->Play_Sound("type", 0.3F);
	}

	else
	{
		TEXT->TextRender("CONV_fix", TEXTBOX_POSX + 20, TEXTBOX_POSY + 50, TEXTBOX_WIDTH - 40, TEXTBOX_HEIGH, { 255.0F, 255.0F, 255.0F });
		//sound->Stop_Sound("type");
	}
}

void StoryScene::ReadScript()
{
	ifstream file;
	file.open("../../_TextTable/TestScript2.txt");

	string buffer;
	wstring buf;

	while (!file.eof())
	{
		buffer.clear();

		Convers temp;
		temp.bkKey = "N/A";
		getline(file, buffer);

		if (buffer.size() < 2)continue;

		//메인 대상 찾기
		if (buffer.find("<Speaker>") != string::npos)
		{
			int pos = buffer.find("<Speaker>") + 9;
			int pos2 = buffer.find("</Speaker>") - pos;
			temp.SpeakName.append(buffer, pos, pos2);
		}

		//배경 찾기
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

			path = "../../_Textures/CG/";
			path.append(temp.bkKey);
			path += ".ab";

			LOADMANAGER->Add_LoadTray(temp.bkKey, path, LOADRESOURCE_TYPE::RESOURCE_IMAGE);

			//if (DRAW->getBitmap(temp.bkKey) == nullptr)
			//	DRAW->LoadImage_d2d(temp.bkKey, path);
		}

		if (buffer.find("<BGM>") != string::npos)
		{
			temp.curSound.clear();

			string path;
			int pos = buffer.find("<BGM>") + 5;
			int pos2 = buffer.find("</BGM>") - pos;
			temp.curSound.append(buffer, pos, pos2);

			if (temp.curSound.compare("STOP") != 0)
			{
				path = "../SoundData/bgm/";
				path.append(temp.curSound);
				path += ".wav";

				//sound->Insert_Sound(temp.curSound, path.c_str());
			}
			//sound->Play_Sound(temp.bkKey, 0.15f);
		}
		//else
			//temp.curSound = "STOP";

		//화자 이미지 찾기
		if (buffer.find("(") != string::npos)
		{
			int pos = buffer.find("(");
			if (pos < 1)	//()옆에 뭐가없다는뜻
				temp.vImageKey.clear();
			else
			{
				string key, path;
				key.append(buffer, 0, buffer.find(")") + 1);
				temp.vImageKey.push_back(key);

				path = "../../_Textures/";
				path.append(key, 0, key.find("("));
				path += "/";
				path.append(key, 0, key.size());
				path += ".ab";

				LOADMANAGER->Add_LoadTray(key, path, LOADRESOURCE_TYPE::RESOURCE_IMAGE);

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

		vCon.push_back(temp);
	}
}

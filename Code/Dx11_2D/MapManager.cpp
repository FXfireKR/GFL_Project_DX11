#include "stdafx.h"
#include "MapManager.h"


MapManager::MapManager()
{
	pManager = nullptr;
	battleMap = nullptr;

	BTM_Loader* loader = new BTM_Loader;
	battleMap = loader->LoadBTM("../../_Assets/BattleMap/bg2.btm");

	//	require 
	//IMAGEMAP->InsertImage_FromeFile("bg2", "../../_Assets/bg2.png");

	this->init();

	SAFE_DELETE(loader);
}

MapManager::~MapManager()
{
	this->release();
}

HRESULT MapManager::init()
{

	if (pManager == nullptr)
	{
		pManager = new PanelManager;
		pManager->init();
	}

	return S_OK;
}

void MapManager::release()
{
	if (pManager != nullptr)
	{
		pManager->Release_Panel();
		SAFE_DELETE(pManager);
	}

	SAFE_DELETE(battleMap);
}

void MapManager::update()
{
	pManager->Update_Panel();
}

void MapManager::render()
{
	//	뒷 배경
	DRAW->render(mapImageFileName, D3DXVECTOR2(2880, 2880),		//	1440p x 1440p
		D3DXVECTOR3(WINSIZEX*0.5F, WINSIZEY*0.5F, 0), D3DXCOLOR(1, 1, 1, 0.25F));

	//	전면부 이미지
	DRAW->render(mapImageFileName, D3DXVECTOR2(1440, 1440),		//	720p x 720p
		D3DXVECTOR3(WINSIZEX*0.5F, WINSIZEY*0.5F, 0));

	pManager->Render_Panel();
}

void MapManager::CreatePanel_()
{
	pManager->Create_Panel(g_ptMouse.x, g_ptMouse.y);
}

void MapManager::Check_ActionPoint()
{
	MaxActionPoint = pManager->Check_ActionPoint_For();
}

void MapManager::Load_MapFile(string path)
{
	//	Clear All Panel
	pManager->Release_Panel();
	pManager->getAllLink().clear();
	Panel::panelId = 0;

	//	Read File
	FILE* file = fopen(path.c_str(), "rb");

	if (file != NULL)
	{
		string buffer;
		char buf[512];
		size_t panelSize;


		while (!feof(file))
		{
			fgets(buf, 512, file);
			buffer = buf;

			// Tokenize #
			if (buffer.find("#") != string::npos)
			{
				//	Find Back Image
				if (buffer.find("image") != string::npos)
				{
					buffer.erase(0, buffer.find_last_of(" ") + 1);
					buffer = eraseEscape(buffer);

					mapImageFileName = buffer;
				}

				//	Find BGM
				else if (buffer.find("bgm") != string::npos)
				{
					buffer.erase(0, buffer.find_last_of(" ") + 1);
					buffer = eraseEscape(buffer);

					if (buffer.size() > 1)
					{
						//	Insert BGM Code
					}
				}

				//	Find BGM
				else if (buffer.find("linklistsize") != string::npos)
				{
					buffer.erase(0, buffer.find_last_of(" ") + 1);
					size_t linkSize = readInteger(buffer);
					pManager->getAllLink().reserve(linkSize);

					for (size_t i = 0; i < linkSize; ++i)
					{
						fgets(buf, 512, file);
						buffer = buf;

						tagPanelLink temp;
						POINT rpt = readDoubleInteger(buffer);
						temp.id1 = rpt.x;
						temp.id2 = rpt.y;

						pManager->getAllLink().push_back(temp);
					}
				}

				//	Find Panel
				else if (buffer.find("panelsize") != string::npos)
				{
					buffer.erase(0, buffer.find_last_of(" ") + 1);
					buffer = eraseEscape(buffer);

					panelSize = atoi(buffer.c_str());

					for (panelSize; panelSize > 0; --panelSize)
					{
						int panelID = 1;
						Panel* newPanel = nullptr;

						while (true)
						{
							fgets(buf, 512, file);
							buffer = buf;

							//	read Position of Panel
							if (buffer.find("t ") != string::npos)
							{
								buffer.erase(0, buffer.find_first_of(" ") + 1);
								POINT pos = readDoubleInteger(buffer);

								pManager->Create_Panel(pos.x, pos.y);
								newPanel = pManager->findPanel(pManager->getTotalPanelNum());
								++panelID;
							}

							//	read PanelClassify of Panel
							else if (buffer.find("ps ") != string::npos)
							{
								buffer.erase(0, buffer.find_last_of(" ") + 1);
								newPanel->setPanelEnum(readPanelClass(buffer));
							}

							//	read PanelAliance of Panel
							else if (buffer.find("pa ") != string::npos)
							{
								buffer.erase(0, buffer.find_last_of(" ") + 1);
								newPanel->setPanelAlience(readAlianceClass(buffer));
							}

							//	read Linked Pnale ID List of Panel
							else if (buffer.find("ls ") != string::npos)
							{
								buffer.erase(0, buffer.find_last_of(" ") + 1);
								size_t ListSize = readUnsignedInteger(buffer);

								newPanel->reserveLinkedList(ListSize);

								//	get List ID
								fgets(buf, 512, file);
								buffer = buf;
								buffer.erase(0, buffer.find_first_of(" ") + 1);
								eraseEscape(buffer);

								for (size_t i = 0; i < ListSize; ++i)
								{
									string temp; temp.append(buffer, 0, buffer.find_first_of(" "));
									buffer.erase(0, buffer.find_first_of(" ") + 1);

									newPanel->setLinkedId(atoi(temp.c_str()));
								}
							}

							//	read Battle Atlas Key
							else if (buffer.find("ba ") != string::npos)
							{
								buffer.erase(0, buffer.find_last_of(" ") + 1);
								buffer = eraseEscape(buffer);
								newPanel->setBattleAtlasKey(buffer);
							}

							//	read Battle Win Style Plag
							else if (buffer.find("bs ") != string::npos)
							{
								buffer.erase(0, buffer.find_last_of(" ") + 1);

								BattlePlagData temp = newPanel->getPlagData();
								temp.battlePlag = static_cast<BYTE>(readInteger(buffer));
								newPanel->setPlagData(temp);
							}

							//	read Battle Protect Object or Area Data
							else if (buffer.find("bpsize ") != string::npos)
							{
								buffer.erase(0, buffer.find_last_of(" ") + 1);

								BattlePlagData temp = newPanel->getPlagData();
								temp.protectObjectNumber = static_cast<BYTE>(readInteger(buffer));
								temp.protectArea.reserve(temp.protectObjectNumber);

								for (int i = 0; i < temp.protectObjectNumber; ++i)
								{
									fgets(buf, 512, file);
									buffer = buf;

									tagArea newArea;
									newArea = readArea(buffer);
									temp.protectArea.push_back(newArea);
								}

								newPanel->setPlagData(temp);

								break;
							}
						}
					}
				}
			}
		}

		fclose(file);
	}

	string image = "../../_Assets/BattleMap/" + mapImageFileName + ".ab";
	LOAD->Add_LoadTray(mapImageFileName, image.c_str(), LOADRESOURCE_TYPE::RESOURCE_IMAGE);

	for (auto& iter : pManager->getAllLink())
		pManager->Panel_Link(iter.id1, iter.id2);
}

string MapManager::eraseEscape(string _str)
{
	if (_str.find("\n") != string::npos)
		_str.erase(_str.find("\n"), _str.size());

	if (_str.find("\r") != string::npos)
		_str.erase(_str.find("\r"), _str.size());

	return _str;
}

int MapManager::readInteger(string _str)
{
	_str = eraseEscape(_str);
	return atoi(_str.c_str());
}

size_t MapManager::readUnsignedInteger(string _str)
{
	_str = eraseEscape(_str);
	return static_cast<size_t>(atoi(_str.c_str()));
}

POINT MapManager::readDoubleInteger(string _str)
{
	_str = eraseEscape(_str);
	string buffer; buffer.append(_str, 0, _str.find(" "));
	_str.erase(0, _str.find(" ") + 1);

	POINT value;
	value.x = atoi(buffer.c_str());
	value.y = atoi(_str.c_str());

	return value;
}

tagArea MapManager::readArea(string _str)
{
	_str = eraseEscape(_str);

	tagArea newArea;
	string buffer;

	for (int i = 0; i < 3; ++i)
	{
		buffer.append(_str, 0, _str.find_first_of(" "));
		_str.erase(0, _str.find_first_of(" ") + 1);

		switch (i)
		{
		case 0:
			newArea.AreaCenter.x = static_cast<FLOAT>(atof(buffer.c_str()));
			break;

		case 1:
			newArea.AreaCenter.y = static_cast<FLOAT>(atof(buffer.c_str()));
			break;

		case 2:
			newArea.AreaRaius.x = static_cast<FLOAT>(atof(buffer.c_str()));
			break;
		}

		buffer.clear();
	}

	newArea.AreaRaius.y = static_cast<FLOAT>(atof(_str.c_str()));


	return newArea;
}

PANEL_CLASSIFY MapManager::readPanelClass(string _str)
{
	_str = eraseEscape(_str);
	return (PANEL_CLASSIFY)atoi(_str.c_str());
}

TATICDOLL_ALIANCE_TYPE MapManager::readAlianceClass(string _str)
{
	_str = eraseEscape(_str);
	return (TATICDOLL_ALIANCE_TYPE)atoi(_str.c_str());
}

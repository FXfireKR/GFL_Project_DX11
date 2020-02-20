#include "stdafx.h"
#include "EquipLoader.h"

EquipLoader::EquipLoader()
{
	readDataBase();
}

EquipLoader::~EquipLoader()
{
	release();
}

void EquipLoader::release()
{
	for (auto& it : mEquipDataBase)
		delete it.second;
}

void EquipLoader::readDataBase()
{
	ifstream file;
	file.open(EQUIP_DATA_PATH);

	//	파일이 성공적으로 접근되었다면
	if (file.is_open())
	{
		EquipBase temp;
		string buffer;
		string str;

		while (!file.eof())
		{
			buffer.clear();

			getline(file, buffer);

			if (buffer.size() < 2) continue;

			//TYPE
			if (buffer.find("<type>") != string::npos)
			{
				str.clear();
				int pos = buffer.find("<type>") + 6;
				str.append(buffer, pos, buffer.find(",") - pos);
				temp.p_getItemType() = (EQUIPTOTAL_TYPE)(atoi(str.c_str()));

				buffer.erase(0, buffer.find(",") + 1);
			}

			// /Type
			else if (buffer.find("</type>") != string::npos)
				buffer.erase(0, buffer.find(",") + 1);


			//KEY
			if (buffer.find("<name>") != string::npos || buffer.find("</name>") != string::npos)
			{
				if (buffer.find("</name>") != string::npos)
					str = temp.getKey();
				else
				{
					str.clear();
					str.append(buffer, buffer.find(">") + 1, buffer.size());
					temp.setKey(str.c_str());
				}

				string path = str;
				path.insert(0, EQUIP_IMAGE_PATH);
				path.append(".ab");

				LOADMANAGER->Add_LoadTray(str, path, LOADRESOURCE_TYPE::RESOURCE_IMAGE);
				buffer.clear();
			}


			//NAME
			if (buffer.find("1#") != string::npos)
			{
				str.clear();
				int pos = buffer.find(",") + 1;
				str.append(buffer, pos, buffer.size());
				temp.p_getString().name = str;
				buffer.clear();
			}

			//SPEC
			if (buffer.find("2#") != string::npos)
			{
				str.clear();
				int pos = buffer.find(",") + 1;
				str.append(buffer, pos, buffer.size());
				temp.p_getString().spec = str;
				buffer.clear();
			}

			//SPEC
			if (buffer.find("3#") != string::npos)
			{
				str.clear();
				int pos = buffer.find(",") + 1;
				str.append(buffer, pos, buffer.size());
				temp.p_getString().native = str;
				buffer.clear();
			}


			//EPL
			if (buffer.find("4#") != string::npos)
			{
				str.clear();
				int pos = buffer.find(",") + 1;
				str.append(buffer, pos, buffer.size());
				temp.p_getString().explain = str;
				buffer.clear();

				EquipBase* eb = new EquipBase;
				assert(SUCCEEDED(eb != nullptr));

				eb->init(temp.getKey(), temp.getString(), temp.getItemType());
				mEquipDataBase.insert(make_pair(eb->getString().name, eb));
			}
		}

		file.close();
	}
}
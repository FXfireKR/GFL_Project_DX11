#include "stdafx.h"
#include "BTM_Loader.h"

BTM_Loader::BTM_Loader()
{
}

BTM_Loader::~BTM_Loader()
{
}

btmPair BTM_Loader::Tokenize(const FILE * file)
{
	char buf[512];
	fgets(buf, 512, const_cast<FILE*>(file));

	string buffer = buf;
	size_t pos;

	btmPair pair;

	if ((pos = buffer.find(':')) != string::npos) {
		pair.key.append(buffer, 0, pos);
		buffer.erase(0, pos + 1);
	}

	pair.value = buffer;

	while (true)
	{
		if ((pos = pair.value.find('\n')) != string::npos)
			pair.value.erase(pos, 1);

		if ((pos = pair.value.find("\r")) != string::npos)
			pair.value.erase(pos, 1);

		if ((pos = pair.value.find('{')) != string::npos)
			pair.value.erase(pos, 1);

		else break;
	}

	return pair;
}

string BTM_Loader::readString(const FILE * file)
{
	return Tokenize(file).value;
}

POINT BTM_Loader::readSize(const FILE * file)
{
	string buf = Tokenize(file).value;
	size_t pos;
	POINT pt;
	pt.x = pt.y = 0;

	if (string::npos != (pos = buf.find(',')))
	{
		string x;	x.append(buf, 0, pos);
		buf.erase(0, pos + 1);

		pt.x = atoi(x.c_str());
		pt.y = atoi(buf.c_str());
	}

	return pt;
}

D3DXVECTOR3 BTM_Loader::readVec3(const FILE * file)
{
	string buf = Tokenize(file).value;
	size_t p;
	D3DXVECTOR3 vec;

	if ((p = buf.find(',')) != string::npos)
	{
		string str;	str.append(buf, 0, p);
		vec.x = static_cast<float>(atof(str.c_str()));
		buf.erase(0, p + 1);

		str.clear();
		str.append(buf, 0, p = buf.find(','));
		vec.y = static_cast<float>(atof(str.c_str()));
		buf.erase(0, p + 1);

		vec.z = static_cast<float>(atof(buf.c_str()));
	}

	return vec;
}

int BTM_Loader::readInteger(const FILE * file)
{
	return atoi(Tokenize(file).value.c_str());
}

bool BTM_Loader::readBoolean(const FILE * file)
{
	return isFind(Tokenize(file).value, "true");
}

bool BTM_Loader::isCompare(const string str, const char * comp)
{
	return (str.compare(comp)) ? false : true;
}

bool BTM_Loader::isFind(const string str, const char * comp)
{
	return (str.find(comp) != string::npos) ? true : false;
}

BattleMap * BTM_Loader::LoadBTM(const char * _path)
{
	FILE* file = fopen(_path, "rb");

	if (file == NULL)
		return nullptr;

	BattleMap* newBTM = new BattleMap;

	if (!feof(file))
	{
		// Load Path
		newBTM->SettingImageString(Tokenize(file).value);

		// Load Size
		newBTM->imgSize = readSize(file);

		POINT lim = readSize(file);
		newBTM->limitSize = static_cast<float>(lim.x * lim.y);

		// Load face & pre Reserve Memory
		newBTM->vDrawOrder.reserve(readInteger(file));

		for (size_t i = 0; i < newBTM->vDrawOrder.capacity(); ++i)
		{
			MapAtlas* newAtlas = new MapAtlas;

			//	Load Atlas
			newAtlas->name = Tokenize(file).value;
			newAtlas->isRotate = readBoolean(file);
			newAtlas->origPos = readSize(file);
			newAtlas->size = readSize(file);
			newAtlas->offset = readSize(file);
			newAtlas->drawIndex = readInteger(file);
			newAtlas->scale = readVec3(file);
			newAtlas->rotate = readVec3(file);
			newAtlas->position = readVec3(file);

			//	insert Data
			newBTM->mAtlas.insert(make_pair(newAtlas->name, newAtlas));
			newBTM->vDrawOrder.push_back(newAtlas);

			//	make UV, Vertex Buffer
			newBTM->AtlasSetting(newAtlas);

			//	},
			Tokenize(file);
		}
	}

	fclose(file);

	return newBTM;
}

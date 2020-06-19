#include "stdafx.h"
#include "uiAtlasLoader.h"

uiAtlasLoader::~uiAtlasLoader(){}

pair<string, string> uiAtlasLoader::Tokenize(const FILE * file)
{
	char buf[512];
	fgets(buf, 512, const_cast<FILE*>(file));

	string buffer = buf;
	size_t pos;

	pair<string, string> Token;

	if ((pos = buffer.find(':')) != string::npos) {
		Token.first.append(buffer, 0, pos);
		buffer.erase(0, pos + 1);
	}

	Token.second = buffer;

	while (true)
	{
		if ((pos = Token.second.find('\n')) != string::npos)
			Token.second.erase(pos, 1);

		if ((pos = Token.second.find("\r")) != string::npos)
			Token.second.erase(pos, 1);

		if ((pos = Token.second.find('{')) != string::npos)
			Token.second.erase(pos, 1);

		else break;
	}

	return Token;
}

string uiAtlasLoader::readString(const FILE * file) { return Tokenize(file).second; }

POINT uiAtlasLoader::read2Integer(const FILE * file)
{
	string buf = Tokenize(file).second;
	size_t pos;
	POINT pt;
	pt.x = pt.y = 0;

	if (string::npos != (pos = buf.find(','))){
		string x;	x.append(buf, 0, pos);
		buf.erase(0, pos + 1);

		pt.x = atoi(x.c_str());
		pt.y = atoi(buf.c_str());
	}

	return pt;
}

int uiAtlasLoader::readInteger(const FILE * file) { return atoi(Tokenize(file).second.c_str()); }

bool uiAtlasLoader::readBoolean(const FILE * file) { return isFind(Tokenize(file).second, "true"); }

bool uiAtlasLoader::isCompare(const string str, const char * comp) { return (str.compare(comp)) ? false : true; }

bool uiAtlasLoader::isFind(const string str, const char * comp) { return (str.find(comp) != string::npos) ? true : false; }

void uiAtlasLoader::LoadUiAtlasData(const char * _path)
{
	unordered_map<string, uiAtlas*>* _newDataBase = IMAGEMAP->getUiAtlas();

	if (_newDataBase == nullptr)
		_newDataBase = new unordered_map<string, uiAtlas*>;

	string normalKey, AlphaKey;
	POINT imgSize;

	FILE* file = fopen(_path, "rb");
	if (file != NULL)
	{
		//	Normal Image Key
		normalKey = readString(file);
		AlphaKey = readString(file);
		imgSize = read2Integer(file);

		string path = "../../_Assets/Texture2D/";
		path.append(normalKey);
		path.append(".ab");
		LOAD->Add_LoadTray(normalKey, path.c_str(), LOADRESOURCE_TYPE::RESOURCE_IMAGE);
		path.clear();

		path = "../../_Assets/Texture2D/";
		path.append(AlphaKey);
		path.append(".ab");
		LOAD->Add_LoadTray(AlphaKey, path.c_str(), LOADRESOURCE_TYPE::RESOURCE_IMAGE);

		D3DXVECTOR2 PerTexcord;
		PerTexcord.x = 1.0f / static_cast<float>(imgSize.x);
		PerTexcord.y = 1.0f / static_cast<float>(imgSize.y);

		while (!feof(file))
		{
			uiAtlas* _new = new uiAtlas;
			_new->textureKey = normalKey;
			_new->alphaTexKey = AlphaKey;
				
			_new->name = readString(file);
			POINT startPos = read2Integer(file);
			POINT endPos = read2Integer(file);

			_new->mixTexCoord.x = PerTexcord.x * static_cast<float>(startPos.x);
			_new->mixTexCoord.y = PerTexcord.y * static_cast<float>(startPos.y);

			_new->maxTexCoord.x = PerTexcord.x * static_cast<float>(endPos.x);
			_new->maxTexCoord.y = PerTexcord.y * static_cast<float>(endPos.y);

			_newDataBase->insert(make_pair(_new->name, _new));

			//	Dummy Cutting
			readString(file);
		}

		fclose(file);
	}
}

const unordered_map<string, uiAtlas*>& uiAtlasLoader::LoadUiAtlasData(const char * _path, unordered_map<string, uiAtlas*>* _dataBase)
{
	if (_dataBase == nullptr)
		_dataBase = new unordered_map<string, uiAtlas*>;

	string normalKey, AlphaKey;
	POINT imgSize;

	FILE* file = fopen(_path, "rb");
	if (file != NULL)
	{
		//	Normal Image Key
		normalKey = readString(file);
		AlphaKey = readString(file);
		imgSize = read2Integer(file);

		string path = "../../_Assets/Texture2D/";
		path.append(normalKey);
		path.append(".ab");
		LOAD->Add_LoadTray(normalKey, path.c_str(), LOADRESOURCE_TYPE::RESOURCE_IMAGE);
		path.clear();

		path = "../../_Assets/Texture2D/";
		path.append(AlphaKey);
		path.append(".ab");
		LOAD->Add_LoadTray(normalKey, path.c_str(), LOADRESOURCE_TYPE::RESOURCE_IMAGE);

		D3DXVECTOR2 PerTexcord;
		PerTexcord.x = 1.0f / static_cast<float>(imgSize.x);
		PerTexcord.y = 1.0f / static_cast<float>(imgSize.y);

		while (!feof(file))
		{
			uiAtlas* _new = new uiAtlas;
			_new->textureKey = normalKey;
			_new->alphaTexKey = AlphaKey;

			_new->name = readString(file);
			POINT startPos = read2Integer(file);
			POINT endPos = read2Integer(file);

			_new->mixTexCoord.x = PerTexcord.x * static_cast<float>(startPos.x);
			_new->mixTexCoord.y = PerTexcord.y * static_cast<float>(startPos.y);

			_new->maxTexCoord.x = PerTexcord.x * static_cast<float>(endPos.x);
			_new->maxTexCoord.y = PerTexcord.y * static_cast<float>(endPos.y);

			_dataBase->insert(make_pair(_new->name, _new));

			//	Dummy Cutting
			readString(file);
		}

		fclose(file);
	}

	return *_dataBase;
}
#pragma once

class uiAtlasLoader
{
public:
	uiAtlasLoader();
	~uiAtlasLoader();

private:
	uiAtlasLoader(const uiAtlasLoader& other) = delete;
	uiAtlasLoader operator= (const uiAtlasLoader& other) = delete;

	pair<string, string> Tokenize(__in const FILE* file);
	string readString(__in const FILE* file);
	POINT read2Integer(__in const FILE* file);
	int readInteger(__in const FILE* file);
	bool readBoolean(__in const FILE* file);
	bool isCompare(__in const string str, __in const char* comp);
	bool isFind(__in const string str, __in const char* comp);

public:
	const map<string, uiAtlas>& LoaduiAtlasData(__in const char* _path, __out map<string, uiAtlas>* _dataBase);
	void LoaduiAtlasData(__in const char* _path);
	//const map<string, uiAtlas>& LoaduiAtlasMemory(__in , __out map<string, uiAtlas>* _dataBase = nullptr);

};
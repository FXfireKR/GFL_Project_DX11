#ifndef UIATLAS_LOADER_H
#define UIATLAS_LOADER_H

class uiAtlasLoader
{
public:	
	~uiAtlasLoader();

private:
	uiAtlasLoader() = delete;
	uiAtlasLoader(const uiAtlasLoader& other) = delete;
	uiAtlasLoader operator= (const uiAtlasLoader& other) = delete;

	static pair<string, string> Tokenize(__in const FILE* file);
	static string readString(__in const FILE* file);
	static POINT read2Integer(__in const FILE* file);
	static int readInteger(__in const FILE* file);
	static bool readBoolean(__in const FILE* file);
	static bool isCompare(__in const string str, __in const char* comp);
	static bool isFind(__in const string str, __in const char* comp);

public:
	static void LoadUiAtlasData(__in const char* _path);
	static const unordered_map<string, uiAtlas*>& LoadUiAtlasData(__in const char* _path, 
		__out unordered_map<string, uiAtlas*>* _dataBase);
};
#endif
#pragma once
#include "Shader.h"
#include "singleton.h"

class ShaderManager : public singleton<ShaderManager>
{
private:
	map<string, Shader*>				mShader;
	map<string, Shader*>::iterator		miShader;

	string currentShader;

public:
	ShaderManager(); 
	~ShaderManager();

	void CreateShader(const string _key, const D3D11_INPUT_ELEMENT_DESC* desc, UINT count, 
		const wstring file,  string vs = "VS", string ps = "PS");

	void setShader(string _key);

public:
	inline string getCurrentShaderKey() const { return currentShader; }
};
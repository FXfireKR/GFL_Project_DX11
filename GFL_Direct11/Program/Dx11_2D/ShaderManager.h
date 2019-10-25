#pragma once
#include "Shader.h"
#include "singleton.h"

class ShaderManager : public singleton<ShaderManager>
{
private:
	map<string, Shader*> mShader;

public:
	ShaderManager();
	~ShaderManager();

	HRESULT AddShaderFile(D3D11_INPUT_ELEMENT_DESC* desc, UINT count, string key, wstring file);
	void release();

	HRESULT setShader(string key);
};
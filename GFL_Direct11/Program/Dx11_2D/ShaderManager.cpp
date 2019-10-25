#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

HRESULT ShaderManager::AddShaderFile(D3D11_INPUT_ELEMENT_DESC * desc, UINT count, string key, wstring file)
{
	if (!mShader.count(key))
	{
		Shader* temp = new Shader(desc, count, file);
		mShader.insert(make_pair(key, temp));
		return S_OK;
	}

	//Not Included Shader File
	MessageBoxA(g_hWnd, "[SHADERMANAGER] :: Already Insert Shader", NULL, NULL);
	return E_FAIL;
}

void ShaderManager::release()
{
	for (auto& it : mShader)
		SAFE_DEL(it.second);	
}

HRESULT ShaderManager::setShader(string key)
{
	if (mShader.count(key))
	{
		mShader.find(key)->second->SetShader();
		return S_OK;
	}

	//Not Included Shader File
	MessageBoxA(g_hWnd, "[SHADERMANAGER] :: Not Included File", NULL, NULL);

	PostQuitMessage(0);	//±‰±ﬁ≈ª√‚!

	return E_FAIL;
}

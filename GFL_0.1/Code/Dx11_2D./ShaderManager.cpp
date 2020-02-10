#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	for (auto& it : mShader)
		SAFE_DEL(it.second);
}

void ShaderManager::CreateShader(const string _key, const D3D11_INPUT_ELEMENT_DESC * desc, UINT count, const wstring file, string vs, string ps)
{
	//	if there was not Shader Key
	if (!mShader.count(_key))
	{
		Shader* newshader = new Shader(desc, count, file, vs, ps);
		mShader.insert(make_pair(_key, newshader));
	}
}

void ShaderManager::setShader(string _key)
{
	if ((miShader = mShader.find(_key)) != mShader.end())
		miShader->second->SetShader();
}

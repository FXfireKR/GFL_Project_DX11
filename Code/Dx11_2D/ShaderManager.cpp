#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	//	define shader file
	CreateShader("PTBase_noWorldColor", PTElementDesc, PTElementCount, L"../../_Shader/TextureBase2.hlsl");
	CreateShader("PTBase", PTElementDesc, PTElementCount, L"../../_Shader/TextureBase.hlsl");
	CreateShader("PT_Noise", PTElementDesc, PTElementCount, L"../../_Shader/NoiseTexture.hlsl");
	CreateShader("PT_Alpha", PTElementDesc, PTElementCount, L"../../_Shader/AlphaChannel.hlsl");
	CreateShader("PT_Alpha2", PTElementDesc, PTElementCount, L"../../_Shader/AlphaChannel2.hlsl");
	CreateShader("PT", PTElementDesc, PTElementCount, L"../../_Shader/Texture.hlsl");
}

ShaderManager::~ShaderManager()
{
	for (auto& it : mShader)
		SAFE_DELETE(it.second);
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

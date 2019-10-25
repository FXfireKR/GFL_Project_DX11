#include "stdafx.h"
#include "Shader.h"

Shader::Shader(D3D11_INPUT_ELEMENT_DESC* desc, UINT count,
	wstring file, string vs, string ps)
{
	// Create VertexShader, PixelShader
	{
		HRESULT hr;

		hr = D3DX11CompileFromFile(file.c_str(), 0, 0, vs.c_str(), "vs_5_0", 0, 0, 0, &m_pVsBlob, 0, 0);
		assert(SUCCEEDED(hr));
		hr = Device->CreateVertexShader(m_pVsBlob->GetBufferPointer(), m_pVsBlob->GetBufferSize(), NULL, &m_pVertexShader);
		assert(SUCCEEDED(hr));

		hr = D3DX11CompileFromFile(file.c_str(), 0, 0, ps.c_str(), "ps_5_0", 0, 0, 0, &m_pPsBlob, 0, 0);
		assert(SUCCEEDED(hr));
		hr = Device->CreatePixelShader(m_pPsBlob->GetBufferPointer(), m_pPsBlob->GetBufferSize(), NULL, &m_pPixelShader);
		assert(SUCCEEDED(hr));
	}

	// CreateInputLayout
	{
		HRESULT hr = Device->CreateInputLayout(desc, count,
			m_pVsBlob->GetBufferPointer(), m_pVsBlob->GetBufferSize(), &m_pVertexLayout);
		assert(SUCCEEDED(hr));
	}
}


Shader::~Shader()
{
	SAFE_REL(m_pVertexLayout);
	SAFE_REL(m_pVertexShader);
	SAFE_REL(m_pPixelShader);
	SAFE_REL(m_pVsBlob);
	SAFE_REL(m_pPsBlob);
}

void Shader::SetShader()
{
	DeviceContext->VSSetShader(m_pVertexShader, 0, 0);
	DeviceContext->PSSetShader(m_pPixelShader, 0, 0);

	DeviceContext->IASetInputLayout(m_pVertexLayout);
}
#pragma once
// ������(Compile) : ��� ���(�ΰ��� ������ �� �ִ� ��� HLSL : High Level Shading Language) -> ����(��ǻ�� ���)
class Shader
{
private:
	ID3D11VertexShader*	m_pVertexShader;
	ID3D11PixelShader*	m_pPixelShader;
	ID3D10Blob*			m_pVsBlob;
	ID3D10Blob*			m_pPsBlob;
	ID3D11InputLayout*	m_pVertexLayout;

public:
	Shader(const D3D11_INPUT_ELEMENT_DESC* desc, UINT count, const wstring file, string vs = "VS", string ps = "PS");
	~Shader();

	void SetShader();
};

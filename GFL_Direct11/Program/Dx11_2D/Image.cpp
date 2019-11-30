#include "stdafx.h"
#include "Image.h"

Image::Image() : worldBuffer(nullptr), vertexBuffer(nullptr), vertexLayout(nullptr)
{
	// 포지션과 정점을 가지는 버텍스 포맷 지정
	D3D11_INPUT_ELEMENT_DESC PCLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	static UINT PCElementCount = ARRAYSIZE(PCLayoutDesc);

	shaderManager = new Shader(PCLayoutDesc, PCElementCount,
		L"../../_Shader/Sprite.hlsl");

	// Create WorldBuffer
	{
		D3D11_BUFFER_DESC desc = { 0, };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(D3DXMATRIX);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = Device->CreateBuffer(&desc, NULL, &worldBuffer);
		assert(SUCCEEDED(hr));
	}
	DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);

	arrVertex[0] = PTVertex(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXVECTOR2(0, 1));
	arrVertex[1] = PTVertex(D3DXVECTOR3(-0.5f, 0.5f, 0.0f), D3DXVECTOR2(0, 0));
	arrVertex[2] = PTVertex(D3DXVECTOR3(0.5f, 0.5f, 0.0f), D3DXVECTOR2(1, 0));
	arrVertex[3] = PTVertex(D3DXVECTOR3(0.5f, -0.5f, 0.0f), D3DXVECTOR2(1, 1));
	arrVertex[4] = PTVertex(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXVECTOR2(0, 1));

	PTVertex vertices[6] =
	{
		arrVertex[0],
		arrVertex[1],
		arrVertex[2],
		arrVertex[0],
		arrVertex[2],
		arrVertex[3],
	};

	// CreateVertexBuffer : m_pVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0, };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(PTVertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0, };
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	Scale = D3DXVECTOR2(1, 1);
	Rotate = D3DXVECTOR3(0, 0, 0);
	Trans = D3DXVECTOR2(0, 0);
}

Image::Image(string srvKey) : worldBuffer(nullptr), vertexBuffer(nullptr), vertexLayout(nullptr)
{
	// Create WorldBuffer
	{
		D3D11_BUFFER_DESC desc = { 0, };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(D3DXMATRIX);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = Device->CreateBuffer(&desc, NULL, &worldBuffer);
		assert(SUCCEEDED(hr));
	}

	VertexVec.push_back(PTVertex(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXVECTOR2(0, 1)));
	VertexVec.push_back(PTVertex(D3DXVECTOR3(-0.5f, 0.5f, 0.0f), D3DXVECTOR2(0, 0)));
	VertexVec.push_back(PTVertex(D3DXVECTOR3(0.5f, 0.5f, 0.0f), D3DXVECTOR2(1, 0)));

	VertexVec.push_back(PTVertex(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXVECTOR2(0, 1)));
	VertexVec.push_back(PTVertex(D3DXVECTOR3(0.5f, 0.5f, 0.0f), D3DXVECTOR2(1, 0)));
	VertexVec.push_back(PTVertex(D3DXVECTOR3(0.5f, -0.5f, 0.0f), D3DXVECTOR2(1, 1)));


	// CreateVertexBuffer : m_pVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0, };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(PTVertex) * VertexVec.size();
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0, };
		data.pSysMem = &VertexVec[0];

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	color = D3DXCOLOR(1, 1, 1, 1);
	{
		D3D11_BUFFER_DESC desc = { 0, };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(D3DXCOLOR);
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0, };
		data.pSysMem = color;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &colorBuffer);
		assert(SUCCEEDED(hr));
	}

	if (srvKey.compare("NULL"))
	{
		srv = IMAGEMANAGER->getTexture(srvKey);
		imgSize.x = IMAGEMANAGER->getImageInfo(srvKey)->Width;
		imgSize.y = IMAGEMANAGER->getImageInfo(srvKey)->Height;
	}

	Scale = D3DXVECTOR2(1.0f, 1.0f);
	Rotate = D3DXVECTOR3(0, 0, 0);
	Trans = D3DXVECTOR2(0, 0);

	Trans = D3DXVECTOR2(WINSIZEX*0.5f, WINSIZEY*0.5f);

	this->srvKey = srvKey;
}

Image::~Image()
{
}

void Image::init()
{
}

void Image::release()
{
	SAFE_REL(worldBuffer);
	SAFE_REL(vertexBuffer);
	SAFE_REL(vertexLayout);
}

void Image::update()
{
	//ImGui::DragFloat("R", &color.r, 0.01f, 0.0F, 1.0F);
	//ImGui::DragFloat("G", &color.g, 0.01f, 0.0F, 1.0F);
	//ImGui::DragFloat("B", &color.b, 0.01f, 0.0F, 1.0F);
	//ImGui::DragFloat("A", &color.a, 0.01f, 0.0F, 1.0F);

	D3DXMATRIX s, rx, ry, rz, t;
	D3DXMatrixScaling(&s, Scale.x * imgSize.x, Scale.y * imgSize.y, 1);

	D3DXMatrixRotationX(&rx, RAD(Rotate.x));
	D3DXMatrixRotationY(&ry, RAD(Rotate.y));
	D3DXMatrixRotationZ(&rz, RAD(Rotate.z));

	D3DXMatrixTranslation(&t, Trans.x - VPOS->x, WINSIZEY - Trans.y + VPOS->y, 0);

	worldMatrix = s * rx * ry * rz * t;
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);
	DeviceContext->UpdateSubresource(colorBuffer, 0, NULL, color, 0, 0);
}

void Image::render()
{
	//shaderManager->SetShader();
	SHADER->setShader("SPRITE");

	UINT stride = sizeof(PTVertex);
	UINT offset = 0;

	DeviceContext->VSSetConstantBuffers(1, 1, &worldBuffer);
	DeviceContext->PSSetConstantBuffers(0, 1, &colorBuffer);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//srv = IMAGEMANAGER->getTexture(srvKey);

	DeviceContext->PSSetShaderResources(0, 1, &srv);
	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->Draw(6, 0);
}

void Image::render(D3DXVECTOR2 s, D3DXVECTOR3 r, D3DXVECTOR2 t, D3DXCOLOR c)
{
	D3DXMATRIX m_s, rx, ry, rz, m_t;
	D3DXMatrixScaling(&m_s, s.x,  s.y, 1);

	D3DXMatrixRotationX(&rx, RAD(Rotate.x));
	D3DXMatrixRotationY(&ry, RAD(Rotate.y));
	D3DXMatrixRotationZ(&rz, RAD(Rotate.z));

	D3DXMatrixTranslation(&m_t, t.x - VPOS->x, WINSIZEY - t.y + VPOS->y, 0);

	worldMatrix = m_s * rx * ry * rz * m_t;
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);

	color = c;
	DeviceContext->UpdateSubresource(colorBuffer, 0, NULL, color, 0, 0);

	SHADER->setShader("SPRITE");

	UINT stride = sizeof(PTVertex);
	UINT offset = 0;

	DeviceContext->VSSetConstantBuffers(1, 1, &worldBuffer);
	DeviceContext->PSSetConstantBuffers(0, 1, &colorBuffer);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DeviceContext->PSSetShaderResources(0, 1, &srv);
	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->Draw(6, 0);
}

void Image::setTextureKey(string key)
{
	srvKey = key;  
	srv = IMAGEMANAGER->getTexture(srvKey);
}
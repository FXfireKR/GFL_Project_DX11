#include "stdafx.h"
#include "DXRect.h"

DXRect::DXRect() : worldMatrix(nullptr), worldBuffer(nullptr),
vertexBuffer(nullptr), vertexLayout(nullptr)
{
	// 포지션과 정점을 가지는 버텍스 포맷 지정
	D3D11_INPUT_ELEMENT_DESC PCLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	static UINT PCElementCount = ARRAYSIZE(PCLayoutDesc);

	shaderManager = new Shader(PCLayoutDesc, PCElementCount,
		L"../../_Shader/Rect.hlsl");

	worldMatrix = new D3DXMATRIX;

	this->init();
	
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

	PCVertex vertices[6] =
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
		desc.ByteWidth = sizeof(PCVertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0, };
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}
}

DXRect::~DXRect()
{
}

HRESULT DXRect::init()
{
	Color = D3DXCOLOR(1, 1, 1, 1);
	arrVertex[0] = PCVertex(D3DXVECTOR3(-0.5f,-0.5f, 0.0f), Color);
	arrVertex[1] = PCVertex(D3DXVECTOR3(-0.5f, 0.5f, 0.0f), Color);
	arrVertex[2] = PCVertex(D3DXVECTOR3(0.5f,  0.5f, 0.0f), Color);
	arrVertex[3] = PCVertex(D3DXVECTOR3(0.5f, -0.5f, 0.0f), Color);
	arrVertex[4] = PCVertex(D3DXVECTOR3(-0.5f,-0.5f, 0.0f), Color);

	Scale[0] = Scale[1] = 50;
	Rotate[0] = Rotate[1] = Rotate[2] = 0.0f;
	Transpos[0] = Transpos[1] = 100;

	Angle = rand() % 360;

	// World 매트릭스 업데이트
	D3DXMATRIX S, RX, RY, RZ, T;

	D3DXMatrixScaling(&S, Scale[0], Scale[1], 1);

	D3DXMatrixRotationX(&RX, Rotate[0]);
	D3DXMatrixRotationY(&RY, Rotate[1]);
	D3DXMatrixRotationZ(&RZ, Rotate[2]);

	D3DXMatrixTranslation(&T, Transpos[0], Transpos[1], 0);

	*worldMatrix = S * RX * RY * RZ * T;
	D3DXMatrixTranspose(worldMatrix, worldMatrix);

	if (vertexBuffer)
	{
		PCVertex vertices[6] =
		{
			arrVertex[0],
			arrVertex[1],
			arrVertex[2],
			arrVertex[0],
			arrVertex[2],
			arrVertex[3],
		};

		DeviceContext->UpdateSubresource(vertexBuffer, 0, 0, vertices, sizeof(PCVertex) * 6, 0);
	}

	return S_OK;
}

void DXRect::release()
{
	SAFE_DEL(worldMatrix);
	SAFE_DEL(shaderManager);
}

void DXRect::update()
{
	// World 매트릭스 업데이트
	D3DXMATRIX S, RX, RY, RZ, T;

	D3DXMatrixScaling(&S, Scale[0], Scale[1], 1);

	D3DXMatrixRotationX(&RX, Rotate[0]);
	D3DXMatrixRotationY(&RY, Rotate[1]);
	D3DXMatrixRotationZ(&RZ, Rotate[2]);

	D3DXMatrixTranslation(&T, Transpos[0], Transpos[1], 0);

	*worldMatrix = S * RX * RY * RZ * T;
	D3DXMatrixTranspose(worldMatrix, worldMatrix);

	DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);

	for (int i = 0; i < 5; i++)
		arrVertex[i].Color = Color;

	if (vertexBuffer)
	{
		PCVertex vertices[6] =
		{
			arrVertex[0],
			arrVertex[1],
			arrVertex[2],
			arrVertex[0],
			arrVertex[2],
			arrVertex[3],
		};

		DeviceContext->UpdateSubresource(vertexBuffer, 0, 0, vertices, sizeof(PCVertex) * 6, 0);
	}
}

void DXRect::render()
{
	SHADER->setShader("RECT");

	DeviceContext->VSSetConstantBuffers(1, 1, &worldBuffer);

	UINT stride = sizeof(PCVertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetInputLayout(vertexLayout);

	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext->Draw(6, 0);
}

bool DXRect::AABB_Collition(const DXRect & rc)
{
	if ((Transpos[0] - Scale[0] * 0.5f) > (rc.getTransX() + rc.getScaleX()*0.5f) || (Transpos[0] + Scale[0] * 0.5f) < (rc.getTransX() - rc.getScaleX()*0.5f))return false;
	if ((Transpos[1] + Scale[1] * 0.5f) < (rc.getTransY() - rc.getScaleY()*0.5f) || (Transpos[1] - Scale[1] * 0.5f) > (rc.getTransY() + rc.getScaleY()*0.5f))return false;

	return true;
}
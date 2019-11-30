#include "stdafx.h"
#include "DXCircle.h"

DXCircle::DXCircle() : worldBuffer(nullptr), vertexBuffer(nullptr)
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

	Scale = D3DXVECTOR2(450, 200);
	Rotate = D3DXVECTOR3(0, 0, 0);
	Trans = D3DXVECTOR2(450, 300);
}

DXCircle::~DXCircle()
{
}

void DXCircle::init()
{
}

void DXCircle::release()
{
	SAFE_REL(worldBuffer);
	SAFE_REL(vertexBuffer);
}

void DXCircle::update()
{
	
}

void DXCircle::render()
{
	D3DXMATRIX s, rx, ry, rz, t;
	D3DXMatrixScaling(&s, Scale.x, Scale.y, 1);

	D3DXMatrixRotationX(&rx, RAD(Rotate.x));
	D3DXMatrixRotationY(&ry, RAD(Rotate.y));
	D3DXMatrixRotationZ(&rz, RAD(Rotate.z));

	D3DXMatrixTranslation(&t, Trans.x - VPOS->x, WINSIZEY - Trans.y + VPOS->y, 0);

	worldMatrix = s * rx * ry * rz * t;
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);
	

	SHADER->setShader("CIRCLE");

	UINT stride = sizeof(PTVertex);
	UINT offset = 0;

	DeviceContext->VSSetConstantBuffers(1, 1, &worldBuffer);
	DeviceContext->PSSetConstantBuffers(0, 1, &colorBuffer);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->Draw(6, 0);
}

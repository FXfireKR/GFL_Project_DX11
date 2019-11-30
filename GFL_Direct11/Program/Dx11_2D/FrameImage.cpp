#include "stdafx.h"
#include "FrameImage.h"

FrameImage::FrameImage(string key)
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

	//VertexVec.push_back(PTVertex(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXVECTOR2(0, 1)));
	//VertexVec.push_back(PTVertex(D3DXVECTOR3(-0.5f, 0.5f, 0.0f), D3DXVECTOR2(0, 0)));
	//VertexVec.push_back(PTVertex(D3DXVECTOR3(0.5f, 0.5f, 0.0f), D3DXVECTOR2(0.2, 0)));

	//VertexVec.push_back(PTVertex(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXVECTOR2(0, 1)));
	//VertexVec.push_back(PTVertex(D3DXVECTOR3(0.5f, 0.5f, 0.0f), D3DXVECTOR2(0.2, 0)));
	//VertexVec.push_back(PTVertex(D3DXVECTOR3(0.5f, -0.5f, 0.0f), D3DXVECTOR2(0.2, 1)));


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

	Scale = D3DXVECTOR2(1.0f, 1.0f);
	Rotate = D3DXVECTOR3(0, 0, 0);
	Trans = D3DXVECTOR2(0, 0);

	Trans = D3DXVECTOR2(WINSIZEX * 0.5f, WINSIZEY * 0.5f);

	this->srvKey = key;
	srv = IMAGEMANAGER->getTexture(srvKey);
	if (srvKey.compare("NULL"))
	{
		imgSize.x = IMAGEMANAGER->getImageInfo(srvKey)->Width;
		imgSize.y = IMAGEMANAGER->getImageInfo(srvKey)->Height;
	}
}

FrameImage::~FrameImage()
{
}

void FrameImage::init()
{
}

void FrameImage::release()
{
	SAFE_REL(worldBuffer);
	SAFE_REL(vertexBuffer);
	//SAFE_REL(vertexLayout);
}

void FrameImage::render(FLOAT sx, FLOAT ex, FLOAT sy, FLOAT ey)
{
	//Sprite UV Change
	VertexVec[0].Texcoord.x = VertexVec[1].Texcoord.x = VertexVec[3].Texcoord.x = sx;
	VertexVec[2].Texcoord.x = VertexVec[4].Texcoord.x = VertexVec[5].Texcoord.x = ex;
	VertexVec[1].Texcoord.y = VertexVec[2].Texcoord.y = VertexVec[4].Texcoord.y = sy;
	VertexVec[0].Texcoord.y = VertexVec[3].Texcoord.y = VertexVec[5].Texcoord.y = ey;
	DeviceContext->UpdateSubresource(vertexBuffer, 0, NULL, &VertexVec[0], 0, 0);

	//Sprite Update
	{
		D3DXMATRIX sc, rx, ry, rz, tr;
		D3DXMatrixScaling(&sc, Scale.x * 200, Scale.y * 200, 1);

		D3DXMatrixRotationX(&rx, RAD(Rotate.x));
		D3DXMatrixRotationY(&ry, RAD(Rotate.y));
		D3DXMatrixRotationZ(&rz, RAD(Rotate.z));

		D3DXMatrixTranslation(&tr, Trans.x, WINSIZEY - Trans.y, 0);

		worldMatrix = sc * rx * ry * rz * tr;
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);
		DeviceContext->UpdateSubresource(colorBuffer, 0, NULL, color, 0, 0);
	}

	SHADER->setShader("SPRITE");

	UINT stride = sizeof(PTVertex);
	UINT offset = 0;

	DeviceContext->VSSetConstantBuffers(1, 1, &worldBuffer);
	DeviceContext->PSSetConstantBuffers(0, 1, &colorBuffer);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	srv = IMAGEMANAGER->getTexture(srvKey);
	DeviceContext->PSSetShaderResources(0, 1, &srv);
	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->Draw(6, 0);
}

void FrameImage::render(D3DXVECTOR2 s, D3DXVECTOR3 r, D3DXVECTOR2 t, FLOAT sx, FLOAT ex, FLOAT sy, FLOAT ey)
{

	VertexVec[0].Texcoord.x = VertexVec[1].Texcoord.x = VertexVec[3].Texcoord.x = sx;
	VertexVec[2].Texcoord.x = VertexVec[4].Texcoord.x = VertexVec[5].Texcoord.x = ex;
	VertexVec[1].Texcoord.y = VertexVec[2].Texcoord.y = VertexVec[4].Texcoord.y = sy;
	VertexVec[0].Texcoord.y = VertexVec[3].Texcoord.y = VertexVec[5].Texcoord.y = ey;
	DeviceContext->UpdateSubresource(vertexBuffer, 0, NULL, &VertexVec[0], 0, 0);

	//Sprite Update
	{
		D3DXMATRIX sc, rx, ry, rz, tr;
		D3DXMatrixScaling(&sc, s.x, s.y, 1);

		D3DXMatrixRotationX(&rx, RAD(r.x));
		D3DXMatrixRotationY(&ry, RAD(r.y));
		D3DXMatrixRotationZ(&rz, RAD(r.z));

		D3DXMatrixTranslation(&tr, t.x - VPOS->x, WINSIZEY - t.y + VPOS->y, 0);

		worldMatrix = sc * rx * ry * rz * tr;
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);
		DeviceContext->UpdateSubresource(colorBuffer, 0, NULL, color, 0, 0);
	}

	SHADER->setShader("SPRITE");

	UINT stride = sizeof(PTVertex);
	UINT offset = 0;

	DeviceContext->VSSetConstantBuffers(1, 1, &worldBuffer);
	DeviceContext->PSSetConstantBuffers(0, 1, &colorBuffer);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	srv = IMAGEMANAGER->getTexture(srvKey);
	DeviceContext->PSSetShaderResources(0, 1, &srv);
	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->Draw(6, 0);
}

void FrameImage::setTextureKey(string key)
{
	srvKey = key;
	srv = IMAGEMANAGER->getTexture(srvKey);
}

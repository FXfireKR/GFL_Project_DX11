#include "stdafx.h"
#include "FrameImage.h"

FrameImage::FrameImage()
{
	vertex.reserve(6);

	//	Create Vertex Buffer
	vertex.push_back(PTVertex(DV3(-1.0f, -1.0f, 0.0f), DV2(0, 1)));		//	0
	vertex.push_back(PTVertex(DV3(-1.0, 1.0, 0.0f), DV2(0, 0)));		//	1
	vertex.push_back(PTVertex(DV3(1.0, 1.0, 0.0f), DV2(1, 0)));			//	2

	vertex.push_back(PTVertex(DV3(-1.0f, -1.0f, 0.0f), DV2(0, 1)));		//	0
	vertex.push_back(PTVertex(DV3(1.0, 1.0, 0.0f), DV2(1, 0)));			//	2
	vertex.push_back(PTVertex(DV3(1.0, -1.0, 0.0f), DV2(1, 1)));		//	3

	CreateVertexBuffer(&vertexBuffer, sizeof(PTVertex) * vertex.size(), &vertex[0], D3D11_USAGE_DEFAULT);

	D3DXMATRIX worldmatrix;
	D3DXCOLOR color;
	CreateConstantBuffer(&worldBuffer, sizeof(D3DXMATRIX), worldmatrix);
	CreateConstantBuffer(&colorBuffer, sizeof(D3DXCOLOR), color);
}

FrameImage::~FrameImage()
{
	SAFE_RELEASE(worldBuffer);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(colorBuffer);
}

void FrameImage::render(string srvKey, DV2 _scale, DV2 _trans, DV2 _minTexCoord, DV2 _maxTexCoord, DV3 _rotate, DCR _color)
{
	//	Vertex Buffer Update
	{
		D3DXMATRIX worldMatrix, s, rx, ry, rz, t;

		D3DXMatrixIdentity(&worldMatrix);

		D3DXMatrixScaling(&s, _scale.x, _scale.y, 1);

		D3DXMatrixRotationX(&rx, RAD(_rotate.x));
		D3DXMatrixRotationY(&ry, RAD(_rotate.y));
		D3DXMatrixRotationZ(&rz, RAD(_rotate.z));

		D3DXMatrixTranslation(&t, _trans.x, _trans.y, 0);

		worldMatrix = s * rx * ry * rz * t;
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);

		DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);
		DeviceContext->UpdateSubresource(colorBuffer, 0, NULL, _color, 0, 0);

		vertex[0].TexCoord.x = vertex[1].TexCoord.x = vertex[3].TexCoord.x = _minTexCoord.x > 1.0 ? 1.0f : _minTexCoord.x;
		vertex[2].TexCoord.x = vertex[4].TexCoord.x = vertex[5].TexCoord.x = _maxTexCoord.x > 1.0 ? 1.0f : _maxTexCoord.x;

		vertex[0].TexCoord.y = vertex[3].TexCoord.y = vertex[5].TexCoord.y = _minTexCoord.y;
		vertex[1].TexCoord.y = vertex[2].TexCoord.y = vertex[4].TexCoord.y = _maxTexCoord.y;

		DeviceContext->UpdateSubresource(vertexBuffer, 0, NULL, &vertex[0], 0, 0);
	}

	//	Render
	{
		SHADER->setShader("PTBase");

		UINT stride = sizeof(PTVertex);
		UINT offset = 0;

		DeviceContext->VSSetConstantBuffers(1, 1, &worldBuffer);
		DeviceContext->PSSetConstantBuffers(0, 1, &colorBuffer);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11ShaderResourceView* srv = IMAGEMAP->getTexture(srvKey);

		DeviceContext->PSSetShaderResources(0, 1, &srv);

		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->Draw(6, 0);
	}
}

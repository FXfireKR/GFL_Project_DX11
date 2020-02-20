#include "stdafx.h"
#include "Image.h"

Image::Image() : worldBuffer(nullptr), vertexBuffer(nullptr), NoiseBuffer(nullptr)
{
	SHADER->CreateShader("PTBase", PTElementDesc, PTElementCount, PT_BaseShaderFile);
	SHADER->CreateShader("PT_Noise", PTElementDesc, PTElementCount, PT_NoiseShaderFile);
	SHADER->CreateShader("PT_Alpha", PTElementDesc, PTElementCount, PT_AlphaShaderFile);
	SHADER->CreateShader("PT_Alpha2", PTElementDesc, PTElementCount, PT_Alpha2ShaderFile);
	//
	//

	//	Create WorldMatrix Buffer
	D3DXMatrixIdentity(&worldMatrix);
	CreateConstantBuffer(&worldBuffer, sizeof(D3DXMATRIX), &worldMatrix);

	//	Create Color Buffer
	color = D3DXCOLOR(1, 1, 1, 1);
	CreateConstantBuffer(&colorBuffer, sizeof(D3DXCOLOR), &color);

	NoiseStruct.startY = 0.0f;
	NoiseStruct.limitAlpha = 0.0f;
	CreateConstantBuffer(&NoiseBuffer, sizeof(tagNoiseBuffer), &NoiseStruct);

	//	Create Vertex Buffer
	arrVertex[0] = PTVertex(DV3(-1.0f, -1.0f, 0.0f),	DV2(0, 1));
	arrVertex[1] = PTVertex(DV3(-1.0, 1.0, 0.0f),		DV2(0, 0));
	arrVertex[2] = PTVertex(DV3(1.0, 1.0, 0.0f),		DV2(1, 0));
	arrVertex[3] = PTVertex(DV3(1.0, -1.0, 0.0f),		DV2(1, 1));

	PTVertex vertices[6] =
	{
		arrVertex[0],
		arrVertex[1],
		arrVertex[2],
		arrVertex[0],
		arrVertex[2],
		arrVertex[3],
	};

	CreateVertexBuffer(&vertexBuffer, sizeof(PTVertex) * 6, vertices, D3D11_USAGE_DEFAULT);

	Scale = D3DXVECTOR3(1, 1, 1);
	Trans = D3DXVECTOR2(0, 0);
	Rotate = D3DXVECTOR3(0, 0, 0);
}

Image::~Image()
{
	this->release();
}

void Image::init()
{
}

void Image::release()
{
	SAFE_REL(worldBuffer);
	SAFE_REL(vertexBuffer);
	SAFE_REL(NoiseBuffer);
}

void Image::update()
{
}

void Image::render(const char * srvKey, DV2 _scale, DV2 _trans, DCR _color, DV3 _rotate)
{
	{
		D3DXMATRIX s, rx, ry, rz, t;

		D3DXMatrixIdentity(&worldMatrix);

		//	Use Camera ScaleUp
		FLOAT percent = static_cast<FLOAT>((100 - whlCount) * 0.01F);
		_scale.x *= percent;
		_scale.y *= percent;

		_trans.x = _trans.x;
		_trans.y = (WINSIZEY - _trans.y);

		//	드래그를 이용한 중앙_ 확대 / 축소 시에 좌표 고정
		FLOAT Dif_Center_X = 640.0f - _trans.x;
		FLOAT Dif_Center_Y = 360.0f - _trans.y;

		//FLOAT Dif_Center_X = g_ptMouse.x - _trans.x;
		//FLOAT Dif_Center_Y = g_ptMouse.y - _trans.y;

		if (fabsf(Dif_Center_X) < FLOAT_EPSILON)
			Dif_Center_X = (int)0;

		if (fabsf(Dif_Center_Y) < FLOAT_EPSILON)
			Dif_Center_Y = (int)0;

		_trans.x += ((1 - percent) * Dif_Center_X);
		_trans.y += ((1 - percent) * Dif_Center_Y);

		D3DXMatrixScaling(&s, _scale.x, _scale.y, 1);

		D3DXMatrixRotationX(&rx, RAD(_rotate.x));
		D3DXMatrixRotationY(&ry, RAD(_rotate.y));
		D3DXMatrixRotationZ(&rz, RAD(_rotate.z));

		D3DXMatrixTranslation(&t, _trans.x, _trans.y, 0);

		worldMatrix = s * rx * ry * rz * t;
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);

		color = _color;
		DeviceContext->UpdateSubresource(colorBuffer, 0, NULL, color, 0, 0);

		PTVertex vertices[6] =
		{
			arrVertex[0],
			arrVertex[1],
			arrVertex[2],
			arrVertex[0],
			arrVertex[2],
			arrVertex[3],
		};
		DeviceContext->UpdateSubresource(vertexBuffer, 0, NULL, vertices, 0, 0);
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

void Image::render(string srvKey, DV2 _scale, DV2 _trans, DCR _color, DV3 _rotate)
{
	//	Vertex Buffer Update
	{
		D3DXMATRIX s, rx, ry, rz, t;

		D3DXMatrixIdentity(&worldMatrix);

		D3DXMatrixScaling(&s, _scale.x , _scale.y , 1);

		D3DXMatrixRotationX(&rx, RAD(_rotate.x));
		D3DXMatrixRotationY(&ry, RAD(_rotate.y));
		D3DXMatrixRotationZ(&rz, RAD(_rotate.z));

		D3DXMatrixTranslation(&t, _trans.x, WINSIZEY - _trans.y, 0);

		worldMatrix = s * rx * ry * rz * t;
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);

		color = _color;
		DeviceContext->UpdateSubresource(colorBuffer, 0, NULL, color, 0, 0);

		PTVertex vertices[6] =
		{
			arrVertex[0],
			arrVertex[1],
			arrVertex[2],
			arrVertex[0],
			arrVertex[2],
			arrVertex[3],
		};
		DeviceContext->UpdateSubresource(vertexBuffer, 0, NULL, vertices, 0, 0);
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

void Image::NoiseRender(string srvKey, DV2 _scale, DV2 _trans, DCR _color, DV3 _rotate)
{
	//	Vertex Buffer Update
	{
		D3DXMATRIX s, rx, ry, rz, t;

		D3DXMatrixIdentity(&worldMatrix);

		//	Use Camera ScaleUp
		FLOAT percent = static_cast<FLOAT>((100 - whlCount) * 0.01F);
		_scale.x *= percent;
		_scale.y *= percent;

		_trans.x = _trans.x;
		_trans.y = (WINSIZEY - _trans.y);

		//	드래그를 이용한 중앙_ 확대 / 축소 시에 좌표 고정
		FLOAT Dif_Center_X = 640.0f - _trans.x;
		FLOAT Dif_Center_Y = 360.0f - _trans.y;

		if (fabsf(Dif_Center_X) < FLOAT_EPSILON)
			Dif_Center_X = (int)0;

		if (fabsf(Dif_Center_Y) < FLOAT_EPSILON)
			Dif_Center_Y = (int)0;

		_trans.x += ((1 - percent) * Dif_Center_X);
		_trans.y += ((1 - percent) * Dif_Center_Y);

		D3DXMatrixScaling(&s, _scale.x, _scale.y, 1);

		D3DXMatrixRotationX(&rx, RAD(_rotate.x));
		D3DXMatrixRotationY(&ry, RAD(_rotate.y));
		D3DXMatrixRotationZ(&rz, RAD(_rotate.z));

		D3DXMatrixTranslation(&t, _trans.x, _trans.y, 0);

		worldMatrix = s * rx * ry * rz * t;
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);

		color = _color;
		DeviceContext->UpdateSubresource(colorBuffer, 0, NULL, color, 0, 0);


		//ImGui::DragFloat("StartY", &NoiseStruct.startY, 0.01F, 0.0F, 1.0F);

		NoiseStruct.limitAlpha = rand() % 255;
		NoiseStruct.startY += (0.01F) * static_cast<float>(rand() % 100);
		if (NoiseStruct.startY > 1.0F)
			NoiseStruct.startY = (int)0;


		DeviceContext->UpdateSubresource(NoiseBuffer, 0, NULL, &NoiseStruct, 0, 0);
	}

	//	Render
	{
		SHADER->setShader("PT_Noise");

		UINT stride = sizeof(PTVertex);
		UINT offset = 0;

		DeviceContext->VSSetConstantBuffers(1, 1, &worldBuffer);
		DeviceContext->PSSetConstantBuffers(0, 1, &colorBuffer);
		DeviceContext->PSSetConstantBuffers(1, 1, &NoiseBuffer);

		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11ShaderResourceView* srv = IMAGEMAP->getTexture(srvKey);
		ID3D11ShaderResourceView* srv_Noize = IMAGEMAP->getTexture("NOIZE");

		DeviceContext->PSSetShaderResources(0, 1, &srv);
		DeviceContext->PSSetShaderResources(1, 1, &srv_Noize);

		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->Draw(6, 0);
	}
}

void Image::render(string srvKey, DV2 _scale, DV3 _trans, DCR _color, DV3 _rotate)
{
	//	Vertex Buffer Update
	{
		D3DXMATRIX s, rx, ry, rz, t;

		D3DXMatrixIdentity(&worldMatrix);

		D3DXMatrixScaling(&s, _scale.x, _scale.y, 1);

		D3DXMatrixRotationX(&rx, RAD(_rotate.x));
		D3DXMatrixRotationY(&ry, RAD(_rotate.y));
		D3DXMatrixRotationZ(&rz, RAD(_rotate.z));

		D3DXMatrixTranslation(&t, _trans.x, WINSIZEY - _trans.y, _trans.z);

		worldMatrix = s * rx * ry * rz * t;
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);

		color = _color;
		DeviceContext->UpdateSubresource(colorBuffer, 0, NULL, color, 0, 0);

		PTVertex vertices[6] =
		{
			arrVertex[0],
			arrVertex[1],
			arrVertex[2],
			arrVertex[0],
			arrVertex[2],
			arrVertex[3],
		};
		DeviceContext->UpdateSubresource(vertexBuffer, 0, NULL, vertices, 0, 0);
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

void Image::render(string srvKey, const D3DXMATRIX & worldMatrix, ID3D11Buffer*const* vertexBuffer)
{
	DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);

	//	Render
	{
		SHADER->setShader("PTBase");

		UINT stride = sizeof(PTVertex);
		UINT offset = 0;

		DeviceContext->VSSetConstantBuffers(1, 1, &worldBuffer);

		color = D3DXCOLOR(1, 1, 1, 1);
		DeviceContext->UpdateSubresource(colorBuffer, 0, NULL, color, 0, 0);
		DeviceContext->PSSetConstantBuffers(0, 1, &colorBuffer);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11ShaderResourceView* srv = IMAGEMAP->getTexture(srvKey);

		DeviceContext->PSSetShaderResources(0, 1, &srv);

		DeviceContext->IASetVertexBuffers(0, 1, vertexBuffer, &stride, &offset);
		DeviceContext->Draw(6, 0);
	}
}

void Image::render(string normalKey, string alphaKey, DV2 _scale, DV2 _trans, DCR _color, DV3 _rotate)
{
	//	Vertex Buffer Update
	{
		D3DXMATRIX s, rx, ry, rz, t;

		D3DXMatrixIdentity(&worldMatrix);

		D3DXMatrixScaling(&s, _scale.x, _scale.y, 1);

		D3DXMatrixRotationX(&rx, RAD(_rotate.x));
		D3DXMatrixRotationY(&ry, RAD(_rotate.y));
		D3DXMatrixRotationZ(&rz, RAD(_rotate.z));

		D3DXMatrixTranslation(&t, _trans.x, WINSIZEY - _trans.y, 0);

		worldMatrix = s * rx * ry * rz * t;
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);

		color = _color;
		DeviceContext->UpdateSubresource(colorBuffer, 0, NULL, color, 0, 0);

		PTVertex vertices[6] =
		{
			arrVertex[0],
			arrVertex[1],
			arrVertex[2],
			arrVertex[0],
			arrVertex[2],
			arrVertex[3],
		};
		DeviceContext->UpdateSubresource(vertexBuffer, 0, NULL, vertices, 0, 0);

	}

	//	Render
	{
		SHADER->setShader("PT_Alpha2");

		UINT stride = sizeof(PTVertex);
		UINT offset = 0;

		DeviceContext->VSSetConstantBuffers(1, 1, &worldBuffer);
		DeviceContext->PSSetConstantBuffers(0, 1, &colorBuffer);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11ShaderResourceView* srv = IMAGEMAP->getTexture(normalKey);

		//	alpha일경우, 자동으로 탐색
		//	그렇지 않을경우, 해당 KEY
		if (!alphaKey.compare("alpha"))
		{
			alphaKey.clear();
			alphaKey = normalKey;
			alphaKey.append("_alpha");
		}

		ID3D11ShaderResourceView* alphaSrv = IMAGEMAP->getTexture(alphaKey);
		if (alphaSrv == nullptr)
		{
			alphaKey.clear();
			alphaKey = "pic_";
			alphaKey.append(normalKey, 0, normalKey.find("("));
			alphaKey.append("_alpha");	
			alphaSrv = IMAGEMAP->getTexture(alphaKey);
		}

		DeviceContext->PSSetShaderResources(0, 1, &srv);
		DeviceContext->PSSetShaderResources(1, 1, &alphaSrv);

		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->Draw(6, 0);
	}
}

void Image::render(string normalKey, string alphaKey, DV2 _scale, DV2 _trans, DV2 minTexCord, DV2 maxTexCord, DCR _color, DV3 _rotate)
{
	//	Vertex Buffer Update
	{
		D3DXMATRIX s, rx, ry, rz, t;

		D3DXMatrixIdentity(&worldMatrix);

		D3DXMatrixScaling(&s, _scale.x, _scale.y, 1);

		D3DXMatrixRotationX(&rx, RAD(_rotate.x));
		D3DXMatrixRotationY(&ry, RAD(_rotate.y));
		D3DXMatrixRotationZ(&rz, RAD(_rotate.z));

		D3DXMatrixTranslation(&t, _trans.x, WINSIZEY - _trans.y, 0);

		worldMatrix = s * rx * ry * rz * t;
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);

		color = _color;
		DeviceContext->UpdateSubresource(colorBuffer, 0, NULL, color, 0, 0);

		mutableVertex[0] = PTVertex(DV3(-1.0f, -1.0f, 0.0f), DV2(minTexCord.x, maxTexCord.y));
		mutableVertex[1] = PTVertex(DV3(-1.0, 1.0, 0.0f), DV2(minTexCord.x, minTexCord.y));
		mutableVertex[2] = PTVertex(DV3(1.0, 1.0, 0.0f), DV2(maxTexCord.x, minTexCord.y));
		mutableVertex[3] = PTVertex(DV3(1.0, -1.0, 0.0f), DV2(maxTexCord.x, maxTexCord.y));

		PTVertex vertices[6] =
		{
			mutableVertex[0],
			mutableVertex[1],
			mutableVertex[2],
			mutableVertex[0],
			mutableVertex[2],
			mutableVertex[3],
		};
		DeviceContext->UpdateSubresource(vertexBuffer, 0, NULL, vertices, 0, 0);

	}

	//	Render
	{
		SHADER->setShader("PT_Alpha");

		UINT stride = sizeof(PTVertex);
		UINT offset = 0;

		DeviceContext->VSSetConstantBuffers(1, 1, &worldBuffer);
		DeviceContext->PSSetConstantBuffers(0, 1, &colorBuffer);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11ShaderResourceView* srv = IMAGEMAP->getTexture(normalKey);
		ID3D11ShaderResourceView* alphaSrv = IMAGEMAP->getTexture(alphaKey);

		DeviceContext->PSSetShaderResources(0, 1, &srv);
		DeviceContext->PSSetShaderResources(1, 1, &alphaSrv);

		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->Draw(6, 0);
	}
}

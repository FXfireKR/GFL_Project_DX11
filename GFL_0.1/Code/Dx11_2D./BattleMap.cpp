#include "stdafx.h"
#include "BattleMap.h"

BattleMap::BattleMap()
{
	addPos = 0;
	skyAngle = 0;
}

BattleMap::~BattleMap()
{
	for (auto& it : mAtlas)
		SAFE_DELETE(it.second);

	for (auto& it : vDrawOrder)
		it = nullptr;

	mAtlas.clear();
	vDrawOrder.clear();
}

void BattleMap::SettingImageString(const string _str)
{
	imgKey = _str;
	imgKey.erase(imgKey.find('.'), imgKey.size());

	imgPath = BTM_FILE_PATH_BASE;
	imgPath.append(_str);
}

void BattleMap::AtlasSetting(MapAtlas* atlas)
{
	DOUBLE perPix_x, perPix_y;

	perPix_x = (DOUBLE)1 / static_cast<DOUBLE>(imgSize.x);
	perPix_y = (DOUBLE)1 / static_cast<DOUBLE>(imgSize.y);

	atlas->uv_min.x = static_cast<FLOAT>((DOUBLE)atlas->origPos.x * perPix_x);
	atlas->uv_min.y = static_cast<FLOAT>((DOUBLE)atlas->origPos.y * perPix_y);

	atlas->uv_max.x = static_cast<FLOAT>((DOUBLE)(atlas->origPos.x + atlas->size.x) * perPix_x);
	atlas->uv_max.y = static_cast<FLOAT>((DOUBLE)(atlas->origPos.y + atlas->size.y) * perPix_y);

	atlas->vecVertex.reserve(6);

	atlas->vecVertex.push_back(PTVertex(DV3(-0.5f, -0.5f, 0),		DV2(atlas->uv_min.x, atlas->uv_max.y)));	//	0
	atlas->vecVertex.push_back(PTVertex(DV3(-0.5f, 0.5f, 0),		DV2(atlas->uv_min.x, atlas->uv_min.y)));	//	1
	atlas->vecVertex.push_back(PTVertex(DV3(0.5f, 0.5f, 0),		DV2(atlas->uv_max.x, atlas->uv_min.y)));	//	2

	atlas->vecVertex.push_back(PTVertex(DV3(-0.5f, -0.5f, 0),		DV2(atlas->uv_min.x, atlas->uv_max.y)));	//	0
	atlas->vecVertex.push_back(PTVertex(DV3(0.5f, 0.5f, 0),		DV2(atlas->uv_max.x, atlas->uv_min.y)));	//	2
	atlas->vecVertex.push_back(PTVertex(DV3(0.5f, -0.5f, 0),		DV2(atlas->uv_max.x, atlas->uv_max.y)));	//	3

	
	CreateVertexBuffer(&(atlas->vertexBuffer), sizeof(PTVertex) * 6, &(atlas->vecVertex[0]), D3D11_USAGE_IMMUTABLE);
}

void BattleMap::update()
{
	if (CameraPositionX < 0)
		CameraPositionX = 0;

	if (CameraPositionX > WINSIZEX + addPos)
	{
		addPos += WINSIZEX;
		skyAngle += 180;
		if (skyAngle > 180)
			skyAngle = 0;
	}

	if (CameraPositionX < addPos)
	{
		addPos -= WINSIZEX;
		skyAngle += 180;
		if (skyAngle > 180)
			skyAngle = 0;
	}

	if (CameraPositionY < -290)
		CameraPositionY = -290;

	else if (CameraPositionY > 150)
		CameraPositionY = 150;

	if (CameraPositionX > limitSize)
		CameraPositionX = limitSize;
}

void BattleMap::render()
{
	for (auto& it : vDrawOrder)
	{
		D3DXMATRIX worldMatrix, s, rx, ry, rz, t;
		D3DXMatrixScaling(&s, it->scale.x, it->scale.y, it->scale.z);

		D3DXMatrixRotationX(&rx, RAD(it->rotate.x));
		D3DXMatrixRotationY(&ry, RAD(it->rotate.y));

		if (it->isRotate)
			D3DXMatrixRotationY(&ry, RAD(skyAngle));

		D3DXMatrixRotationZ(&rz, RAD(it->rotate.z));

		FLOAT x_half = it->scale.x * 0.5f;
		FLOAT y_half = it->scale.y * 0.5f;

		D3DXMatrixTranslation(&t, it->position.x + x_half + addPos, (WINSIZEY - it->position.y) - y_half, it->position.z);

		worldMatrix = s * rx * ry* rz* t;

		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		DRAW->render(imgKey, worldMatrix, &(it->vertexBuffer));
	}

	if (CameraPositionX > addPos && CameraPositionX < WINSIZEX + addPos + 10)
	{
		for (auto& it : vDrawOrder)
		{
			D3DXMATRIX worldMatrix, s, rx, ry, rz, t;
			D3DXMatrixScaling(&s, it->scale.x, it->scale.y, it->scale.z);

			D3DXMatrixRotationX(&rx, RAD(it->rotate.x));
			D3DXMatrixRotationY(&ry, RAD(it->rotate.y));

			if (it->isRotate)
			{
				float addAngle = skyAngle + 180.0f;
				D3DXMatrixRotationY(&ry, RAD(addAngle));
			}

			D3DXMatrixRotationZ(&rz, RAD(it->rotate.z));

			FLOAT x_half = it->scale.x * 0.5f;
			FLOAT y_half = it->scale.y * 0.5f;

			D3DXMatrixTranslation(&t, it->position.x + x_half + addPos + WINSIZEX, (WINSIZEY - it->position.y) - y_half, it->position.z);

			worldMatrix = s * rx * ry* rz* t;

			D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
			DRAW->render(imgKey, worldMatrix, &(it->vertexBuffer));
		}
	}

	if (CameraPositionX < addPos && CameraPositionX > WINSIZEX - addPos - 10)
	{
		for (auto& it : vDrawOrder)
		{
			D3DXMATRIX worldMatrix, s, rx, ry, rz, t;
			D3DXMatrixScaling(&s, it->scale.x, it->scale.y, it->scale.z);

			D3DXMatrixRotationX(&rx, RAD(it->rotate.x));
			D3DXMatrixRotationY(&ry, RAD(it->rotate.y));

			if (it->isRotate)
			{
				float addAngle = skyAngle + 180.0f;
				D3DXMatrixRotationY(&ry, RAD(addAngle));
			}

			D3DXMatrixRotationZ(&rz, RAD(it->rotate.z));

			FLOAT x_half = it->scale.x * 0.5f;
			FLOAT y_half = it->scale.y * 0.5f;

			D3DXMatrixTranslation(&t, it->position.x + x_half - addPos - WINSIZEX, (WINSIZEY - it->position.y) - y_half, it->position.z);

			worldMatrix = s * rx * ry* rz* t;

			D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
			DRAW->render(imgKey, worldMatrix, &(it->vertexBuffer));
		}
	}


}

void BattleMap::testRender(const vector<POINT>& _rendList)
{
	D3DXMATRIX worldMatrix, s, rx, ry, rz, t;

	for (size_t i = 0; i < vDrawOrder.size(); ++i)
	{
		MapAtlas* iter = vDrawOrder[i];

		D3DXMatrixScaling(&s, iter->scale.x, iter->scale.y, iter->scale.z);

		D3DXMatrixRotationX(&rx, RAD(iter->rotate.x));
		D3DXMatrixRotationY(&ry, RAD(iter->rotate.y));

		if (iter->isRotate)
			D3DXMatrixRotationY(&ry, RAD(skyAngle));

		D3DXMatrixRotationZ(&rz, RAD(iter->rotate.z));

		FLOAT x_half = iter->scale.x * 0.5f;
		FLOAT y_half = iter->scale.y * 0.5f;

		D3DXMatrixTranslation(&t, iter->position.x + x_half + addPos, (WINSIZEY - iter->position.y) - y_half, iter->position.z);

		worldMatrix = s * rx * ry* rz* t;

		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		DRAW->render(imgKey, worldMatrix, &(iter->vertexBuffer));

		//if (i == 0)
		//{
		//	for (auto& it : _rendList)
		//	{
		//		//Ã¶Ç÷ ºÐ´ëÀÎ°¡
		//		if (it.x < 100)
		//			PLAYER->getIOPdoll_crntSquad(it.y)->render_Ellipse();
		//		else
		//			BDATA->getObject(it.y)->render_Ellipse();
		//	}
		//}
	}

	if (CameraPositionX > addPos && CameraPositionX < WINSIZEX + addPos + 10)
	{
		for (auto& it : vDrawOrder)
		{
			D3DXMatrixScaling(&s, it->scale.x, it->scale.y, it->scale.z);

			D3DXMatrixRotationX(&rx, RAD(it->rotate.x));
			D3DXMatrixRotationY(&ry, RAD(it->rotate.y));

			if (it->isRotate)
			{
				float addAngle = skyAngle + 180.0f;
				D3DXMatrixRotationY(&ry, RAD(addAngle));
			}

			D3DXMatrixRotationZ(&rz, RAD(it->rotate.z));

			FLOAT x_half = it->scale.x * 0.5f;
			FLOAT y_half = it->scale.y * 0.5f;

			D3DXMatrixTranslation(&t, it->position.x + x_half + addPos + WINSIZEX, (WINSIZEY - it->position.y) - y_half, it->position.z);

			worldMatrix = s * rx * ry* rz* t;

			D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
			DRAW->render(imgKey, worldMatrix, &(it->vertexBuffer));
		}
	}

	if (CameraPositionX < addPos && CameraPositionX > WINSIZEX - addPos - 10)
	{
		for (auto& it : vDrawOrder)
		{
			D3DXMatrixScaling(&s, it->scale.x, it->scale.y, it->scale.z);

			D3DXMatrixRotationX(&rx, RAD(it->rotate.x));
			D3DXMatrixRotationY(&ry, RAD(it->rotate.y));

			if (it->isRotate)
			{
				float addAngle = skyAngle + 180.0f;
				D3DXMatrixRotationY(&ry, RAD(addAngle));
			}

			D3DXMatrixRotationZ(&rz, RAD(it->rotate.z));

			FLOAT x_half = it->scale.x * 0.5f;
			FLOAT y_half = it->scale.y * 0.5f;

			D3DXMatrixTranslation(&t, it->position.x + x_half - addPos - WINSIZEX, (WINSIZEY - it->position.y) - y_half, it->position.z);

			worldMatrix = s * rx * ry* rz* t;

			D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
			DRAW->render(imgKey, worldMatrix, &(it->vertexBuffer));
		}
	}

}

#include "stdafx.h"
#include "spineBase.h"

#define SPINE_VERTEX_MAX_INPUT	1000
#define PER_UINT8_TO_DXCOLOR	0.003921568627451

spineBase::spineBase(spSkeletonData * skeletonData, spAnimationStateData * stateData)
{
	//	initialize
	{
		scale = D3DXVECTOR3(1, 1, 1);
		rotate = D3DXVECTOR3(0, 0, 0);
		trans = D3DXVECTOR3(0, 0, 0);

		worldBuffer = nullptr;
		worldVertices = nullptr;
		this->stateData = nullptr;
		state = nullptr;
		skeleton = nullptr;
		srv = nullptr;
		curAnimation = nullptr;
	}

	SHADER->CreateShader("PT", PTElementDesc, PTElementCount, PTShaderFile);

	worldVertices = (float*)(malloc(sizeof(float) * SPINE_VERTEX_MAX_INPUT));
	skeleton = spSkeleton_create(skeletonData);

	ownsAnimationStateData = stateData == 0;
	if (ownsAnimationStateData)stateData = spAnimationStateData_create(skeletonData);

	this->stateData = stateData;
	state = spAnimationState_create(stateData);
	skeletonData->defaultSkin->name;

	createVertex();

	CreateConstantBuffer(&worldBuffer, sizeof(D3DXMATRIX), &worldMatrix);
}

spineBase::~spineBase()
{
	delete[] worldVertices;
	spAnimationStateData_dispose(stateData);
	spAnimationState_dispose(state);
	spSkeleton_dispose(skeleton);

	for (auto& iter : SlotNode)
	{
		if (iter.second != nullptr)
		{
			SAFE_RELEASE(iter.second->colorBuffer);
			SAFE_RELEASE(iter.second->vertexBuffer);
			SAFE_DELETE(iter.second);
		}
	}

	SAFE_RELEASE(worldBuffer);
}

void spineBase::setShaderResourceView(string _key)
{
	imgKey = _key;
	srv = IMAGEMAP->getTexture(imgKey);
	assert(SUCCEEDED(srv != nullptr));
}

void spineBase::update(float delta)
{
	if (srv == NULL || srv == nullptr)
		srv = IMAGEMAP->getTexture(imgKey);

	spSkeleton_update(skeleton, delta);
	spAnimationState_update(state, delta * timeScale);
	spAnimationState_apply(state, skeleton);
	spSkeleton_updateWorldTransform(skeleton);

	spSkeleton_updateAllVertex();
}

void spineBase::render()
{
	SHADER->setShader("PT");

	D3DXMATRIX s, r, t;
	D3DXMatrixIdentity(&worldMatrix);

	D3DXMatrixScaling(&s, scale);
	D3DXMatrixRotating(&r, rotate);
	D3DXMatrixTranslation(&t, trans);

	t._42 = WINSIZEY - t._42;
	worldMatrix = s * r * t;

	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, worldMatrix, 0, 0);

	UINT stride = sizeof(PTVertex);
	UINT offset = 0;

	for (int i = 0; i < skeleton->slotsCount; ++i)
	{
		spSlot* slot = skeleton->drawOrder[i];
		spAttachment* attachment = slot->attachment;

		if (!attachment) continue;

		const auto& it = SlotNode[attachment->name];	
		
		DeviceContext->VSSetConstantBuffers(1, 1, &worldBuffer);

		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		DeviceContext->PSSetConstantBuffers(0, 1, &it->colorBuffer);

		srv = IMAGEMAP->getTexture(imgKey);
		DeviceContext->PSSetShaderResources(0, 1, &srv);

		DeviceContext->IASetVertexBuffers(0, 1, &it->vertexBuffer, &stride, &offset);
		DeviceContext->Draw(it->vecVertex.size(), 0);
	}
}

void spineBase::changeMotion_byName(string _aniKey, bool _loop, int _track)
{
	curAnimation = spAnimationState_setAnimationByName(state, _track, _aniKey.c_str(), _loop);
}

void spineBase::createVertex()
{
	for (int i = 0; i < skeleton->slotsCount; ++i)
	{
		spSlot* slot = skeleton->drawOrder[i];
		spAttachment* attachment = slot->attachment;

		if (!attachment) continue;

		SlotNode.insert(make_pair(slot->attachment->name, new spSpineNode));
		auto& node = SlotNode[slot->attachment->name];

		if (attachment->type == SP_ATTACHMENT_REGION)
		{
			spRegionAttachment* regionAttachment = (spRegionAttachment*)attachment;			
			spRegionAttachment_computeWorldVertices(regionAttachment, slot->bone, worldVertices);

			node->color.r = skeleton->r * slot->r;
			node->color.g = skeleton->g * slot->g;
			node->color.b = skeleton->b * slot->b;
			node->color.a = skeleton->a * slot->a;

			PTVertex vertices[4];
			vertices[0].Position.x = worldVertices[SP_VERTEX_X1];
			vertices[0].Position.y = worldVertices[SP_VERTEX_Y1];
			vertices[0].Position.z = 0;
			vertices[0].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X1];
			vertices[0].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y1];

			vertices[1].Position.x = worldVertices[SP_VERTEX_X2];
			vertices[1].Position.y = worldVertices[SP_VERTEX_Y2];
			vertices[1].Position.z = 0;
			vertices[1].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X2];
			vertices[1].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y2];

			vertices[2].Position.x = worldVertices[SP_VERTEX_X3];
			vertices[2].Position.y = worldVertices[SP_VERTEX_Y3];
			vertices[2].Position.z = 0;
			vertices[2].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X3];
			vertices[2].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y3];

			vertices[3].Position.x = worldVertices[SP_VERTEX_X4];
			vertices[3].Position.y = worldVertices[SP_VERTEX_Y4];
			vertices[3].Position.z = 0;
			vertices[3].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X4];
			vertices[3].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y4];

			node->vecVertex.reserve(6);

			node->vecVertex.push_back(vertices[0]);
			node->vecVertex.push_back(vertices[1]);
			node->vecVertex.push_back(vertices[2]);
			node->vecVertex.push_back(vertices[0]);
			node->vecVertex.push_back(vertices[2]);
			node->vecVertex.push_back(vertices[3]);
		}

		else if (attachment->type == SP_ATTACHMENT_MESH)
		{
			spMeshAttachment* mesh = (spMeshAttachment*)attachment;
			if (mesh->verticesCount > SPINE_VERTEX_MAX_INPUT) continue;
			spMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);

			node->color.r = skeleton->r * slot->r;
			node->color.g = skeleton->g * slot->g;
			node->color.b = skeleton->b * slot->b;
			node->color.a = skeleton->a * slot->a;


			node->vecVertex.reserve(mesh->trianglesCount);

			for (int i = 0; i < mesh->trianglesCount; ++i) {
				int index = mesh->triangles[i] << 1;

				PTVertex vertex;

				//	input Position
				vertex.Position.x = worldVertices[index];
				vertex.Position.y = worldVertices[index + 1];
				vertex.Position.z = 0;

				//	input Texcoord
				vertex.TexCoord.x = mesh->uvs[index];
				vertex.TexCoord.y = mesh->uvs[index + 1];

				node->vecVertex.push_back(vertex);
			}

		}

		else if (attachment->type == SP_ATTACHMENT_SKINNED_MESH)
		{
			spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*)attachment;
			if (mesh->uvsCount > SPINE_VERTEX_MAX_INPUT) continue;
			spSkinnedMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);

			node->color.r = skeleton->r * slot->r;
			node->color.g = skeleton->g * slot->g;
			node->color.b = skeleton->b * slot->b;
			node->color.a = skeleton->a * slot->a;

			node->vecVertex.reserve(mesh->trianglesCount);

			for (int i = 0; i < mesh->trianglesCount; ++i) {
				int index = mesh->triangles[i] << 1;

				PTVertex vertex;

				//	input Position
				vertex.Position.x = worldVertices[index];
				vertex.Position.y = worldVertices[index + 1];
				vertex.Position.z = 0;

				//	input Texcoord
				vertex.TexCoord.x = mesh->uvs[index];
				vertex.TexCoord.y = mesh->uvs[index + 1];

				node->vecVertex.push_back(vertex);
			}

		}

		CreateVertexBuffer(&node->vertexBuffer, sizeof(PTVertex) * node->vecVertex.size(), &node->vecVertex[0]);
		CreateConstantBuffer(&node->colorBuffer, sizeof(D3DXCOLOR), &node->color);
	}
}

void spineBase::createVertex(spSlot * slot, const spAttachment * attachment)
{
	SlotNode.insert(make_pair(slot->attachment->name, new spSpineNode));
	auto& node = SlotNode[slot->attachment->name];

	if (attachment->type == SP_ATTACHMENT_REGION)
	{
		spRegionAttachment* regionAttachment = (spRegionAttachment*)attachment;
		spRegionAttachment_computeWorldVertices(regionAttachment, slot->bone, worldVertices);

		node->color.r = skeleton->r * slot->r;
		node->color.g = skeleton->g * slot->g;
		node->color.b = skeleton->b * slot->b;
		node->color.a = skeleton->a * slot->a;

		PTVertex vertices[4];
		vertices[0].Position.x = worldVertices[SP_VERTEX_X1];
		vertices[0].Position.y = worldVertices[SP_VERTEX_Y1];
		vertices[0].Position.z = 0;
		vertices[0].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X1];
		vertices[0].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y1];

		vertices[1].Position.x = worldVertices[SP_VERTEX_X2];
		vertices[1].Position.y = worldVertices[SP_VERTEX_Y2];
		vertices[1].Position.z = 0;
		vertices[1].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X2];
		vertices[1].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y2];

		vertices[2].Position.x = worldVertices[SP_VERTEX_X3];
		vertices[2].Position.y = worldVertices[SP_VERTEX_Y3];
		vertices[2].Position.z = 0;
		vertices[2].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X3];
		vertices[2].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y3];

		vertices[3].Position.x = worldVertices[SP_VERTEX_X4];
		vertices[3].Position.y = worldVertices[SP_VERTEX_Y4];
		vertices[3].Position.z = 0;
		vertices[3].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X4];
		vertices[3].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y4];

		node->vecVertex.reserve(6);

		node->vecVertex.push_back(vertices[0]);
		node->vecVertex.push_back(vertices[1]);
		node->vecVertex.push_back(vertices[2]);
		node->vecVertex.push_back(vertices[0]);
		node->vecVertex.push_back(vertices[2]);
		node->vecVertex.push_back(vertices[3]);
	}

	else if (attachment->type == SP_ATTACHMENT_MESH)
	{
		spMeshAttachment* mesh = (spMeshAttachment*)attachment;
		if (mesh->verticesCount > SPINE_VERTEX_MAX_INPUT)
			assert(SUCCEEDED(false));
		spMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);

		node->color.r = skeleton->r * slot->r;
		node->color.g = skeleton->g * slot->g;
		node->color.b = skeleton->b * slot->b;
		node->color.a = skeleton->a * slot->a;


		node->vecVertex.reserve(mesh->trianglesCount);

		for (int i = 0; i < mesh->trianglesCount; ++i) {
			int index = mesh->triangles[i] << 1;

			PTVertex vertex;

			//	input Position
			vertex.Position.x = worldVertices[index];
			vertex.Position.y = worldVertices[index + 1];
			vertex.Position.z = 0;

			//	input Texcoord
			vertex.TexCoord.x = mesh->uvs[index];
			vertex.TexCoord.y = mesh->uvs[index + 1];

			node->vecVertex.push_back(vertex);
		}

	}

	else if (attachment->type == SP_ATTACHMENT_SKINNED_MESH)
	{
		spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*)attachment;
		if (mesh->uvsCount > SPINE_VERTEX_MAX_INPUT)
			assert(SUCCEEDED(false));
		spSkinnedMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);

		node->color.r = skeleton->r * slot->r;
		node->color.g = skeleton->g * slot->g;
		node->color.b = skeleton->b * slot->b;
		node->color.a = skeleton->a * slot->a;

		node->vecVertex.reserve(mesh->trianglesCount);

		for (int i = 0; i < mesh->trianglesCount; ++i) {
			int index = mesh->triangles[i] << 1;

			PTVertex vertex;

			//	input Position
			vertex.Position.x = worldVertices[index];
			vertex.Position.y = worldVertices[index + 1];
			vertex.Position.z = 0;

			//	input Texcoord
			vertex.TexCoord.x = mesh->uvs[index];
			vertex.TexCoord.y = mesh->uvs[index + 1];

			node->vecVertex.push_back(vertex);
		}

	}

	CreateVertexBuffer(&node->vertexBuffer, sizeof(PTVertex) * node->vecVertex.size(), &node->vecVertex[0]);
	CreateConstantBuffer(&node->colorBuffer, sizeof(D3DXCOLOR), &node->color);
}

void spineBase::updateVertex(spSlot * slot, const spAttachment * attachment)
{
	auto& node = SlotNode[slot->attachment->name];

	if (attachment->type == SP_ATTACHMENT_REGION)
	{
		spRegionAttachment* regionAttachment = (spRegionAttachment*)attachment;
		spRegionAttachment_computeWorldVertices(regionAttachment, slot->bone, worldVertices);

		node->color.r = skeleton->r * slot->r;
		node->color.g = skeleton->g * slot->g;
		node->color.b = skeleton->b * slot->b;
		node->color.a = skeleton->a * slot->a;

		PTVertex vertices[4];
		vertices[0].Position.x = worldVertices[SP_VERTEX_X1];
		vertices[0].Position.y = worldVertices[SP_VERTEX_Y1];
		vertices[0].Position.z = 0;
		vertices[0].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X1];
		vertices[0].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y1];

		vertices[1].Position.x = worldVertices[SP_VERTEX_X2];
		vertices[1].Position.y = worldVertices[SP_VERTEX_Y2];
		vertices[1].Position.z = 0;
		vertices[1].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X2];
		vertices[1].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y2];

		vertices[2].Position.x = worldVertices[SP_VERTEX_X3];
		vertices[2].Position.y = worldVertices[SP_VERTEX_Y3];
		vertices[2].Position.z = 0;
		vertices[2].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X3];
		vertices[2].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y3];

		vertices[3].Position.x = worldVertices[SP_VERTEX_X4];
		vertices[3].Position.y = worldVertices[SP_VERTEX_Y4];
		vertices[3].Position.z = 0;
		vertices[3].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X4];
		vertices[3].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y4];

		node->vecVertex[0] = (vertices[0]);
		node->vecVertex[1] = (vertices[1]);
		node->vecVertex[2] = (vertices[2]);
		node->vecVertex[3] = (vertices[0]);
		node->vecVertex[4] = (vertices[2]);
		node->vecVertex[5] = (vertices[3]);
	}

	else if (attachment->type == SP_ATTACHMENT_MESH)
	{
		spMeshAttachment* mesh = (spMeshAttachment*)attachment;
		if (mesh->verticesCount > SPINE_VERTEX_MAX_INPUT)
			assert(SUCCEEDED(false));
		spMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);


		node->color.r = skeleton->r * slot->r;
		node->color.g = skeleton->g * slot->g;
		node->color.b = skeleton->b * slot->b;
		node->color.a = skeleton->a * slot->a;

		for (int i = 0; i < mesh->trianglesCount; ++i) {
			int index = mesh->triangles[i] << 1;

			PTVertex vertex;

			//	input Position
			vertex.Position.x = worldVertices[index];
			vertex.Position.y = worldVertices[index + 1];
			vertex.Position.z = 0;

			//	input Texcoord
			vertex.TexCoord.x = mesh->uvs[index];
			vertex.TexCoord.y = mesh->uvs[index + 1];

			node->vecVertex[i] = (vertex);
		}
	}

	else if (attachment->type == SP_ATTACHMENT_SKINNED_MESH)
	{
		spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*)attachment;
		if (mesh->uvsCount > SPINE_VERTEX_MAX_INPUT)
			assert(SUCCEEDED(false));
		spSkinnedMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);


		node->color.r = skeleton->r * slot->r;
		node->color.g = skeleton->g * slot->g;
		node->color.b = skeleton->b * slot->b;
		node->color.a = skeleton->a * slot->a;

		for (int i = 0; i < mesh->trianglesCount; ++i) {
			int index = mesh->triangles[i] << 1;

			PTVertex vertex;

			//	input Position
			vertex.Position.x = worldVertices[index];
			vertex.Position.y = worldVertices[index + 1];
			vertex.Position.z = 0;

			//	input Texcoord
			vertex.TexCoord.x = mesh->uvs[index];
			vertex.TexCoord.y = mesh->uvs[index + 1];

			node->vecVertex[i] = (vertex);
		}

	}

	DeviceContext->UpdateSubresource(node->vertexBuffer, 0, NULL, &(node->vecVertex[0]), 0, 0);
	DeviceContext->UpdateSubresource(node->colorBuffer, 0, NULL, node->color, 0, 0);
}

void spineBase::spSkeleton_updateAllVertex()
{
	for (int i = 0; i < skeleton->slotsCount; ++i)
	{
		spSlot* slot = skeleton->drawOrder[i];
		spAttachment* attachment = slot->attachment;

		if (!attachment) continue;

		//	Slot에 존재하지않는 데이터인경우
		if (SlotNode.count(slot->attachment->name) == 0)
			createVertex(slot, attachment);

		else
			updateVertex(slot, attachment);
	}
}

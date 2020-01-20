#include "stdafx.h"
#include "spSkeletonDrawable.h"

#define SPINE_VERTEX_MAX_INPUT	1000
#define PER_UINT8_TO_DXCOLOR	0.003921568627451

spSkeletonDrawable::spSkeletonDrawable(spSkeletonData * skeletonData, spAnimationStateData * stateData)
{
	SHADER->CreateShader("PT", PTElementDesc, PTElementCount, PTShaderFile);

	//	Y축이 뒤집힘
	//spBone_setYDown(true);
	worldVertices = (float*)(malloc(sizeof(float) * SPINE_VERTEX_MAX_INPUT));
	skeleton = spSkeleton_create(skeletonData);

	ownsAnimationStateData = stateData == 0;
	if (ownsAnimationStateData)stateData = spAnimationStateData_create(skeletonData);

	this->stateData = stateData;
	state = spAnimationState_create(stateData);
	skeletonData->defaultSkin->name;

	IMAGEMAP->InsertImage_FromeFile("9A91", "../Asset/ak12.png");

}

spSkeletonDrawable::~spSkeletonDrawable()
{
}

void spSkeletonDrawable::update(float delta, size_t idx1, size_t idx2, float mixTime)
{
	spAnimation* a = (skeleton->data->animations[idx1]);
	spAnimation* b = (skeleton->data->animations[idx2]);

	spAnimationStateData_setMix(stateData, a, b, mixTime);

	spSkeleton_update(skeleton, delta);
	spAnimationState_update(state, delta * timeScale);
	spAnimationState_apply(state, skeleton);
	spSkeleton_updateWorldTransform(skeleton);

	update_Vertex();
}

void spSkeletonDrawable::update(float delta, size_t idx1, size_t idx2)
{
	spAnimation* a = (skeleton->data->animations[idx1]);
	spAnimation* b = (skeleton->data->animations[idx2]);

	spAnimationStateData_setMix(stateData, a, b, 0.125f);

	spSkeleton_update(skeleton, delta);
	spAnimationState_update(state, delta * timeScale);
	spAnimationState_apply(state, skeleton);
	spSkeleton_updateWorldTransform(skeleton);

	update_Vertex();
}

void spSkeletonDrawable::update(float delta)
{
	
	spSkeleton_update(skeleton, delta);
	spAnimationState_update(state, delta * timeScale);
	spAnimationState_apply(state, skeleton);
	spSkeleton_updateWorldTransform(skeleton);

	update_Vertex();
}

void spSkeletonDrawable::update_Vertex()
{
	// 자신의 KEY를 찾아서 업데이트 하게 바꿔야겠다.

	for (int i = 0; i < skeleton->slotsCount; ++i)
	{
		spSlot* slot = skeleton->drawOrder[i];
		spAttachment* attachment = slot->attachment;

		if (!attachment) continue;

		if (SlotNode.count(slot->attachment->name) == 0)
		{
			SlotNode.insert(make_pair(slot->attachment->name, new spSpineNode));
			auto& node = SlotNode[slot->attachment->name];

			if (attachment->type == SP_ATTACHMENT_REGION)
			{
				spRegionAttachment* regionAttachment = (spRegionAttachment*)attachment;
				//texture = (Texture*)((spAtlasRegion*)regionAttachment->rendererObject)->page->rendererObject;
				spRegionAttachment_computeWorldVertices(regionAttachment, slot->bone, worldVertices);

				node->color.r = skeleton->r * slot->r;
				node->color.g = skeleton->g * slot->g;
				node->color.b = skeleton->b * slot->b;
				node->color.a = skeleton->a * slot->a;

				/*Uint8 r = static_cast<Uint8>(skeleton->r * slot->r * 255);
				Uint8 g = static_cast<Uint8>(skeleton->g * slot->g * 255);
				Uint8 b = static_cast<Uint8>(skeleton->b * slot->b * 255);
				Uint8 a = static_cast<Uint8>(skeleton->a * slot->a * 255);*/

				//Vector2u size = texture->getSize();
				//vertices[0].color.r = r;
				//vertices[0].color.g = g;
				//vertices[0].color.b = b;
				//vertices[0].color.a = a;
				PTVertex vertices[4];
				vertices[0].Position.x = worldVertices[SP_VERTEX_X1];
				vertices[0].Position.y = worldVertices[SP_VERTEX_Y1];
				vertices[0].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X1];
				vertices[0].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y1];

				//vertices[1].color.r = r;
				//vertices[1].color.g = g;
				//vertices[1].color.b = b;
				//vertices[1].color.a = a;
				vertices[1].Position.x = worldVertices[SP_VERTEX_X2];
				vertices[1].Position.y = worldVertices[SP_VERTEX_Y2];
				vertices[1].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X2];
				vertices[1].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y2];

				//vertices[2].color.r = r;
				//vertices[2].color.g = g;
				//vertices[2].color.b = b;
				//vertices[2].color.a = a;
				vertices[2].Position.x = worldVertices[SP_VERTEX_X3];
				vertices[2].Position.y = worldVertices[SP_VERTEX_Y3];
				vertices[2].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X3];
				vertices[2].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y3];

				//vertices[3].color.r = r;
				//vertices[3].color.g = g;
				//vertices[3].color.b = b;
				//vertices[3].color.a = a;
				vertices[3].Position.x = worldVertices[SP_VERTEX_X4];
				vertices[3].Position.y = worldVertices[SP_VERTEX_Y4];
				vertices[3].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X4];
				vertices[3].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y4];

				//vertexArray->append(vertices[0]);
				//vertexArray->append(vertices[1]);
				//vertexArray->append(vertices[2]);
				//vertexArray->append(vertices[0]);
				//vertexArray->append(vertices[2]);
				//vertexArray->append(vertices[3]);

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
				////texture = (Texture*)((AtlasRegion*)mesh->rendererObject)->page->rendererObject;
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
				//if (mesh->uvsCount > SPINE_VERTEX_MAX_INPUT) continue;
				//texture = (Texture*)((AtlasRegion*)mesh->rendererObject)->page->rendererObject;
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

			D3DXMatrixIdentity(&node->worldMatrix);
			CreateConstantBuffer(&node->worldBuffer, sizeof(D3DXMATRIX), &node->worldMatrix);

			CreateConstantBuffer(&node->colorBuffer, sizeof(D3DXCOLOR), &node->color);
		}

		auto& node = SlotNode[slot->attachment->name];

		if (attachment->type == SP_ATTACHMENT_REGION)
		{
			spRegionAttachment* regionAttachment = (spRegionAttachment*)attachment;
			//texture = (Texture*)((spAtlasRegion*)regionAttachment->rendererObject)->page->rendererObject;
			spRegionAttachment_computeWorldVertices(regionAttachment, slot->bone, worldVertices);

			node->color.r = skeleton->r * slot->r;
			node->color.g = skeleton->g * slot->g;
			node->color.b = skeleton->b * slot->b;
			node->color.a = skeleton->a * slot->a;


			//Vector2u size = texture->getSize();
			//vertices[0].color.r = r;
			//vertices[0].color.g = g;
			//vertices[0].color.b = b;
			//vertices[0].color.a = a;
			PTVertex vertices[4];
			vertices[0].Position.x = worldVertices[SP_VERTEX_X1];
			vertices[0].Position.y = worldVertices[SP_VERTEX_Y1];
			vertices[0].Position.z = 0;
			vertices[0].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X1];
			vertices[0].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y1];

			//vertices[1].color.r = r;
			//vertices[1].color.g = g;
			//vertices[1].color.b = b;
			//vertices[1].color.a = a;
			vertices[1].Position.x = worldVertices[SP_VERTEX_X2];
			vertices[1].Position.y = worldVertices[SP_VERTEX_Y2];
			vertices[1].Position.z = 0;
			vertices[1].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X2];
			vertices[1].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y2];

			//vertices[2].color.r = r;
			//vertices[2].color.g = g;
			//vertices[2].color.b = b;
			//vertices[2].color.a = a;
			vertices[2].Position.x = worldVertices[SP_VERTEX_X3];
			vertices[2].Position.y = worldVertices[SP_VERTEX_Y3];
			vertices[2].Position.z = 0;
			vertices[2].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X3];
			vertices[2].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y3];

			//vertices[3].color.r = r;
			//vertices[3].color.g = g;
			//vertices[3].color.b = b;
			//vertices[3].color.a = a;
			vertices[3].Position.x = worldVertices[SP_VERTEX_X4];
			vertices[3].Position.y = worldVertices[SP_VERTEX_Y4];
			vertices[3].Position.z = 0;
			vertices[3].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X4];
			vertices[3].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y4];

			//vertexArray->append(vertices[0]);
			//vertexArray->append(vertices[1]);
			//vertexArray->append(vertices[2]);
			//vertexArray->append(vertices[0]);
			//vertexArray->append(vertices[2]);
			//vertexArray->append(vertices[3]);

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
			if (mesh->verticesCount > SPINE_VERTEX_MAX_INPUT) continue;
			////texture = (Texture*)((AtlasRegion*)mesh->rendererObject)->page->rendererObject;
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


			//vector<D3DXVERTEX> vertices;
			//vertices.reserve(mesh->trianglesCount);

			//for (int i = 0; i < mesh->trianglesCount; ++i) {

			//	D3DXVERTEX vertex;

			//	int index = mesh->triangles[i] << 1;
			//	vertex.position.x = worldVertices[index];
			//	vertex.position.y = worldVertices[index + 1];
			//	vertex.texCoords.x = mesh->uvs[index];
			//	vertex.texCoords.y = mesh->uvs[index + 1];

			//	vertices.push_back(vertex);
			//	//vertexArray->append(vertex);
		}

		else if (attachment->type == SP_ATTACHMENT_SKINNED_MESH)
		{
			spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*)attachment;
			//if (mesh->uvsCount > SPINE_VERTEX_MAX_INPUT) continue;
			//texture = (Texture*)((AtlasRegion*)mesh->rendererObject)->page->rendererObject;
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

		//node->color = D3DXCOLOR(1, 1, 1, 1);
		DeviceContext->UpdateSubresource(node->vertexBuffer, 0, NULL, &(node->vecVertex[0]), 0, 0);
		DeviceContext->UpdateSubresource(node->colorBuffer, 0, NULL, node->color, 0, 0);
	}
}

void spSkeletonDrawable::render()
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
			//texture = (Texture*)((spAtlasRegion*)regionAttachment->rendererObject)->page->rendererObject;
			spRegionAttachment_computeWorldVertices(regionAttachment, slot->bone, worldVertices);

			node->color.r = skeleton->r * slot->r;
			node->color.g = skeleton->g * slot->g;
			node->color.b = skeleton->b * slot->b;
			node->color.a = skeleton->a * slot->a;

			/*Uint8 r = static_cast<Uint8>(skeleton->r * slot->r * 255);
			Uint8 g = static_cast<Uint8>(skeleton->g * slot->g * 255);
			Uint8 b = static_cast<Uint8>(skeleton->b * slot->b * 255);
			Uint8 a = static_cast<Uint8>(skeleton->a * slot->a * 255);*/

			//Vector2u size = texture->getSize();
			//vertices[0].color.r = r;
			//vertices[0].color.g = g;
			//vertices[0].color.b = b;
			//vertices[0].color.a = a;
			PTVertex vertices[4];
			vertices[0].Position.x = worldVertices[SP_VERTEX_X1];
			vertices[0].Position.y = worldVertices[SP_VERTEX_Y1];
			vertices[0].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X1];
			vertices[0].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y1];

			//vertices[1].color.r = r;
			//vertices[1].color.g = g;
			//vertices[1].color.b = b;
			//vertices[1].color.a = a;
			vertices[1].Position.x = worldVertices[SP_VERTEX_X2];
			vertices[1].Position.y = worldVertices[SP_VERTEX_Y2];
			vertices[1].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X2];
			vertices[1].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y2];

			//vertices[2].color.r = r;
			//vertices[2].color.g = g;
			//vertices[2].color.b = b;
			//vertices[2].color.a = a;
			vertices[2].Position.x = worldVertices[SP_VERTEX_X3];
			vertices[2].Position.y = worldVertices[SP_VERTEX_Y3];
			vertices[2].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X3];
			vertices[2].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y3];

			//vertices[3].color.r = r;
			//vertices[3].color.g = g;
			//vertices[3].color.b = b;
			//vertices[3].color.a = a;
			vertices[3].Position.x = worldVertices[SP_VERTEX_X4];
			vertices[3].Position.y = worldVertices[SP_VERTEX_Y4];
			vertices[3].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X4];
			vertices[3].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y4];

			//vertexArray->append(vertices[0]);
			//vertexArray->append(vertices[1]);
			//vertexArray->append(vertices[2]);
			//vertexArray->append(vertices[0]);
			//vertexArray->append(vertices[2]);
			//vertexArray->append(vertices[3]);

			
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
			////texture = (Texture*)((AtlasRegion*)mesh->rendererObject)->page->rendererObject;
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
			//if (mesh->uvsCount > SPINE_VERTEX_MAX_INPUT) continue;
			//texture = (Texture*)((AtlasRegion*)mesh->rendererObject)->page->rendererObject;
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

		D3DXMatrixIdentity(&node->worldMatrix);
		CreateConstantBuffer(&node->worldBuffer, sizeof(D3DXMATRIX), &node->worldMatrix);

		CreateConstantBuffer(&node->colorBuffer, sizeof(D3DXCOLOR), &node->color);
	}

}

void spSkeletonDrawable::tempVertexRender()
{
	SHADER->setShader("PT");

	for (int i = 0; i < skeleton->slotsCount; ++i)
	{
		spSlot* slot = skeleton->drawOrder[i];
		spAttachment* attachment = slot->attachment;

		if (!attachment) continue;

		auto& it = SlotNode[slot->attachment->name];

		UINT stride = sizeof(PTVertex);
		UINT offset = 0;

		D3DXMatrixIdentity(&it->worldMatrix);

		D3DXMATRIX s, t;

		D3DXMatrixScaling(&s, 0.005, 0.005, 0);
		D3DXMatrixTranslation(&t, 0, 0, 0);

		it->worldMatrix = s * t;

		D3DXMatrixTranspose(&it->worldMatrix, &it->worldMatrix);
		DeviceContext->UpdateSubresource(it->worldBuffer, 0, NULL, it->worldMatrix, 0, 0);
		DeviceContext->VSSetConstantBuffers(1, 1, &it->worldBuffer);
		
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11ShaderResourceView* srv = IMAGEMAP->getResourceView("9A91");
		DeviceContext->PSSetConstantBuffers(0, 1, &it->colorBuffer);
		DeviceContext->PSSetShaderResources(0, 1, &srv);

		DeviceContext->IASetVertexBuffers(0, 1, &it->vertexBuffer, &stride, &offset);
		DeviceContext->Draw(it->vecVertex.size(), 0);
	}

}
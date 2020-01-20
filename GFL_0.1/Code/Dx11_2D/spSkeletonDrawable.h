#pragma once

struct spSpineNode
{
	vector<PTVertex>		vecVertex;

	D3DXMATRIX				worldMatrix;	//	Identity
	D3DXCOLOR				color;

	ID3D11Buffer*			worldBuffer;
	ID3D11Buffer*			vertexBuffer;
	ID3D11Buffer*			colorBuffer;
};

class spSkeletonDrawable
{
public:
	spSkeleton* skeleton;
	spAnimationState* state;
	spAnimationStateData* stateData;
	float timeScale;

	float* worldVertices;
	bool ownsAnimationStateData;


	map<string, spSpineNode*> SlotNode;

public:
	spSkeletonDrawable(spSkeletonData* skeletonData, spAnimationStateData* stateData = 0);
	~spSkeletonDrawable();

	void update(float delta, size_t idx1, size_t idx2, float mixTime);
	void update(float delta, size_t idx1, size_t idx2);
	void update(float delta);
	void update_Vertex();
	void render();
	void tempVertexRender();
};


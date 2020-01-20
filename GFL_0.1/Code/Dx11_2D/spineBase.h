#pragma once

struct spSpineNode
{
	vector<PTVertex>		vecVertex;

	D3DXCOLOR				color;

	ID3D11Buffer*			vertexBuffer;
	ID3D11Buffer*			colorBuffer;
};

class spineBase
{
private:
	spSkeleton*				skeleton;
	spTrackEntry*			curAnimation;
	spAnimationState*		state;
	spAnimationStateData*	stateData;

	float*					worldVertices;
	bool					ownsAnimationStateData;

	D3DXMATRIX				worldMatrix;
	D3DXVECTOR3				scale;
	D3DXVECTOR3				rotate;
	D3DXVECTOR3				trans;

	ID3D11Buffer*			worldBuffer;

	ID3D11ShaderResourceView* srv;

	map<string, spSpineNode*> SlotNode;

public:
	string					imgKey;
	float					timeScale;

public:
	spineBase(spSkeletonData* skeletonData, spAnimationStateData* stateData = NULL);
	~spineBase();

	void setShaderResourceView(string _key);
	void update(float delta);
	void render();

	void changeMotion_byName(string _aniKey, bool _loop = false, int _track = 0);

private:
	void createVertex();
	void createVertex(__in spSlot* slot, __in const spAttachment* attachment);
	void updateVertex(__in spSlot* slot, __in const spAttachment* attachment);
	void spSkeleton_updateAllVertex();

public:
	inline spSkeleton* getSkeleton() const { return skeleton; }
	inline spAnimationState* getState() const { return state; }
	inline spAnimationStateData* getStateData() const { return stateData; }
	inline spTrackEntry* getTrackEntry() const { return curAnimation; }

	inline D3DXVECTOR3& p_getScale() { return scale; }
	inline const D3DXVECTOR3 getScale() { return scale; }

	inline D3DXVECTOR3& p_getRotate() { return rotate; }
	inline const D3DXVECTOR3 getRotate() { return rotate; }

	inline D3DXVECTOR3& p_getTrans() { return trans; }
	inline const D3DXVECTOR3 getTrans() { return trans; }
};


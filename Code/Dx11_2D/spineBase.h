#ifndef SPINEBASE_H
#define SPINEBASE_H

struct spSpineNode
{
	vector<PTVertex>	vecVertex;

	COLR				color;

	DBuffer*			vertexBuffer;
	DBuffer*			colorBuffer;
};

class spineBase
{
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

	inline VEC3& p_getScale() { return scale; }
	inline const VEC3 getScale() { return scale; }

	inline VEC3& p_getRotate() { return rotate; }
	inline const VEC3 getRotate() { return rotate; }

	inline VEC3& p_getTrans() { return trans; }
	inline const VEC3 getTrans() { return trans; }

private:
	spSkeleton*				skeleton;
	spTrackEntry*			curAnimation;
	spAnimationState*		state;
	spAnimationStateData*	stateData;

	float*					worldVertices;
	bool					ownsAnimationStateData;

	MATRIX					worldMatrix;
	VEC3					scale;
	VEC3					rotate;
	VEC3					trans;

	DBuffer*				worldBuffer;

	ShaderResourceView*		srv;

	unordered_map<string, spSpineNode*> SlotNode;

public:
	string					imgKey;
	float					timeScale;

};

#endif
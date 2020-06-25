#pragma once

struct btmMapAtlas
{
	string				name;

	D3DXVECTOR3			scale;
	D3DXVECTOR3			rotate;
	D3DXVECTOR3			position;

	D3DXVECTOR2			uv_min;
	D3DXVECTOR2			uv_max;

	POINT				origPos;
	POINT				size;
	POINT				offset;

	size_t				drawIndex;

	vector<PTVertex>	vecVertex;
	ID3D11Buffer*		vertexBuffer;

	bool				isRotate;
};
typedef btmMapAtlas MapAtlas;

class BattleMap
{
public:
	BattleMap();
	~BattleMap();

	void SettingImageString(const string _str);
	void AtlasSetting(MapAtlas* atlas);

	void update();
	void render();
	void testRender(const vector<POINT>& _rendList);

private:
	const string BTM_FILE_PATH_BASE = "BattleMap/";

public:
	string						imgPath;
	string						imgKey;
	POINT						imgSize;
	FLOAT						limitSize;

	map<string, MapAtlas*>		mAtlas;			//	맵에 그려지기 대기하는 대상들
	vector<MapAtlas*>			vDrawOrder;		//	그려질 순서

	float						addPos;
	float						skyAngle;
};
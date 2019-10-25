#pragma once
//�߻� Ŭ����
class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual HRESULT init() = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
};
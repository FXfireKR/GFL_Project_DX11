#pragma once
//�߻� Ŭ����
class Scene
{
public:
	Scene();
	virtual ~Scene();
	
	//virtual HRESULT awak() = 0;		// ���ʻ����Լ�
	virtual HRESULT init() = 0;			// ���� �ʱ�ȭ �Լ�
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
};
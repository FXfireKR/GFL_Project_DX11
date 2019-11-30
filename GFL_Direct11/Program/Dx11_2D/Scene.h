#pragma once
//추상 클래스
class Scene
{
public:
	Scene();
	virtual ~Scene();
	
	//virtual HRESULT awak() = 0;		// 최초생성함수
	virtual HRESULT init() = 0;			// 이후 초기화 함수
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
};
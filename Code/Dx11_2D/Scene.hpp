#ifndef SCENE_HPP
#define SCENE_HPP
class Scene {
public:
	Scene() {}
	virtual ~Scene() {}
	virtual void init() PURE;
	virtual void release() PURE;
	virtual void update() PURE;
	virtual void render() PURE;
};
#endif
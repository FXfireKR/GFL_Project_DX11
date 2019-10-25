#pragma once
interface GameNode
{
public:
	virtual void Init() PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
};
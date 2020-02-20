#pragma once
#include "TaticDoll.h"

class Pyxis : public TaticDoll
{
private:
	const float		PYXIS_ACL_X = 0.8f;
	const float		PYXIS_ACL_Y = 0.8f;

	BYTE			safeTirgger;
	D3DXCOLOR		elipseColor;

	bool			setComplete;

public:
	Pyxis();
	~Pyxis();

	virtual void LoadTray_SoundList()override;
	virtual void LoadTray_ImageList()override;
	virtual HRESULT init()override;
	virtual void release()override;
	virtual void update()override;
	virtual void render()override;
	virtual void Use_ActiveSkill()override;
	virtual void MotionUpdate()override;
	virtual void Update_DrawPos()override;

	virtual void render_VisualBar()override;
	virtual void render_Motion()override;
	virtual void render_Ellipse()override;

	static void Pyxis_Set_Action(void* _this);
	static void Pyxis_Attack_Action(void* _this);
};
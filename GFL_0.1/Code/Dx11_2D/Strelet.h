#pragma once
#include "TaticDoll.h"

class Strelet : public TaticDoll
{
private:
	const double	STRELET_SPEED = 180.0;
	const float		STRELET_ACL_X = 0.8f;
	const float		STRELET_ACL_Y = 0.8f;

	BYTE			safeTirgger;
	D3DXCOLOR		elipseColor;

public:
	Strelet();
	~Strelet();

	
	void LoadTray_SoundList()override;
	void LoadTray_ImageList()override;
	HRESULT init()override;
	void release()override;
	void update()override;
	void render()override;
	void Use_ActiveSkill()override;
	void MotionUpdate()override;
	void Update_DrawPos()override;

	void render_VisualBar()override;
	void render_Motion()override;
	void render_Ellipse()override;

public:
	static void Strelet_Attack_Action(void* _this);
};
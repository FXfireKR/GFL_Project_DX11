#pragma once
#include "TaticDoll.h"

class Drone : public TaticDoll
{
private:
	const float		DRONE_ACL_X = 0.8f;
	const float		DRONE_ACL_Y = 0.8f;

	D3DXCOLOR elipseColor;

public:
	Drone();
	~Drone();

	static void LoadImageList();
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
};
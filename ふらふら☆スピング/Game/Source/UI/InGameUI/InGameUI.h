#pragma once
#include "Source/Source.h"

class InGameUI:public Source
{
	public:
	InGameUI();
	virtual ~InGameUI();
	virtual void Update();
	virtual bool Start();
	virtual void Render(RenderContext& rc);
	void SetBatPosition(const Vector3&pos);
	void SetBatRotation(float rotDeg);
private:
	SpriteRender m_spriteRender;
	SpriteRender m_spriteRenderBat;
	Vector3 m_batPosition = Vector3::Zero;
	Quaternion m_batRotation = Quaternion::Identity;
	float m_rad;
};


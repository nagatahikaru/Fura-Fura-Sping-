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
	FontRender m_fontRender;
	SpriteRender m_spriteRender;
	SpriteRender m_spriteRenderBat;
	SpriteRender m_spriteRenderMeet;
	Vector3 m_batPosition = Vector3::Zero;
	Vector3 m_meetPosition = Vector3::Zero;
	Quaternion m_batRotation = Quaternion::Identity;
	float m_rad;
	Vector3 m_meetPos;
};


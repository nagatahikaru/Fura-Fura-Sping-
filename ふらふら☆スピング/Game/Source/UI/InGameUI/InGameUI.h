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
	void SetUIVisible(bool isVisible) { m_isUIVisible = isVisible; }
	void SetFontVisble(bool isVisible) { m_isFontVisible = isVisible; }
	void SetReplayVisible(bool isVisible){ m_isReplayVisible = isVisible; }
private:
	FontRender m_fontRender;
	FontRender m_fontBollRender;
	SpriteRender m_spriteRender;
	SpriteRender m_spriteRenderBat;
	SpriteRender m_spriteRenderMeet;
	SpriteRender m_spriteRenderReplay;
	SpriteRender m_spriteRenderBall;
	Vector3 m_batPosition = Vector3::Zero;
	Vector3 m_meetPosition = Vector3::Zero;
	Quaternion m_batRotation = Quaternion::Identity;
	float m_rad;
	Vector3 m_meetPos;
	float m_km;
	bool m_isUIVisible = true;
	bool m_isFontVisible = true;
	bool m_isReplayVisible = true;
};


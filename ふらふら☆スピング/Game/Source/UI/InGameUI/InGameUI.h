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
	void SetBatterSide(bool isLeft);
	void SetPause(bool isPaused);
	//int GetGuruguruValue() const { return m_guruguruValue; }
	//int GetKmValue() const { return m_kmValue; }
private:
	FontRender m_fontRender;
	FontRender m_fontBollRender;
	SpriteRender m_spriteRender;
	SpriteRender m_spriteRenderBat;
	SpriteRender m_spriteRenderMeet;
	SpriteRender m_spriteRenderReplay;
	SpriteRender m_spriteRenderBall;
	Vector3 m_batPositionRight = Vector3::Zero;
	Vector3 m_batPositionLeft = Vector3::Zero;
	Vector3 m_meetPositionRight = Vector3::Zero;
	Vector3 m_meetPositionLeft = Vector3::Zero;
	Quaternion m_batRotation = Quaternion::Identity;
	float m_rad;
	Vector3 m_meetPos;
	Vector3 m_batPos;
	Vector3 m_offset;
	float m_km;
	bool m_isUIVisible = true;
	bool m_isFontVisible = true;
	bool m_isReplayVisible = true;
	bool m_isLeftBatter = true;
	bool m_isPaused = false;
	float batScaleX;
	float m_meetScaleX;
};


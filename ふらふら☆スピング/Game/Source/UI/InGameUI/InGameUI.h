#pragma once
#include "Source/Source.h"

class InGameUI :public Source
{
public:
	InGameUI();
	virtual ~InGameUI();
	virtual void Update();
	virtual bool Start();
	virtual void Render(RenderContext& rc);
	void SetBatPosition(const Vector3& pos);
	void SetBatRotation(float rotDeg);
	void SetUIVisible(bool isVisible) { m_isUIVisible = isVisible; }
	void SetFontVisble(bool isVisible) { m_isFontVisible = isVisible; }
	void SetReplayVisible(bool isVisible) { m_isReplayVisible = isVisible; }
	void SetBatterSide(bool isLeft);
	void SetPause(bool isPaused);
	void SetPredictedBallPos(const Vector3& pos3D);
	Vector3 ConvertBall3DToUI(const Vector3& pos3D);
	void FixBallUI(const Vector3& pos3D);
	void SetMeetCursorPosition(Vector3 m_inputOffset);
	Vector3 GetMeetCursorPosition() const { return m_meetPos; }
	void SetKm(float km) { m_km = km; }
	void SetStartZ(float z);
	void SetGuruGuruCount(int count)
	{
		m_guruGuruCount = count;
	}
	bool GetIsError() const { return m_isError; }
	void DisableErrorCheck() { m_isErrorCheckEnabled = false; }
	void SetGuruGuruTimer(float time);
	void SetBaisokuVisible(bool isVisible);
	float m_shuchusenTimer = 0.0f;
	void OnButtonPressed();
private:
	FontRender m_fontRender;
	FontRender m_fontBollRender;
	SpriteRender m_spriteRender;
	SpriteRender m_spriteRenderBat;
	SpriteRender m_spriteRenderMeet;
	SpriteRender m_spriteRenderReplay;
	SpriteRender m_spriteRenderBall;
	SpriteRender m_kiiro;
	SpriteRender m_besu;
	Vector3 m_batPositionRight = Vector3::Zero;
	Vector3 m_batPositionLeft = Vector3::Zero;
	Vector3 m_meetPositionRight = Vector3::Zero;
	Vector3 m_meetPositionLeft = Vector3::Zero;
	Quaternion m_batRotation = Quaternion::Identity;
	float m_rad;
	Vector3 m_meetPos;
	Vector3 m_batPos;
	Vector3 m_offset;
	float m_km = 0.0f;
	bool m_isUIVisible = true;
	bool m_isFontVisible = true;
	bool m_isReplayVisible = true;
	bool m_isLeftBatter = true;
	bool m_isPaused = false;
	float batScaleX;
	float m_meetScaleX;
	Vector3 m_predictedBallPos3D = Vector3::Zero;
	bool m_hasPredictedBall = false;
	bool m_isBallUIFixed = false;
	Vector3 m_fixedBallUIPos = Vector3::Zero;
	FontRender m_debugFont;
	FontRender m_debugFont2;
	float m_startZ = 0.0f;
	bool m_isError = false;
	int m_guruGuruCount = 0;
	bool  m_isErrorCheckEnabled = false;
	float m_guruGuruTimer = 0.0f;
	FontRender m_Count;
	SpriteRender m_baisoku;
	bool m_isBaisokuVisible = false;
	SpriteRender m_shuchusen;
	SpriteRender m_yazirusi;
	SpriteRender m_konto;
	float m_yazirusiRot = 0.0f;
	Quaternion m_yazirusiRotation;
	float      m_yazirusiAngleDeg = 0.0f;
	SpriteRender m_mawase;
	SpriteRender m_taimingu;
	SpriteRender m_Abotan;
	SpriteRender m_Abotan2;
	bool m_isButtonPressed = false;
	float m_buttonPressTimer = 0.0f;
	float m_uiToggleTimer = 0.0f;
	bool m_isAltUI = false;   // 交互UIフラグ
	SpriteRender m_gizagiza;
	float m_ballAlpha = 0.0f;   // 透明度（0=透明, 1=完全表示）
};


#pragma once
#include "Source/Source.h"

class InGameUI :public Source
{
public:
	enum PredictionType {
		Prediction_None,
		Prediction_Nice,
		Prediction_Great,
		Prediction_Excellent,
		Prediction_Perfect
	};
	PredictionType m_predictionType = Prediction_None;
	inline float Lerp(float a, float b, float t) {
		return a + (b - a) * t;
	}
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
	void SetBaisokuVisible(bool isVisible);
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
	float m_shuchusenTimer = 0.0f;
	float m_shuchusenTimer2 = 0.0f;
	void OnButtonPressed();
	void SetBallCount(int count);
	int GetGuruGuruCount() const;
	void StartFadeOut(float speed);
	void StartFadeIn(float speed);
	bool IsFadingOut() const { return m_isFadeOut; }
	void StartStrikeAnim();
	void ResetPrediction();
	float GetPredictedDistance() const { return m_predictedDistance; }
	void OnStrike(int ballIndex);
	void ResetBatAndMeetOnly();
	void ShowPrediction(float predicted);
	bool IsFadingIn() const { return m_isFadeIn; }
	std::function<void()> m_onFadeOutFinished;
	std::function<void()> m_onFadeInFinished;
	int m_threeShots[3] = { 0,0,0 };      // cm 単位で保存
	bool m_shotDone[3] = { false,false,false }; // 打ったかどうか
	void SetCursorScale(float distance)
	{
		m_meetCursorScale = distance;
	};
	float m_perfectAnimTimer = 0.0f;
	bool  m_isPerfectAnimActive = false;

private:

	FontRender m_fontRender;
	FontRender m_fontBollRender1;
	FontRender m_fontBollRender2;
	FontRender m_fontBollRender3;
	ModelRender m_wakuModel;
	SpriteRender m_spriteRenderBat;
	SpriteRender m_spriteRenderMeet;
	SpriteRender m_spriteRenderReplay;
	SpriteRender m_spriteRenderBall;
	SpriteRender m_kiiro1;
	SpriteRender m_kiiro2;
	SpriteRender m_kiiro3;
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
	bool m_baisokuUI = true;
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
	int m_ballCount = 1;  // 1球目〜3球目
	FontRender m_fontBallCount; // 新しいフォント
	SpriteRender m_ballIcon[3];
	SpriteRender m_spritekuro;
	float m_fadeAlpha = 0.0f;
	bool  m_isFadeOut = false;
	float m_fadeSpeed = 0.5f; // 2秒で真っ黒になる例
	bool  m_isFadeIn = false;
	SpriteRender m_bbb;
	SpriteRender m_bsuki;
	FontRender m_fontDebug1;
	FontRender m_fontDebug2;
	FontRender m_fontDebug3;
	FontRender m_fontDebug4;
	FontRender m_fontDebug5;
	SpriteRender m_strikeSprite;
	float m_meetCursorScale = 1.0; // ミートカーソルの基本スケール
	// --- ストライク演出用 ---
	float m_strikeTimer = 0.0f;
	bool  m_isStrikeAnim = false;
	float m_strikeHoldTime = 0.0f;   // 表示を維持する時間
	bool m_isPredictionVisible = false;
	SpriteRender m_niceSprite;
	SpriteRender m_greatSprite;
	SpriteRender m_excellentSprite;
	SpriteRender m_perfectSprite;
	float m_predictionTimer = 0.0f;   // ← 追加：表示時間管理
	bool  m_isPredictionAnim = false;
	float m_predictionAnimTimer = 0.0f;
	float m_predictionHoldTime = 0.0f;
	float m_predictionScale = 1.0f;
	float m_predictionAlpha = 0.0f;
	FontRender m_fontPrediction;    // 予測テキスト用
	float m_predictedDistance = 0.0f; // 予測された飛距離(m)
	SpriteRender m_batu[3];
	bool m_isMiss[3] = { false, false, false }; // 空振りフラグ
	SpriteRender m_guruguruSprite; // 例：5段階
	FontRender    m_fontStage[10];
	const wchar_t* m_stageTextList[10] = {
	L"揺れ(弱)",   // 0段階
	L"360度揺れ(弱)",       // 1段階
	L"ランダム揺れ(弱)",         // 2段階
	L"ランダム誘導(弱)",       // 3段階
	L"流され誘導(弱)",         // 4段階
	L"流され誘導(強)",       // 5段階
	L"誘導ノイズ(強)",          // 6段階
	L"ディレイ",     // 7段階
	L"操作反転",          // 8段階
	L"ランダム誘導ノイズ(強)"   // 9段階
	};
	Vector3 m_miniMapBasePos;
	float   m_miniMapHeightY = 250.0f;
	float   m_miniMapHeightX = 271.0f;    // メーター全体の高さ（ピクセル単位）
	SpriteRender m_ballMapIcon;
	SpriteRender m_gurahu;
	SpriteRender m_kuro;
	SpriteRender m_keisuu;
	SpriteRender m_kakunin;
	float m_kakuninFlashTimer = 0.0f;
	float m_kakuninScale = 1.0f;
	bool  m_isKakuninFlash = true;   // ループON
	SpriteRender m_imagesen;
	float        m_animeTimer = 0.0f;
	SpriteRender m_kakin;
	SpriteRender m_kiroku;
};


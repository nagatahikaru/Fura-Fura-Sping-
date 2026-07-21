#pragma once
#include "Source/Source.h"
class Game;

class InGameUI :public Source
{
private:
	// === enum / 難易度 ===
	enum class UIDifficulty { Easy, Normal, Hard };
	UIDifficulty m_uiDifficulty = UIDifficulty::Normal;

	// === ポインタ ===
	Game* m_game = nullptr;
	const wchar_t* m_debuffComment = nullptr;

	// === bool ===
	bool m_isUIVisible = true;
	bool m_isFontVisible = true;
	bool m_isReplayVisible = true;
	bool m_isLeftBatter = true;
	bool m_isPaused = false;
	bool m_baisokuUI = true;
	bool m_hasPredictedBall = false;
	bool m_isBallUIFixed = false;
	bool m_isError = false;
	bool m_isErrorCheckEnabled = false;
	bool m_isBaisokuVisible = false;
	bool m_isButtonPressed = false;
	bool m_isAltUI = false;   // 交互UIフラグ
	bool m_isFadeOut = false;
	bool m_isFadeIn = false;
	bool m_isStrikeAnim = false;
	bool m_isPredictionVisible = false;
	bool m_isPredictionAnim = false;
	bool m_isKakuninFlash = true;   // ループON
	bool m_isMiss[3] = { false, false, false }; // 空振りフラグ

	// === int ===
	int m_guruGuruCount = 0;
	int m_ballCount = 1;  // 1球目〜3球目

	// === float ===
	float m_rad;
	float m_km = 0.0f;
	float batScaleX;
	float m_meetScaleX;
	float m_startZ = 0.0f;
	float m_guruGuruTimer = 0.0f;
	float m_yazirusiRot = 0.0f;
	float m_yazirusiAngleDeg = 0.0f;
	float m_buttonPressTimer = 0.0f;
	float m_uiToggleTimer = 0.0f;
	float m_ballAlpha = 0.0f;   // 透明度（0=透明, 1=完全表示）
	float m_fadeAlpha = 0.0f;
	float m_fadeSpeed = 0.5f; // 2秒で真っ黒になる例
	float m_meetCursorScale = 1.0f; // ミートカーソルの基本スケール
	float m_strikeTimer = 0.0f;
	float m_strikeHoldTime = 0.0f;   // 表示を維持する時間
	float m_predictionTimer = 0.0f;   // ← 表示時間管理
	float m_predictionAnimTimer = 0.0f;
	float m_predictionHoldTime = 0.0f;
	float m_predictionScale = 1.0f;
	float m_predictionAlpha = 0.0f;
	float m_predictedDistance = 0.0f; // 予測された飛距離(m)
	float m_miniMapHeightY = 250.0f;
	float m_miniMapHeightX = 271.0f;    // メーター全体の高さ（ピクセル単位）
	float m_kakuninFlashTimer = 0.0f;
	float m_kakuninScale = 1.0f;
	float m_animeTimer = 0.0f;

	// === Vector3 ===
	Vector3 m_batPositionRight = Vector3::Zero;
	Vector3 m_batPositionLeft = Vector3::Zero;
	Vector3 m_meetPositionRight = Vector3::Zero;
	Vector3 m_meetPositionLeft = Vector3::Zero;
	Vector3 m_meetPos;
	Vector3 m_batPos;
	Vector3 m_offset;
	Vector3 m_predictedBallPos3D = Vector3::Zero;
	Vector3 m_fixedBallUIPos = Vector3::Zero;
	Vector3 m_miniMapBasePos;

	// === Quaternion ===
	Quaternion m_batRotation = Quaternion::Identity;
	Quaternion m_yazirusiRotation;

	// === FontRender ===
	FontRender m_fontRender;
	FontRender m_fontBollRender1;
	FontRender m_fontBollRender2;
	FontRender m_fontBollRender3;
	FontRender m_debugFont;
	FontRender m_debugFont2;
	FontRender m_Count;
	FontRender m_fontBallCount; // 新しいフォント
	FontRender m_fontDebug1;
	FontRender m_fontDebug2;
	FontRender m_fontDebug3;
	FontRender m_fontDebug4;
	FontRender m_fontDebug5;
	FontRender m_fontPrediction;    // 予測テキスト用
	FontRender m_fontStage;         // デバフ段階表示用（最大15段階）

	// === ModelRender ===
	ModelRender m_wakuModel;

	// === SpriteRender ===
	SpriteRender m_spriteRenderBat;
	SpriteRender m_spriteRenderMeet;
	SpriteRender m_spriteRenderReplay;
	SpriteRender m_spriteRenderBall;
	SpriteRender m_kiiro1;
	SpriteRender m_kiiro2;
	SpriteRender m_kiiro3;
	SpriteRender m_besu;
	SpriteRender m_baisoku;
	SpriteRender m_shuchusen;
	SpriteRender m_yazirusi;
	SpriteRender m_konto;
	SpriteRender m_mawase;
	SpriteRender m_taimingu;
	SpriteRender m_Abotan;
	SpriteRender m_Abotan2;
	SpriteRender m_gizagiza;
	SpriteRender m_spritekuro;
	SpriteRender m_bbb;
	SpriteRender m_bsuki;
	SpriteRender m_strikeSprite;
	SpriteRender m_niceSprite;
	SpriteRender m_greatSprite;
	SpriteRender m_excellentSprite;
	SpriteRender m_perfectSprite;
	SpriteRender m_guruguruSprite; // 例：5段階
	SpriteRender m_ballMapIcon;
	SpriteRender m_gurahu;
	SpriteRender m_kuro;
	SpriteRender m_keisuu;
	SpriteRender m_kakunin;
	SpriteRender m_imagesen;
	SpriteRender m_kakin;
	SpriteRender m_kiroku;
	SpriteRender m_easySprite;
	SpriteRender m_normalSprite;
	SpriteRender m_hardSprite;
	SpriteRender m_guruE;
	SpriteRender m_guruN;

	// === SpriteRender (配列) ===
	SpriteRender m_ballIcon[3];
	SpriteRender m_batu[3];

public:
	enum PredictionType {
		Prediction_None,
		Prediction_Nice,
		Prediction_Great,
		Prediction_Excellent,
		Prediction_Perfect,
		Prediction_Goro
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
	void SetGameInstance(Game* game) { m_game = game; }	
	void SetDebuffComment(const wchar_t* comment)
	{
		m_debuffComment = comment;
	}

	
	// ----------------------------------------
	// 便利関数
	// ----------------------------------------
	/// <summary>
	/// floatを小数点第二位まで四捨五入する
	/// </summary>
	/// <param name="value">四捨五入するfloat値。</param>
	///　<returns>四捨五入されたfloat値。</returns>
	float RoundToInt(float value) {
		if (value >= 0.0f) {
			// 100倍して四捨五入し、100.0fで割る
			return static_cast<int>(value * 100.0f + 0.5f) / 100.0f;
		}
		else {
			// 負の数の場合は-0.5fする
			return static_cast<int>(value * 100.0f - 0.5f) / 100.0f;
		}
	}

};

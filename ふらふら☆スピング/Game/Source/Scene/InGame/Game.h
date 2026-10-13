#pragma once
#include "Source/Source.h"
#include "Source/Actor/Character/Ball/Ball.h"
#include "Source/Difficulty.h"
#include "Source/Weather/Weather.h"

class GameCamera;
class Background;
class InGameUI;
class Batter;
class Pitcher;
class Catcher;
class Start1;

enum CameraMode
{
	Camera_Catcher,
	Camera_Replay,
	Camera_Ball,
	Camera_BackBall,
	Camera_Kakutei
};

class Game : public Source
{
public:
	Game() {}
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//==============================================================
	// 進行
	//==============================================================
	void OnBallLanded();                // ボールが着地した
	void OnPitcherThrow();              // ピッチャーが投げた
	void OnOver100m();                  // 100m超えの演出
	void ResetForNextShot();            // 次の球の準備
	void StartEndFade();                // 終了フェード開始
	void GoToResult();                  // リザルトへ
	void GoToTiter();                   // タイトルへ戻る
	void StartHitGlance(float duration);// 打撃の瞬間の注視

	//==============================================================
	// リプレイ
	//==============================================================
	void StartReplay(int index);
	void DecideBestReplay();
	void StartReplayRecording();
	int  GetReplayFrameCount() const { return m_replayFrameCounter; }

	bool IsRecording() const { return m_isRecording; }
	bool GetIsReplayPlaying() const { return m_isReplayPlaying; }
	bool IsReplayPlaying() const { return m_isReplayPlaying; }
	void SetIsReplayPlaying(bool isReplayPlaying) { m_isReplayPlaying = isReplayPlaying; }

	float GetReplayDelayTimer() const { return m_replayDelayTimer; }
	void SetReplayDelayTimer(float timer) { m_replayDelayTimer = timer; }

	void SetReplaySwingDelayFrames(int frames) { m_replaySwingDelayFrames = frames; }
	int GetReplaySwingDelayFrames() const { return m_replaySwingDelayFrames; }

	// ボール録画フレーム数（スイング記録用）
	int GetCurrentReplayRecordFrame() const { return (int)m_ball->GetReplayPath().size(); }

	//==============================================================
	// 状態の取得・設定
	//==============================================================
	// ポーズ・準備フェーズ
	bool GetIsPaused() const { return m_isPaused; }
	void SetIsPaused(bool isPaused) { m_isPaused = isPaused; }
	bool GetIsReadyPhase() const { return m_isReadyPhase; }
	void SetIsReadyPhase(bool isReadyPhase) { m_isReadyPhase = isReadyPhase; }
	float GetReadyTimer() const { return m_readyTimer; }
	void SetReadyTimer(float readyTimer) { m_readyTimer = readyTimer; }

	// ヒットストップ
	bool GetIsHitStop() const { return m_isHitStop; }
	void SetIsHitStop(bool isHitStop) { m_isHitStop = isHitStop; }
	float GetHitStopTimer() const { return m_hitStopTimer; }
	void SetHitStopTimer(float hitStopTimer) { m_hitStopTimer = hitStopTimer; }

	// 確定演出
	bool GetIsKakutei() const { return m_isKakutei; }
	void SetIsKakutei(bool isKakutei) { m_isKakutei = isKakutei; }
	float GetKakuteiTimer() const { return m_kakuteiTimer; }
	void SetKakuteiTimer(float kakuteiTimer) { m_kakuteiTimer = kakuteiTimer; }

	// 時間・入力
	float GetTimeScale() const { return m_timeScale; }
	void SetTimeScale(float timeScale) { m_timeScale = timeScale; }
	bool GetCanFastForward() const { return m_canFastForward; }
	void SetCanFastForward(bool canFastForward) { m_canFastForward = canFastForward; }
	bool GetIsInputLocked() const { return m_isInputLocked; }
	void SetIsInputLocked(bool isInputLocked) { m_isInputLocked = isInputLocked; }

	// 100m・ホームラン・飛距離
	bool GetHasTriggered100m() const { return m_hasTriggered100m; }
	void SetHasTriggered100m(bool hasTriggered100m) { m_hasTriggered100m = hasTriggered100m; }
	bool GetIsHomeRun() const { return m_isHomeRun; }
	void SetIsHomeRun(bool isHomeRun) { m_isHomeRun = isHomeRun; }
	float GetHitStartZ() const { return m_hitStartZ; }
	void SetHitStartZ(float hitStartZ) { m_hitStartZ = hitStartZ; }
	bool GetHasStartedDistance() const { return m_hasStartedDistance; }
	void SetHasStartedDistance(bool hasStartedDistance) { m_hasStartedDistance = hasStartedDistance; }
	void SetKmValue(float km) { m_km = km; }

	// ゲーム開始・着地
	void SetGameStarted(bool flag) { m_isGameStarted = flag; }
	bool IsGameStarted() const { return m_isGameStarted; }
	bool IsBallLanded() const { return m_isBallLanded; }

	// ショット数
	int GetShots() const { return m_shots; }
	int GetCurrentShotIndex() const { return m_shots; }
	void SetShots(int shots) { m_shots = shots; }

	// ベストショット（リプレイ対象）。無いときは -1
	int GetBestShotIndex() const { return m_bestShotIndex; }

	// 難易度・天候・風
	void SetDifficulty(Difficulty diff) { m_difficulty = diff; }
	Difficulty GetDifficulty() const { return m_difficulty; }
	void SetWeatherType(WeatherType type) { m_weatherType = type; }
	WeatherType GetWeatherType() const { return m_weatherType; }
	WindType GetWindType() const { return m_weather ? m_weather->GetWindType() : Wind_None; }
	bool IsWindActive() const { return m_weather ? m_weather->IsWindActive() : false; }

	// ぐるぐる
	int GetGuruguru() const { return m_guruguru; }
	void SetGuruGuru(int count) { m_guruguru = count; }
	int GetGuruguruSEInterval() const;
	bool GetRotationSeen() const { return m_isRotationSeen; }
	void SetRotationSeen(bool isRotationSeen) { m_isRotationSeen = isRotationSeen; }

	// チュートリアル
	bool GetShouldContinueTutorial() const { return m_shouldContinueTutorial; }
	bool GetIsTutorialSelectTitle() const { return m_isTutorialSelectTitle; }

	// カメラ・UI
	GameCamera* GetGameCamera() const { return m_gameCamera; }
	InGameUI* GetInGameUI() const { return m_InGameUI; }
	CameraMode GetCameraMode() const { return m_cameraMode; }
	void SetCameraMode(CameraMode mode) { m_cameraMode = mode; }
	int GetCameraType() const { return m_cameraType; }

	//==============================================================
	// 球ごとのデータ（範囲外のインデックスは安全な値を返す）
	//==============================================================
	int GetHitFrame(int shot) const { return IsValidShot(shot) ? m_hitFrame[shot] : -1; }
	void SetHitFrame(int shot, int frame) { if (IsValidShot(shot)) m_hitFrame[shot] = frame; }

	const Vector3& GetHitVelocity(int shot) const
	{
		static const Vector3 zero = Vector3::Zero;
		return IsValidShot(shot) ? m_hitVelocities[shot] : zero;
	}
	void SetHitVelocity(int shot, const Vector3& velocity) { if (IsValidShot(shot)) m_hitVelocities[shot] = velocity; }

	const Vector3& GetHitDirection(int shot) const
	{
		static const Vector3 zero = Vector3::Zero;
		return IsValidShot(shot) ? m_hitDirections[shot] : zero;
	}
	void SetHitDirection(int shot, const Vector3& direction) { if (IsValidShot(shot)) m_hitDirections[shot] = direction; }

	const Vector3& GetHitStartPos(int shot) const
	{
		static const Vector3 zero = Vector3::Zero;
		return IsValidShot(shot) ? m_hitStartPos[shot] : zero;
	}
	void SetHitStartPos(int shot, const Vector3& pos) { if (IsValidShot(shot)) m_hitStartPos[shot] = pos; }

	const float& GetHitPower(int shot) const
	{
		static const float zero = 0.0f;
		return IsValidShot(shot) ? m_hitPower[shot] : zero;
	}
	void SetHitPower(int shot, float power) { if (IsValidShot(shot)) m_hitPower[shot] = power; }

	int GetPitchFrame(int shot) const { return IsValidShot(shot) ? m_pitchFrame[shot] : -1; }
	void SetPitchFrame(int shot, int frame) { if (IsValidShot(shot)) m_pitchFrame[shot] = frame; }

	// 打った瞬間に予測した飛距離（リプレイの軌跡エフェクトの判定に使う）
	float GetPredictedDistance(int shot) const { return IsValidShot(shot) ? m_predictedDistance[shot] : 0.0f; }
	void SetPredictedDistance(int shot, float distance) { if (IsValidShot(shot)) m_predictedDistance[shot] = distance; }

	void SetSwingFrame(int shot, int frame) { if (IsValidShot(shot)) m_swingFrame[shot] = frame; }
	void SetHasSwung(int shot, bool swung) { if (IsValidShot(shot)) m_hasSwung[shot] = swung; }
	bool GetHasSwung(int shot) const { return IsValidShot(shot) ? m_hasSwung[shot] : false; }

	void SetIsMagicBallShot(int shot, bool isMagic) { if (IsValidShot(shot)) m_isMagicBallShot[shot] = isMagic; }
	bool GetIsMagicBallShot(int shot) const { return IsValidShot(shot) ? m_isMagicBallShot[shot] : false; }

	// リプレイフレーム構造体
	struct ReplayFrame {
		Vector3 ballPos;			// そのフレームでのボール位置
		Vector3 ballVel;			// そのフレームでのボール速度
		float batterAnimTime;		// バッターのアニメーション再生時間
		float pitcherAnimTime;		// ピッチャーのアニメーション再生時間
		bool swingTriggered;		// Aボタンを押した瞬間かどうか
	};
	std::vector<ReplayFrame> GetReplayFrames(int shot) const
	{
		return IsValidShot(shot) ? m_replayFrames[shot] : std::vector<ReplayFrame>();
	}
	void SetReplayFrames(int shot, const std::vector<ReplayFrame>& frames)
	{
		if (IsValidShot(shot)) m_replayFrames[shot] = frames;
	}

	bool m_hasStartedGuruIntroCamera = false;

private:
	// 難易度によって最大5球（Tutorial）まであるため、全ショット配列はこのサイズに統一する
	static constexpr int MAX_SHOTS = 5;
	static bool IsValidShot(int shot) { return shot >= 0 && shot < MAX_SHOTS; }

	//==============================================================
	// Update の内訳。bool を返すものは false なら Update をそこで終える
	//==============================================================
	void UpdateGuruguruSE();            // ぐるぐるの回数に応じたSE
	void UpdateGuruGuruIntroCamera();   // ぐるぐる旋回の開始・終了でカメラ切り替え
	void UpdateCountdown();             // カウントダウン中の更新
	void OpenPause();                   // ポーズ画面を開く
	void UpdateReadyPhase();            // 準備フェーズ
	void UpdateTutorialSelect();        // チュートリアル終了後の選択待ち
	void RestartTutorial();             // チュートリアルを最初からやり直す
	void UpdateHitStop();               // ヒットストップ
	bool UpdateKakutei();               // パーフェクト確定演出
	void UpdateTimeScale();             // 倍速
	bool UpdateHitGlance();             // 打撃の瞬間の注視
	void UpdateCameraAndUI();           // カメラとUIの表示切り替え
	bool UpdateAfterLanding();          // 着地後の待ち
	void UpdateLiveDistance();          // 飛行中の飛距離表示
	bool UpdateFadeInDelay();           // フェードイン遅延
	void UpdateReplay();                // リプレイ再生
	void EndReplay();                   // リプレイを終えてリザルトへ
	void SetUIVisibility(bool ui, bool font, bool replay, bool baisoku);

	//==============================================================
	// 主要オブジェクト参照
	//==============================================================
	GameCamera* m_gameCamera = nullptr;	// ゲームカメラ
	Background* m_background = nullptr;	// 背景
	Batter* m_batter = nullptr;			// バッター
	Pitcher* m_pitcher = nullptr;		// ピッチャー
	Catcher* m_catcher = nullptr;		// キャッチャー
	Ball* m_ball = nullptr;				// ボール
	SkyCube* m_skyCube = nullptr;		// スカイキューブ
	InGameUI* m_InGameUI = nullptr;		// インゲームUI
	Weather* m_weather = nullptr;

	//==============================================================
	// カメラ・進行状態
	//==============================================================
	CameraMode m_cameraMode = Camera_Catcher;	// 現在のカメラモード
	int m_cameraType = 0;						// カメラの種別
	Difficulty m_difficulty = Difficulty::Easy;
	WeatherType m_weatherType = WeatherType::Sunny;
	bool m_isGameStarted = false;				// ゲーム本編が開始したか
	bool m_isPaused = false;
	int m_maxShots = 3;							// 難易度によって変わる球数（Start で上書き）
	int m_shots = 0;							// 現在の球数カウンタ

	//==============================================================
	// ぐるぐる
	//==============================================================
	int m_guruguru = 0;
	int m_prevGuruGuru = 0;						// 直前フレームの値（変化検知用）
	bool m_isRotationSeen = true;

	//==============================================================
	// 飛距離・着地
	//==============================================================
	float m_km = 0.0f;							// 表示用の飛距離
	float m_hitStartZ = 0.0f;					// ヒット開始時のZ座標（飛距離計測基準）
	bool m_hasStartedDistance = false;
	bool m_isHomeRun = false;
	bool m_hasTriggered100m = false;
	bool m_isBallLanded = false;
	float m_afterLandingTimer = 0.0f;			// 着地後の経過時間
	int m_scores[MAX_SHOTS] = {};				// 各球のスコア

	//==============================================================
	// 球ごとの打撃データ
	//==============================================================
	int     m_hitFrame[MAX_SHOTS] = {};
	Vector3 m_hitVelocities[MAX_SHOTS];
	Vector3 m_hitDirections[MAX_SHOTS];
	Vector3 m_hitStartPos[MAX_SHOTS];
	float   m_hitPower[MAX_SHOTS] = {};
	float   m_predictedDistance[MAX_SHOTS] = {};
	int     m_swingFrame[MAX_SHOTS] = { -1, -1, -1, -1, -1 };
	int     m_pitchFrame[MAX_SHOTS] = {};
	bool    m_hasSwung[MAX_SHOTS] = {};
	bool    m_isMagicBallShot[MAX_SHOTS] = {};

	//==============================================================
	// リプレイ
	//==============================================================
	std::vector<std::vector<Vector3>> m_replayPaths;	// 各球のボール軌道（Start で resize）
	std::vector<Vector3> m_currentReplay;				// 再生中の軌道
	std::vector<ReplayFrame> m_replayFrames[MAX_SHOTS];
	bool  m_isReplayPlaying = false;
	bool  m_isRecording = false;
	bool  m_shouldStartReplay = false;
	int   m_bestShotIndex = -1;
	int   m_bestSwingFrame = 0;
	int   m_replayFrameCounter = 0;
	int   m_replayStartFrame = 0;						// 再生中のフレーム番号
	int   m_replayPitchFrame = 0;
	int   m_replaySwingDelayFrames = 0;
	float m_replayTimer = 0.0f;
	float m_replayDuration = 5.0f;						// 最大再生時間（Update で難易度から更新）
	float m_replayDelayTimer = 0.0f;
	bool  m_hasPlayedReplaySwing = false;
	bool  m_hasStartedReplayZoom = false;
	bool  m_hasReachedPitchFrame = false;
	float m_replayZoomDelayTimer = 0.0f;				// 投球フレーム到達後の経過時間
	float m_replayZoomDelaySeconds = 0.0f;				// 何秒待ってからズームを始めるか

	//==============================================================
	// 準備フェーズ・演出
	//==============================================================
	float m_readyTimer = 5.0f;
	bool  m_isReadyPhase = false;
	bool  m_isKakutei = false;
	float m_kakuteiTimer = 0.0f;
	float m_timeScale = 1.0f;
	bool  m_isInputLocked = false;
	bool  m_canFastForward = false;
	float m_hitStopTimer = 0.0f;
	bool  m_isHitStop = false;
	float m_hitGlanceTimer = 0.0f;
	bool  m_isHitGlancing = false;
	float m_fadeInDelayTimer = -1.0f;					// -1 は未使用
	bool  m_startFadeSE2 = false;

	//==============================================================
	// チュートリアル
	//==============================================================
	bool m_shouldContinueTutorial = false;
	bool m_isTutorialSelectTitle = false;				// false=続ける, true=タイトルへ戻る
	bool m_tutorialStickNeutral = true;					// スティックが中央に戻ったか（連続入力防止）
};
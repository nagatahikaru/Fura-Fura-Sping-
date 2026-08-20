#pragma once
#include "Source/Source.h"
#include "Source/Actor/Character/Ball/Ball.h"
#include "Source/Difficulty.h"
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

enum WindType
{
	Wind_None,
	Wind_LeftToRight,   // 左から右への風
	Wind_RightToLeft,   // 右から左への風
	Wind_Tailwind,      // 追い風
	Wind_Headwind,       // 向かい風
};

class Game : public Source
{
private:
	// ★ 難易度によって最大5球（Tutorial）まであるため、全ショット配列はこのサイズに統一する
	static const int MAX_SHOTS = 5;

	// === 主要オブジェクト参照 ===
	GameCamera* m_gameCamera;	//ゲームカメラ。
	Background* m_background;	//背景。
	Batter* m_batter;			//バッター。
	Pitcher* m_pitcher;			//ピッチャー。
	Catcher* m_catcher;			//キャッチャー。
	Ball* m_ball;				//ボール。
	SkyCube* m_skyCube;			//スカイキューブ。
	InGameUI* m_InGameUI;		//インゲームUI。
	Start1* m_start1;			// タイトル/開始演出オブジェクト

	// === カメラ・進行状態 ===
	CameraMode m_cameraMode = Camera_Catcher;	// 現在のカメラモード（初期はキャッチャー視点）
	int m_cameraType = 0;						// カメラの種別切り替え用フラグ

	// === 打撃/演出系カウンタ ===
	int m_guruguru = 0;			// デバフ「グルグル」の現在段階
	int m_prevGuruGuru = 0;		// 直前フレームのグルグル段階（変化検知用）
	float m_km = 0;				// 表示用の球速(km/h)

	// === BGM/開始フラグ ===
	bool m_bgmStarted = false;			// BGMを再生開始したかどうか
	bool m_isGameStarted = false;		// ゲーム本編が開始したかどうか

	// === 着弾関連 ===
	float m_afterLandingTimer = 0.0f;	// ボール着弾後の経過時間
	bool  m_isBallLanded = false;		// ボールが着弾したかどうか
	float m_zeroDistanceTimer = 0.0f;	// 飛距離0（空振り/ファウル等）判定用タイマー

	bool m_isRotationSeen = true;		// 回転（グルグル演出）を見せたかどうか

	// === スコア管理 ===
	int m_scores[MAX_SHOTS] = { 0,0,0,0,0 };	// 各回（最大5球分）のスコア保存

	// === ミス演出待機 ===
	bool  m_isMissWait = false;		// 空振り後の待機演出中かどうか
	float m_missWaitTimer = 0.0f;		// 空振り待機の経過時間
	float m_fadeInDelayTimer = -1.0f;	// フェードイン開始までの遅延タイマー（-1は未使用状態）

	// === リプレイ軌道データ ===
	std::vector<std::vector<Vector3>> m_replayPaths;	// 各球分のボール軌道データ（Start()でMAX_SHOTS分resize）

	// === リプレイ再生制御 ===
	float m_replayTimer = 0.0f;			// リプレイ再生の経過時間
	float m_replaySpeed = 1.0f;			// リプレイ再生速度（1.0f固定でOK）
	int   m_bestShotIndex = -1;			// ベストショット（リプレイ対象）のインデックス
	std::vector<Vector3> m_currentReplay;	// 現在再生中の軌道データ
	bool m_shouldStartReplay = false;		// リプレイを開始すべきかどうかのフラグ
	float m_replayDuration = 5.0f;			// リプレイの最大再生時間（3秒で打ち切り、とコメントあり）
	Vector3 m_initialVelocities[MAX_SHOTS];		// 各球投球直後の初速度
	int m_replayStartFrame = 0;			// リプレイ開始フレーム番号
	int m_replayPitchFrame = 0;			// リプレイ中の投球フレーム番号
	bool m_isRecording = false;			// リプレイ用に軌道を記録中かどうか
	int m_replayDelayFrames = 0;			// リプレイ開始までの遅延フレーム数
	float m_replayAccumulator = 0.0f;		// 再生速度制御用のアキュムレータ
	float m_replaySwingTimer = 0.0f;		// リプレイ中のスイング演出タイマー
	bool m_hasPlayedReplaySwing = false;	// リプレイ中にスイングを1回だけ再生するためのフラグ
	int m_bestSwingFrame = 0;				// ベストショットのスイングフレーム番号
	int m_bestPitchFrame = 0;				// ベストショットの投球フレーム番号
	float m_replaySkipHoldTime = 0.0f;		// リプレイスキップ用のBボタン長押し時間
	bool m_hasAppliedHitMoment = false;	// ヒット瞬間の演出（ヒットストップ等）を適用済みかどうか
	bool m_startFadeSE2 = false;			// フェード用SE2を再生開始したかどうか
	bool m_hasSwung[MAX_SHOTS] = { false, false, false, false, false };	// 各球でスイングしたかどうか

	// === 難易度 ===
	Difficulty m_difficulty = Difficulty::Easy;	// 現在の難易度設定

	// === リプレイ用フレーム記録 ===
	int m_replayFrameCounter = 0;			// リプレイ記録用フレームカウンタ
	int m_swingFrame[MAX_SHOTS] = { -1, -1, -1, -1, -1 };	// 各球のスイング発生フレーム番号
	int m_pitchFrame[MAX_SHOTS] = { 0,0,0,0,0 };		// 各球の投球発生フレーム番号
	bool  m_isReplayPlaying = false;		// リプレイ再生中かどうか

	// === 打球データ（各球ごと） ===
	int     m_hitFrame[MAX_SHOTS];			// 各球のヒット発生フレーム番号
	Vector3 m_hitVelocities[MAX_SHOTS];	// 各球のヒット時の速度ベクトル
	Vector3 m_hitDirections[MAX_SHOTS];	// 各球のヒット時の方向ベクトル
	Vector3 m_hitStartPos[MAX_SHOTS];		// 各球のヒット開始位置
	float   m_hitPower[MAX_SHOTS];			// 各球のヒットパワー

	// === ショット進行管理 ===
	int m_shots = 0;				// 何回打ったか（現在の球数カウンタ）
	float m_hitStartZ = 0.0f;		// ヒット開始時のZ座標（飛距離計測基準）
	bool m_hasStartedDistance = false;	// 飛距離計測を開始したかどうか
	bool m_isHomeRun = false;		// ホームランになったかどうか

	// === リプレイ開始遅延・準備フェーズ ===
	float m_replayDelayTimer = 0.0f;	// リプレイ開始までの遅延タイマー
	float m_readyTimer = 5.0f;			// 準備フェーズの残り時間
	bool m_isReadyPhase = false;		// 準備フェーズ中かどうか

	// === 確定演出 ===
	bool m_isKakutei = false;		// 確定演出中かどうか
	float m_kakuteiTimer = 0.0f;	// 確定演出の経過時間

	// === 時間制御・入力制御 ===
	float m_timeScale = 1.0f;			// ゲーム内時間の進行倍率（スローモーション等）
	bool m_isInputLocked = false;		// 入力をロックしているかどうか
	bool m_canFastForward = false;		// 早送り可能かどうか

	// === ヒットストップ ===
	float m_hitStopTimer = 0.0f;	// ヒットストップの経過時間
	bool m_isHitStop = false;		// ヒットストップ中かどうか

	bool m_hasTriggered100m = false;			// 100m到達演出をすでに発火したかどうか
	bool m_shouldContinueTutorial = false;		// チュートリアルを続けるかどうか
	bool m_isTutorialSelectTitle = false;       // ★ 追加：false=続ける選択中, true=タイトルへ戻る選択中
	bool m_tutorialStickNeutral = true;         // ★ 追加：スティックがニュートラルに戻ったか（連続入力防止）

	// === リプレイフレーム構造体 ===
	struct ReplayFrame {
		Vector3 ballPos;			// そのフレームでのボール位置
		Vector3 ballVel;			// そのフレームでのボール速度

		float batterAnimTime;		// バッターのアニメーション再生時間
		float pitcherAnimTime;		// ピッチャーのアニメーション再生時間

		bool swingTriggered;		// Aボタンを押した瞬間かどうか
	};
	std::vector<ReplayFrame> m_replayFrames[MAX_SHOTS];	// 各球分のリプレイフレーム記録

	// === ポーズ・スイング遅延 ===
	bool m_isPaused = false;				// ゲームが一時停止中かどうか
	int m_replaySwingDelayFrames = 0;		// リプレイ中のスイング再生遅延フレーム数

	// === 難易度依存の球数 ===
	int m_maxShots = 3;	// 難易度によって変わる球数（デフォルト3球、実行時はGetDifficultyParams()で上書き）

	bool m_isMagicBallShot[MAX_SHOTS] = { false, false, false, false, false };
	float m_hitGlanceTimer = 0.0f;
	bool  m_isHitGlancing = false;
	bool m_hasStartedReplayZoom = false;
	float m_replayZoomDelayTimer = 0.0f;   // ★ 投球フレーム到達後の経過時間
	float m_replayZoomDelaySeconds = 0.0f; // ★ 何秒待ってからズーム開始するか
	bool  m_hasReachedPitchFrame = false;  // ★ 投球フレームに到達済みか

	// === エフェクト ===
	uint32_t m_rainEffectId = 0; 
	uint32_t m_kazeEffectId = 0;
	uint32_t m_kaze2EffectId = 0;
	uint32_t m_kaze3EffectId = 0;
	uint32_t m_kaze4EffectId = 0;
	WindType m_currentWindType = Wind_None;   
	bool m_isWindActive = false;       
	bool m_isRainyFromLoad = false;
	SoundSource* m_rainSE = nullptr;
	SoundSource* m_kazeSE = nullptr;
public:
	Game() {}
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	InGameUI* GetInGameUI() const { return m_InGameUI; }
	void SetCameraMode(CameraMode mode) { m_cameraMode = mode; }
	void SetKmValue(float km) { m_km = km; }
	void StartReplay(int index);
	void DecideBestReplay();
	void GoToResult();
	void OnBallLanded();
	void OnPitcherThrow();
	bool GetIsPaused()
	{
		return m_isPaused;
	}
	void SetIsPaused(bool isPaused)
	{
		m_isPaused = isPaused;
	}
	bool GetIsReadyPhase() const
	{
		return m_isReadyPhase;
	}
	void SetIsReadyPhase(bool isReadyPhase)
	{
		m_isReadyPhase = isReadyPhase;
	}
	float GetReadyTimer() const
	{
		return m_readyTimer;
	}
	void SetReadyTimer(float readyTimer)
	{
		m_readyTimer = readyTimer;
	}
	bool GetIsHitStop() const
	{
		return m_isHitStop;
	}
	void SetIsHitStop(bool isHitStop)
	{
		m_isHitStop = isHitStop;
	}
	float GetHitStopTimer() const
	{
		return m_hitStopTimer;
	}
	void SetHitStopTimer(float hitStopTimer)
	{
		m_hitStopTimer = hitStopTimer;
	}
	bool GetCanFastForward() const
	{
		return m_canFastForward;
	}
	void SetCanFastForward(bool canFastForward)
	{
		m_canFastForward = canFastForward;
	}
	bool GetIsKakutei() const
	{
		return m_isKakutei;
	}
	void SetIsKakutei(bool isKakutei)
	{
		m_isKakutei = isKakutei;
	}
	bool GetIsReplayPlaying() const
	{
		return m_isReplayPlaying;
	}
	void SetIsReplayPlaying(bool isReplayPlaying)
	{
		m_isReplayPlaying = isReplayPlaying;
	}
	bool GetIsInputLocked() const
	{
		return m_isInputLocked;
	}
	void SetIsInputLocked(bool isInputLocked)
	{
		m_isInputLocked = isInputLocked;
	}
	bool GetHasTriggered100m() const
	{
		return m_hasTriggered100m;
	}
	void SetHasTriggered100m(bool hasTriggered100m)
	{
		m_hasTriggered100m = hasTriggered100m;
	}
	bool GetIsHomeRun() const
	{
		return m_isHomeRun;
	}
	void SetIsHomeRun(bool isHomeRun)
	{
		m_isHomeRun = isHomeRun;
	}
	float GetHitStartZ() const
	{
		return m_hitStartZ;
	}
	void SetHitStartZ(float hitStartZ)
	{
		m_hitStartZ = hitStartZ;
	}
	void SetShots(int shots)
	{
		m_shots = shots;
	}
	bool GetHasStartedDistance() const
	{
		return m_hasStartedDistance;
	}
	void SetHasStartedDistance(bool hasStartedDistance)
	{
		m_hasStartedDistance = hasStartedDistance;
	}

	int GetHitFrame(int shotIndex) const
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			return m_hitFrame[shotIndex];
		}
		return -1; // 無効なインデックスの場合は-1を返す
	}
	void SetHitFrame(int shotIndex, int frame)
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			m_hitFrame[shotIndex] = frame;
		}
	}

	const Vector3& GetHitVelocity(int shotIndex) const
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			return m_hitVelocities[shotIndex];
		}
		static Vector3 zeroVelocity = Vector3::Zero;
		return zeroVelocity; // 無効なインデックスの場合はゼロベクトルを返す
	}
	void SetHitVelocity(int shotIndex, const Vector3& velocity)
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			m_hitVelocities[shotIndex] = velocity;
		}
	}
	const Vector3& GetHitDirection(int shotIndex) const
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			return m_hitDirections[shotIndex];
		}
		static Vector3 zeroDirection = Vector3::Zero;
		return zeroDirection; // 無効なインデックスの場合はゼロベクトルを返す
	}
	void SetHitDirection(int shotIndex, const Vector3& direction)
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			m_hitDirections[shotIndex] = direction;
		}
	}

	const Vector3& GetHitStartPos(int shotIndex) const
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			return m_hitStartPos[shotIndex];
		}
		static Vector3 zeroPos = Vector3::Zero;
		return zeroPos; // 無効なインデックスの場合はゼロベクトルを返す
	}
	void SetHitStartPos(int shotIndex, const Vector3& pos)
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			m_hitStartPos[shotIndex] = pos;
		}
	}

	const float& GetHitPower(int shotIndex) const
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			return m_hitPower[shotIndex];
		}
		static float zeroPower = 0.0f;
		return zeroPower; // 無効なインデックスの場合は0を返す
	}
	void SetHitPower(int shotIndex, float power)
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			m_hitPower[shotIndex] = power;
		}
	}
	float GetKakuteiTimer() const
	{
		return m_kakuteiTimer;
	}
	void SetKakuteiTimer(float kakuteiTimer)
	{
		m_kakuteiTimer = kakuteiTimer;
	}

	float GetTimeScale()
	{
		return m_timeScale;
	}
	void SetTimeScale(float timeScale)
	{
		m_timeScale = timeScale;
	}
	float GetReplayDelayTimer() const
	{
		return m_replayDelayTimer;
	}
	void SetReplayDelayTimer(float timer)
	{
		m_replayDelayTimer = timer;
	}
	std::vector<ReplayFrame> GetReplayFrames(int shotIndex) const
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			return m_replayFrames[shotIndex];
		}
		return std::vector<ReplayFrame>(); // 無効なインデックスの場合は空のベクトルを返す
	}

	void SetReplayFrames(int shotIndex, const std::vector<ReplayFrame>& frames)
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			m_replayFrames[shotIndex] = frames;
		}
	}

	int GetPitchFrame(int shotIndex) const
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			return m_pitchFrame[shotIndex];
		}
		return -1; // 無効なインデックスの場合は-1を返す
	}
	void SetPitchFrame(int shotIndex, int frame)
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			m_pitchFrame[shotIndex] = frame;
		}
	}
	GameCamera* GetGameCamera() const { return m_gameCamera; }
	CameraMode GetCameraMode() const { return m_cameraMode; }
	void SetDifficulty(Difficulty diff) { m_difficulty = diff; }
	Difficulty GetDifficulty() const { return m_difficulty; }
	int GetGuruguru()const {
		return  m_guruguru;
	}
	void SetGuruGuru(int count)
	{
		m_guruguru = count;
	}

	int GetGuruguruSEInterval() const;

	bool GetRotationSeen() const
	{
		return m_isRotationSeen;
	}
	void SetRotationSeen(bool isRotationSeen)
	{
		m_isRotationSeen = isRotationSeen;
	}

	void SetGameStarted(bool flag)
	{
		m_isGameStarted = flag;
	}

	bool IsGameStarted() const
	{
		return m_isGameStarted;
	}

	int GetCameraType() const
	{
		return m_cameraType;
	}
	float GetTimeScale() const { return m_timeScale; }
	void ResetForNextShot();

	void OnOver100m();
	void StartEndFade();
	bool IsBallLanded() const { return m_isBallLanded; }

	bool IsReplayPlaying() const { return m_isReplayPlaying; }

	// ★ 今が何球目か返す
	int GetCurrentShotIndex() const { return m_shots; }

	// ★ ボール録画フレーム数を返す（スイング記録用）
	int GetCurrentReplayRecordFrame() const {
		return (int)m_ball->GetReplayPath().size();
	}
	void SetSwingFrame(int shotIndex, int frame)
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			m_swingFrame[shotIndex] = frame;
		}
	}
	void SetHasSwung(int shotIndex, bool swung)
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			m_hasSwung[shotIndex] = swung;
		}
	}
	bool GetHasSwung(int shotIndex) const
	{
		if (shotIndex >= 0 && shotIndex < MAX_SHOTS) {
			return m_hasSwung[shotIndex];
		}
		return false;
	}
	int GetReplayFrameCount() const;
	void StartReplayRecording();
	int GetShots() const { return m_shots; }
	bool IsRecording() const { return m_isRecording; }
	void SetReplaySwingDelayFrames(int frames) { m_replaySwingDelayFrames = frames; }
	int GetReplaySwingDelayFrames() const { return m_replaySwingDelayFrames; }
	bool GetShouldContinueTutorial() const { return m_shouldContinueTutorial; }
	void GoToTiter();          // ★ 追加：タイトルへ戻る
	bool GetIsTutorialSelectTitle() const { return m_isTutorialSelectTitle; } // ★ 追加
	void SetIsMagicBallShot(int shot, bool isMagic) {
		if (shot >= 0 && shot < MAX_SHOTS) m_isMagicBallShot[shot] = isMagic;
	}
	bool GetIsMagicBallShot(int shot) const {
		return (shot >= 0 && shot < MAX_SHOTS) ? m_isMagicBallShot[shot] : false;
	}
	void StartHitGlance(float duration);

	WindType GetCurrentWindType() const { return m_currentWindType; }
	bool GetIsWindActive() const { return m_isWindActive; }
	void SetIsRainyFromLoad(bool isRainy) { m_isRainyFromLoad = isRainy; }
	bool GetIsRainy() const { return m_isRainyFromLoad; }
	bool m_hasStartedGuruIntroCamera = false;
};
#pragma once
#include "Source/Source.h"
#include "Source/Actor/Character/Ball/Ball.h"
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

enum Difficulty {
	Easy,
	Normal,
	Hard
};

class Game : public Source
{
private:
	GameCamera* m_gameCamera;	//ゲームカメラ。
	Background* m_background;	//背景。
	Batter* m_batter;			//バッター。
	Pitcher* m_pitcher;			//ピッチャー。
	Catcher* m_catcher;			//キャッチャー。
	Ball* m_ball;				//ボール。
	SkyCube* m_skyCube;			//スカイキューブ。
	InGameUI* m_InGameUI;		//インゲームUI。
	CameraMode m_cameraMode = Camera_Catcher;//初期カメラ
	Start1* m_start1;
	int m_guruguru = 0;
	float m_km = 0;
	bool m_bgmStarted = false;
	float m_afterLandingTimer = 0.0f;
	bool  m_isBallLanded = false;
	float m_zeroDistanceTimer = 0.0f;
	bool m_isRotationSeen = true;
	bool m_isGameStarted = false;
	int m_cameraType = 0;

	int m_scores[3] = { 0,0,0 }; // 各回のスコア保存
	// Game.h など
	bool  m_isMissWait = false;
	float m_missWaitTimer = 0.0f;
	float m_fadeInDelayTimer = -1.0f;
	std::vector<std::vector<Vector3>> m_replayPaths; // 3球分の軌道

	float m_replayTimer = 0.0f;
	float m_replaySpeed = 1.0f;   // 1.0f 固定でOK
	int   m_bestShotIndex = -1;
	std::vector<Vector3> m_currentReplay; // 再生中の軌道
	bool m_shouldStartReplay = false;
	float m_replayDuration = 5.0f;   // リプレイは3秒で打ち切り
	Vector3 m_initialVelocities[3];   // 1球目〜3球目の投球直後の速度
	int m_replayStartFrame = 0;
	int m_replayPitchFrame = 0;
	bool m_isRecording = false;
	int m_replayDelayFrames = 0;
	float m_replayAccumulator = 0.0f;   // 再生速度制御用のアキュムレータ
	float m_replaySwingTimer = 0.0f;
	bool m_hasPlayedReplaySwing = false;  // ★ リプレイ中にスイングを1回だけ再生するためのフラグ
	int m_bestSwingFrame = 0;
	int m_bestPitchFrame = 0;
	float m_replaySkipHoldTime = 0.0f;  // B長押し時間
	bool m_hasAppliedHitMoment = false;
	bool m_startFadeSE2 = false;
	int m_prevGuruGuru = 0;
	bool m_hasSwung[3] = { false, false, false };
	Difficulty m_difficulty = Difficulty::Easy;
	int m_replayFrameCounter = 0;
	int m_swingFrame[3] = { -1, -1, -1 };
	int m_pitchFrame[3] = { 0,0,0 };
	bool  m_isReplayPlaying = false;
	int     m_hitFrame[3];
	Vector3 m_hitVelocities[3];
	Vector3 m_hitDirections[3];
	Vector3 m_hitStartPos[3];
	float   m_hitPower[3];
	int m_shots = 0;          // 何回打ったか
	float m_hitStartZ = 0.0f;
	bool m_hasStartedDistance = false;
	bool m_isHomeRun = false;
	float m_replayDelayTimer = 0.0f;    // リプレイ開始までの遅延タイマー
	float m_readyTimer = 5.0f;
	bool m_isReadyPhase = false;
	bool m_isKakutei = false;
	float m_kakuteiTimer = 0.0f;
	float m_timeScale = 1.0f;
	bool m_isInputLocked = false;
	bool m_canFastForward = false;
	float m_hitStopTimer = 0.0f;
	bool m_isHitStop = false;
	bool m_hasTriggered100m = false;
	struct ReplayFrame {
		Vector3 ballPos;
		Vector3 ballVel;

		float batterAnimTime;
		float pitcherAnimTime;

		bool swingTriggered; // Aボタン押した瞬間
	};
	std::vector<ReplayFrame> m_replayFrames[3]; // 3球分
	bool m_isPaused = false;
	int m_replaySwingDelayFrames = 0;
	bool m_isMagicBallShot[3] = { false, false, false };
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
		if (shotIndex >= 0 && shotIndex < 3) {
			return m_hitFrame[shotIndex];
		}
		return -1; // 無効なインデックスの場合は-1を返す
	}
	void SetHitFrame(int shotIndex, int frame)
	{
		if (shotIndex >= 0 && shotIndex < 3) {
			m_hitFrame[shotIndex] = frame;
		}
	}

	const Vector3& GetHitVelocity(int shotIndex) const
	{
		if (shotIndex >= 0 && shotIndex < 3) {
			return m_hitVelocities[shotIndex];
		}
		static Vector3 zeroVelocity = Vector3::Zero;
		return zeroVelocity; // 無効なインデックスの場合はゼロベクトルを返す
	}
	void SetHitVelocity(int shotIndex, const Vector3& velocity)
	{
		if (shotIndex >= 0 && shotIndex < 3) {
			m_hitVelocities[shotIndex] = velocity;
		}
	}
	const Vector3& GetHitDirection(int shotIndex) const
	{
		if (shotIndex >= 0 && shotIndex < 3) {
			return m_hitDirections[shotIndex];
		}
		static Vector3 zeroDirection = Vector3::Zero;
		return zeroDirection; // 無効なインデックスの場合はゼロベクトルを返す
	}
	void SetHitDirection(int shotIndex, const Vector3& direction)
	{
		if (shotIndex >= 0 && shotIndex < 3) {
			m_hitDirections[shotIndex] = direction;
		}
	}

	const Vector3& GetHitStartPos(int shotIndex) const
	{
		if (shotIndex >= 0 && shotIndex < 3) {
			return m_hitStartPos[shotIndex];
		}
		static Vector3 zeroPos = Vector3::Zero;
		return zeroPos; // 無効なインデックスの場合はゼロベクトルを返す
	}
	void SetHitStartPos(int shotIndex, const Vector3& pos)
	{
		if (shotIndex >= 0 && shotIndex < 3) {
			m_hitStartPos[shotIndex] = pos;
		}
	}

	const float& GetHitPower(int shotIndex) const
	{
		if (shotIndex >= 0 && shotIndex < 3) {
			return m_hitPower[shotIndex];
		}
		static float zeroPower = 0.0f;
		return zeroPower; // 無効なインデックスの場合は0を返す
	}
	void SetHitPower(int shotIndex, float power)
	{
		if (shotIndex >= 0 && shotIndex < 3) {
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
		if (shotIndex >= 0 && shotIndex < 3) {
			return m_replayFrames[shotIndex];
		}
		return std::vector<ReplayFrame>(); // 無効なインデックスの場合は空のベクトルを返す
	}

	void SetReplayFrames(int shotIndex, const std::vector<ReplayFrame>& frames)
	{
		if (shotIndex >= 0 && shotIndex < 3) {
			m_replayFrames[shotIndex] = frames;
		}
	}

	int GetPitchFrame(int shotIndex) const
	{
		if (shotIndex >= 0 && shotIndex < 3) {
			return m_pitchFrame[shotIndex];
		}
		return -1; // 無効なインデックスの場合は-1を返す
	}
	void SetPitchFrame(int shotIndex, int frame)
	{
		if (shotIndex >= 0 && shotIndex < 3) {
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
		return (int)m_ball->m_replayPath.size();
	}
	void SetSwingFrame(int shotIndex, int frame)
	{
		if (shotIndex >= 0 && shotIndex < 3) {
			m_swingFrame[shotIndex] = frame;
		}
	}
	void SetHasSwung(int shotIndex, bool swung) { m_hasSwung[shotIndex] = swung; }
	bool GetHasSwung(int shotIndex) const { return m_hasSwung[shotIndex]; }
	int GetReplayFrameCount() const;
	void StartReplayRecording();
	int GetShots() const { return m_shots; }
	bool IsRecording() const { return m_isRecording; }
	void SetReplaySwingDelayFrames(int frames) { m_replaySwingDelayFrames = frames; }
	int GetReplaySwingDelayFrames() const { return m_replaySwingDelayFrames; }
	void SetIsMagicBallShot(int shot, bool isMagic) {
		if (shot >= 0 && shot < 3) m_isMagicBallShot[shot] = isMagic;
	}
	bool GetIsMagicBallShot(int shot) const {
		return (shot >= 0 && shot < 3) ? m_isMagicBallShot[shot] : false;
	}
};
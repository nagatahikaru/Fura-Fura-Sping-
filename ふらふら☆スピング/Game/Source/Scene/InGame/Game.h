#pragma once
#include "Source/Source.h"
#include "Source/Actor/Character/Ball/Ball.h"
class GameCamera;
class Background;
class InGameUI;
class Batter;
class Pitcher;
class Catcher;
class Ball;
class Start1;


enum CameraMode
{
	Camera_Catcher,
	Camera_Replay,
	Camera_Ball,
	Camera_BackBall
};

class Game : public Source
{
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
	bool m_isPaused = false;

	int GetGuruguru()const{
		return  m_guruguru;
	}
	void SetGuruGuru(int count)
	{
		m_guruguru = count;
	}

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
	GameCamera* GetGameCamera() const { return m_gameCamera; }
	void ResetForNextShot();
	bool m_canFastForward = false;
	float m_hitStopTimer = 0.0f;
	bool m_isHitStop = false;
	bool m_hasTriggered100m = false;
	void OnOver100m();
	void StartEndFade();
	bool IsBallLanded() const { return m_isBallLanded; }
	struct ReplayFrame {
		Vector3 ballPos;
		Vector3 ballVel;

		float batterAnimTime;
		float pitcherAnimTime;

		bool swingTriggered; // Aボタン押した瞬間
	};
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
	int GetReplayFrameCount() const;
	void StartReplayRecording();
	int GetShots() const { return m_shots; }
	bool IsRecording() const { return m_isRecording; }
	void OnPitcherThrow();
	std::vector<ReplayFrame> m_replayFrames[3]; // 3球分
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
	bool m_isGameStarted=false;
	int m_cameraType = 0;
	float m_timeScale = 1.0f;
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
	float m_replayDuration = 5.0f;   // リプレイは5秒で打ち切り
	Vector3 m_initialVelocities[3];   // 1球目〜3球目の投球直後の速度
	int m_replayStartFrame = 0;
	int m_replayPitchFrame = 0;
	bool m_isRecording = false;
	int m_replayDelayFrames = 0;
	float m_replayDelayTimer = 0.0f;    // リプレイ開始までの遅延タイマー
	float m_replayAccumulator = 0.0f;   // 再生速度制御用のアキュムレータ
	float m_replaySwingTimer = 0.0f;
	bool m_hasPlayedReplaySwing = false;  // ★ リプレイ中にスイングを1回だけ再生するためのフラグ
	int m_bestSwingFrame = 0;
	int m_bestPitchFrame = 0;
	float m_replaySkipHoldTime = 0.0f;  // B長押し時間
	bool m_hasAppliedHitMoment = false;
	bool m_startFadeSE2 = false;
	int m_prevGuruGuru = 0;
	bool m_hasTriggeredReplayPitch = false;
	bool m_ballLandedDuringFade = false;
};


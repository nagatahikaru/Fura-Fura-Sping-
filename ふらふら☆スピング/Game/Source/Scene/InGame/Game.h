#pragma once
#include "Source/Source.h"
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
	int m_km = 0;
	bool m_bgmStarted = false;
	float m_afterLandingTimer = 0.0f;
	bool  m_isBallLanded = false;
	float m_zeroDistanceTimer = 0.0f;
	bool m_isRotationSeen = true;
	bool m_isGameStarted=false;
	int m_cameraType = 0;
	float m_timeScale = 1.0f;
	int m_shots = 0;          // 何回打ったか
	int m_scores[3] = { 0,0,0 }; // 各回のスコア保存
	// Game.h など
	bool  m_isMissWait = false;
	float m_missWaitTimer = 0.0f;
	float m_fadeInDelayTimer = -1.0f;
};


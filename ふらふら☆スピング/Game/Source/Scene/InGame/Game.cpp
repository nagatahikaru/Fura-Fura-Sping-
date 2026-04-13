#include "stdafx.h"
#include "Game.h"
#include"Source/Actor/GameCamera/GameCamera.h"
#include"Source/Actor/Stage/Background.h"
#include"Source/UI/InGameUI/InGameUI.h"
#include"Source/Actor/Character/Batter/Batter.h"
#include"Source/Actor/Character/Pitcher/Pitcher.h"
#include"Source/Actor/Character/Catcher/Catcher.h"
#include"Source/Actor/Character/Ball/Ball.h"
#include"Source/Scene/Result/Result.h"
#include"Source/Sound/SoundManager.h"
#include"Source/UI/PauseUI/PauseUI.h"
#include"Source/UI/SoundTestUI/SoundTestUI.h"
#include"Source/Scene/Start/Start.h"


Game::~Game()
{

	DeleteGO(m_gameCamera);
	DeleteGO(m_background);
	DeleteGO(m_batter);
	DeleteGO(m_pitcher);
	DeleteGO(m_catcher);
	DeleteGO(m_ball);
	DeleteGO(m_skyCube);
	DeleteGO(m_InGameUI);
}


bool Game::Start()
{

	// ★ Load で作ったオブジェクトを取得するだけ
	m_skyCube = FindGO<SkyCube>("skycube");
	m_background = FindGO<Background>("backGround");
	m_gameCamera = FindGO<GameCamera>("gameCamera");
	m_InGameUI = FindGO<InGameUI>("inGameUI");
	m_batter = FindGO<Batter>("batter");
	m_pitcher = FindGO<Pitcher>("pitcher");
	m_ball = FindGO<Ball>("ball");
	m_catcher = FindGO<Catcher>("catcher");
	// ★ カウントダウンUIを表示
	m_start1 =NewGO<Start1>(0, "start1");



	// ボールをカメラにセット
	if (m_gameCamera && m_ball) {
		m_gameCamera->SetBall(m_ball);
	}

	return true;
}


void Game::Update()
{
	//当たり判定の表示
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	// ★ カウントダウン中はポーズボタン無効 & ゲームロジック停止
	if (FindGO<Start1>("start1") != nullptr) {

		if (m_InGameUI) {
			m_InGameUI->SetUIVisible(false);
			m_InGameUI->SetFontVisble(false);
			m_InGameUI->SetReplayVisible(false);
		}

		if (m_batter) m_batter->AnimationUpdate();
		if (m_pitcher) m_pitcher->AnimationUpdate();
		if (m_catcher) m_catcher->AnimationUpdate();

		return; // ← START ボタンも完全に無効
	}

	// ★ START ボタン処理（ここに1回だけ）
	if (g_pad[0]->IsTrigger(enButtonStart)) {

		if (FindGO<PauseUI>("pause") != nullptr) return;
		if (FindGO<SoundTestUI>("soundtest") != nullptr) return;

		m_isPaused = true;

		if (m_InGameUI) {
			m_InGameUI->SetPause(true);
		}

		NewGO<PauseUI>(0, "pause");
	}

	// ★ ポーズ中はゲーム停止
	if (m_isPaused) {
		return;
	}


	// ★ Aボタン押しっぱなしで2倍速
	// ★ 打った後だけ倍速ボタンを有効化
	if (m_canFastForward && g_pad[0]->IsPress(enButtonB)) {
		m_timeScale = 5.0f;
	}
	else {
		m_timeScale = 1.0f;
	}

	// ★ ボタンでカメラ切り替え
	// ★ Xボタンでカメラ順番切り替え
	/*if (g_pad[0]->IsTrigger(enButtonLB1)) {
		m_cameraMode = static_cast<CameraMode>((m_cameraMode + 1) % 4);
	}*/

	switch (m_cameraMode) {
	case Camera_Catcher:
		m_gameCamera->SetCatcherCamera();
		break;

	case Camera_Replay:
		m_gameCamera->SetReplayCamera();
		break;

	case Camera_Ball:
		m_gameCamera->SetFollowBallCamera();
		break;

	case Camera_BackBall:
		m_gameCamera->SetFollowBallBackCamera();
		break;
	}

	if (m_cameraMode == Camera_Catcher) {
		m_cameraType = Camera_Catcher;
		m_InGameUI->SetUIVisible(true);
		m_InGameUI->SetFontVisble(true);
		m_InGameUI->SetReplayVisible(false);
	}
	else if(m_cameraMode==Camera_Ball||m_cameraMode==Camera_BackBall)
	{
		m_cameraType = Camera_Ball;
		m_InGameUI->SetUIVisible(false);
		m_InGameUI->SetFontVisble(true);
		m_InGameUI->SetReplayVisible(false);
	}
	else if(m_cameraMode==Camera_Replay)
	{
		m_cameraType = Camera_Replay;
		m_InGameUI->SetUIVisible(false);
		m_InGameUI->SetFontVisble(false);
		m_InGameUI->SetReplayVisible(true);

	}
	else if (m_batter->GetRotationSeen())
	{
		m_InGameUI->SetUIVisible(false);
		m_InGameUI->SetFontVisble(false);
		m_InGameUI->SetReplayVisible(true);
		m_InGameUI->SetGuruGuruCount(GetGuruguru());
	}

	//if (g_pad[0]->IsTrigger(enButtonRB1)) {
	//	 Result*result= NewGO<Result>(0);
	//	//m_guruguru = m_InGameUI->GetGuruguruValue();
	//	//m_km = m_InGameUI->GetKmValue();
	//	result->SetResultValues(m_guruguru, m_km);
	//	if (g_bgm) {
	//		g_bgm->Stop();
	//		g_bgm = nullptr;
	//	}
	//	DeleteGO(this);
	//}

	if (m_isBallLanded) {
		m_afterLandingTimer += (1.0f / 60.0f) * m_timeScale;

		if (m_afterLandingTimer >= 1.0f) {
			Result* result = NewGO<Result>(0);
			result->SetResultValues(m_guruguru, m_km);
			DeleteGO(this);
			return;
		}
	}
	if (m_km <= 0.1f && m_isGameStarted) {
		m_zeroDistanceTimer += (1.0f / 60.0f) * m_timeScale;

		if (m_zeroDistanceTimer >= 10.0f) {
			Result* result = NewGO<Result>(0);
			result->SetResultValues(m_guruguru, m_km);
			DeleteGO(this);
			return;
		}
	}
	else {
		m_zeroDistanceTimer = 0.0f;
	}

	if (m_InGameUI) {
		m_InGameUI->SetKm(m_km);
	}
}

void Game::ResetForNextShot()
{
	m_isBallLanded = false;
	m_afterLandingTimer = 0.0f;
	m_zeroDistanceTimer = 0.0f;
	m_km = 0.0f;

	// ボールを初期位置に戻す
	if (m_ball) {
		m_ball->ResetBall();
	}

	// UI のリセット
	if (m_InGameUI) {
		m_InGameUI->SetKm(0);
		m_InGameUI->SetBaisokuVisible(false);
	}

	// カメラをキャッチャー視点に戻す
	m_cameraMode = Camera_Catcher;
}

void Game::OnBallLanded()
{
    m_isBallLanded = true;
    m_afterLandingTimer = 0.0f;
    m_canFastForward = false;
    m_timeScale = 1.0f;

    // ★ ピッチャーのアニメーションをリセット
    Pitcher* pitcher = FindGO<Pitcher>("pitcher");
    if (pitcher) {
        pitcher->ResetThrow();
    }

    // ★ スコア保存（3球制）
    m_scores[m_shots] = m_km;
    m_shots++;

    if (m_shots >= 3) {
        // 最高スコアを計算して Result へ
        int best = max(m_scores[0], max(m_scores[1], m_scores[2]));
        Result* result = NewGO<Result>(0);
        result->SetResultValues(m_guruguru, best);
        DeleteGO(this);
        return;
    }

    // ★ 次の球の準備
    ResetForNextShot();
}


void Game::Render(RenderContext& rc)
{
	
}
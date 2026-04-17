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

	// ★ ぐるぐる値を毎フレーム Game に保存する
	if (m_batter) {
		SetGuruGuru(m_batter->GetGuruGuru());
	}

	if (m_InGameUI) {
		m_InGameUI->SetBallCount(3-m_shots);
	}

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

	// ★ ヒットストップ処理（ゲーム全体を一瞬停止）
	if (m_hitStopTimer > 0.0f) {
		m_hitStopTimer -= g_gameTime->GetFrameDeltaTime();

		// アニメーションだけは進めたい場合はここに AnimationUpdate() を書く
		if (m_batter)  m_batter->AnimationUpdate();
		if (m_pitcher) m_pitcher->AnimationUpdate();
		if (m_catcher) m_catcher->AnimationUpdate();

		return; // ★ これでゲーム全体が停止する
	}

	// ★ Aボタン押しっぱなしで2倍速
	// ★ 打った後だけ倍速ボタンを有効化
// ★ Aボタン押しっぱなしで2倍速（100m演出中は触らない）
	// ★ フェードアウト中は timeScale を触らない（1倍のまま）
// ★ フェードアウト中は絶対に timeScale を触らない
	if (m_InGameUI && m_InGameUI->IsFadingOut()) {
		m_timeScale = 1.0f;   // ← フェードアウト中は常に1倍速
	}
	else if (!m_hasTriggered100m) {
		// ★ 100m演出前だけ倍速を許可
		if (m_canFastForward && g_pad[0]->IsPress(enButtonB)) {
			m_timeScale = 10.0f;
		}
		else if (m_canFastForward) {
			m_timeScale = 5.0f;
		}
		else {
			m_timeScale = 1.0f;
		}
	}



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

	if (m_isBallLanded) {
		m_afterLandingTimer += (1.0f / 60.0f) * m_timeScale;

		if (m_afterLandingTimer >= 1.0f) {
			m_shots++;
			// ★ 3球終わった？
			if (m_shots >= 3) {
				int best = max(m_scores[0], max(m_scores[1], m_scores[2]));
				Result* result = NewGO<Result>(0);
				result->SetResultValues(m_guruguru, best,m_scores);
				DeleteGO(this);
				return;
			}

			// ★ まだ続く → 次の球へ
			ResetForNextShot();
			return;
		}
	}

	

	if (m_InGameUI) {
		m_InGameUI->SetKm(m_km);
	}

	// ★ フェードイン遅延処理
	if (m_fadeInDelayTimer >= 0.0f) {
		if (m_shots == 2) {
			m_fadeInDelayTimer = -1.0f;
			return;
		}
		m_fadeInDelayTimer -= g_gameTime->GetFrameDeltaTime();

		if (m_fadeInDelayTimer <= 0.0f) {

			// ★ 1.1秒後にフェードイン開始
			if (m_InGameUI) {
				m_InGameUI->StartFadeIn(0.5f);

				m_fadeInDelayTimer = -1.0f;
			}
		}
	}
}

void Game::ResetForNextShot()
{
	m_isBallLanded = false;
	m_afterLandingTimer = 0.0f;
	m_zeroDistanceTimer = 0.0f;
	m_km = 0.0f;
	m_hasTriggered100m = false;   // ★ これを追加
	// ボールを初期位置に戻す
	if (m_ball) {
		m_ball->ResetBall();
	}

	// UI のリセット
	if (m_InGameUI) {
		m_InGameUI->SetKm(0);
		m_InGameUI->SetBaisokuVisible(false);
	}
}

void Game::OnBallLanded()
{
	m_isBallLanded = true;
	m_afterLandingTimer = 0.0f;
	m_canFastForward = false;
	m_timeScale = 1.0f;

	// ピッチャーのアニメーションをリセット
	Pitcher* pitcher = FindGO<Pitcher>("pitcher");
	if (pitcher) {
		pitcher->ResetThrow();
	}

	// スコア保存（3球制）
	m_scores[m_shots] = m_km;
}

void Game::OnOver100m()
{
	if (m_gameCamera) {
		m_gameCamera->FreezeCamera();
	}

	if (m_InGameUI) {
		m_InGameUI->StartFadeOut(0.5f);

		m_InGameUI->m_onFadeOutFinished = [this]() {

			// ★ 完全に黒くなった瞬間にカメラ切り替え
			m_cameraMode = Camera_Catcher;

			// ★ カメラの凍結解除もここで行う（重要）
			if (m_gameCamera) {
				m_gameCamera->UnfreezeCamera();
			}

			// ★ フェードアウト完了 → ここで20倍速にする
			m_timeScale = 20.0f;

			if (m_shots == 2) {
				m_fadeInDelayTimer = -1.0f;
				return;
			}

			// ★ フェードインは 1.1 秒後に実行
			m_fadeInDelayTimer = 1.0f;
		};
	}
	m_canFastForward = false;
	m_hasTriggered100m = true;
}

void Game::Render(RenderContext& rc)
{
	
}
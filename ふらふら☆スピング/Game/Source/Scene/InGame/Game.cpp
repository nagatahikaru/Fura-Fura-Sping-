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

Game::~Game()
{
	if (g_bgm) {
		g_bgm->Stop();
		g_bgm = nullptr;
	}

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

	// ボールをカメラにセット
	if (m_gameCamera && m_ball) {
		m_gameCamera->SetBall(m_ball);
	}

	return true;
}


void Game::Update()
{
	// START でポーズ切り替え
	// START でポーズ切り替え
	// START でポーズ開始
	if (g_pad[0]->IsTrigger(enButtonStart)) {

		// ★ PauseUI があるなら START を無効化
		if (FindGO<PauseUI>("pause") != nullptr) {
			return;
		}

		// ★ SoundTestUI があるなら START を無効化
		if (FindGO<SoundTestUI>("soundtest") != nullptr) {
			return;
		}

		// ★ 通常のポーズ処理
		m_isPaused = true;

		if (m_InGameUI) {
			m_InGameUI->SetPause(true);
		}

		NewGO<PauseUI>(0, "pause");
	}

	// ★ ポーズ中はゲームの動きを完全停止
	if (m_isPaused) {
		return;
	}

	// ★ ボタンでカメラ切り替え
	// ★ Xボタンでカメラ順番切り替え
	if (g_pad[0]->IsTrigger(enButtonX)) {
		m_cameraMode = static_cast<CameraMode>((m_cameraMode + 1) % 4);
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
		m_InGameUI->SetUIVisible(true);
		m_InGameUI->SetFontVisble(true);
		m_InGameUI->SetReplayVisible(false);
	}
	else if(m_cameraMode==Camera_Ball||m_cameraMode==Camera_BackBall)
	{
		m_InGameUI->SetUIVisible(false);
		m_InGameUI->SetFontVisble(true);
		m_InGameUI->SetReplayVisible(false);
	}
	else if(m_cameraMode==Camera_Replay)
	{
		m_InGameUI->SetUIVisible(false);
		m_InGameUI->SetFontVisble(false);
		m_InGameUI->SetReplayVisible(true);

	}

	if (g_pad[0]->IsTrigger(enButtonY)) {
		 Result*result= NewGO<Result>(0);
		//m_guruguru = m_InGameUI->GetGuruguruValue();
		//m_km = m_InGameUI->GetKmValue();
		result->SetResultValues(m_guruguru, m_km);
		if (g_bgm) {
			g_bgm->Stop();
			g_bgm = nullptr;
		}
		DeleteGO(this);
	}
	

}

void Game::Render(RenderContext& rc)
{
	
}
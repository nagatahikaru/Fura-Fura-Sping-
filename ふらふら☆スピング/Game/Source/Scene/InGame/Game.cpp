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

bool Game::Start()
{
	// ▼ 最新の音量を必ず適用して再生
	float v = g_soundManager->m_bgmVolume / 100.0f;
	float curved = powf(v, 1.5f);

	g_bgm = g_soundManager->PlayingSound(enSound_GameBGM1, true, curved);
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_background = NewGO<Background>(0,"backGround");
	m_gameCamera = NewGO<GameCamera>(0,"gameCamera");
	m_InGameUI = NewGO<InGameUI>(0,"inGameUI");
	m_batter = NewGO<Batter>(0,"batter");
	m_pitcher = NewGO<Pitcher>(0,"pitcher");
	//m_catcher = NewGO<Catcher>(0, "catcher");
	m_ball = NewGO<Ball>(0, "ball");

	m_gameCamera->SetBall(m_ball);

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
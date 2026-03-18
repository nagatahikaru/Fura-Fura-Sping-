#include "stdafx.h"
#include "Game.h"
#include"Source/Actor/GameCamera/GameCamera.h"
#include"Source/Actor/Stage/Background.h"
#include"Source/UI/InGameUI/InGameUI.h"
#include"Source/Actor/Character/Batter/Batter.h"
#include"Source/Actor/Character/Pitcher/Pitcher.h"
#include"Source/Actor/Character/Catcher/Catcher.h"

bool Game::Start()
{
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_background = NewGO<Background>(0,"backGround");
	m_gameCamera = NewGO<GameCamera>(0,"gameCamera");
	m_InGameUI = NewGO<InGameUI>(0,"inGameUI");
	m_batter = NewGO<Batter>(0,"batter");
	m_pitcher = NewGO<Pitcher>(0,"pitcher");
	//m_catcher = NewGO<Catcher>(0, "catcher");

	return true;
}

void Game::Update()
{
	//切り替え
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
	}
	// ★ ボタンでカメラ切り替え
	// ★ Yボタンでカメラ順番切り替え
	if (g_pad[0]->IsTrigger(enButtonX)) {
		m_cameraMode = static_cast<CameraMode>((m_cameraMode + 1) % 3);
	}

	if (m_cameraMode == Camera_Catcher) {
		m_InGameUI->SetUIVisible(true);
		m_InGameUI->SetFontVisble(true);
		m_InGameUI->SetReplayVisible(false);
	}
	else if(m_cameraMode==Camera_Ball)
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
}

void Game::Render(RenderContext& rc)
{
	
}
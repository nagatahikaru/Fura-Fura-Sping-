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
	m_catcher = NewGO<Catcher>(0, "catcher");

	return true;
}

void Game::Update()
{
	
	
}

void Game::Render(RenderContext& rc)
{
	
}
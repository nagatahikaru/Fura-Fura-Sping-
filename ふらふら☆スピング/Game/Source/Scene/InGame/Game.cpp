#include "stdafx.h"
#include "Game.h"
#include"Source/Actor/GameCamera/GameCamera.h"
#include"Source/Actor/Stage/Background.h"
#include"Source/UI/InGameUI/InGameUI.h"

bool Game::Start()
{
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_background = NewGO<Background>(0);
	m_gameCamera = NewGO<GameCamera>(0);
	m_InGameUI = NewGO<InGameUI>(0);
	return true;
}

void Game::Update()
{
	
	
}

void Game::Render(RenderContext& rc)
{
	
}
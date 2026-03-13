#include "stdafx.h"
#include "Game.h"
#include"Source/Actor/GameCamera/GameCamera.h"
#include"Source/Actor/Stage/Background.h"

bool Game::Start()
{
	m_background = NewGO<Background>(0);
	m_gameCamera = NewGO<GameCamera>(0);
	return true;
}

void Game::Update()
{
	
	
}

void Game::Render(RenderContext& rc)
{
	
}
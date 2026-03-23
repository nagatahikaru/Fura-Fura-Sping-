#include "stdafx.h"
#include "Titer.h"
#include"Source/Scene/InGame/Game.h"
#include"Source/Sound/SoundManager.h"
#include"Source/Scene/Load/Load.h"
bool Titer::Start()
{
	m_spriteRender.Init("Assets/sprite/Sping.DDS", 1920.0f, 1080.0f);
	//m_uiManager->AddAnimation(std::make_unique<MoveAnimation>(m_transform, 1.0f, Vector2(1.0f, 1.0f), Vector2(5.0f, 5.0f)));

	// ★ BGM 再生（ループ）
	m_bgm = g_soundManager->PlayingSound(enSound_TitleBGM, true, 1.0f);
	return true;
}

void Titer::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		NewGO<Game>(0,"game");
		DeleteGO(this);
	}
	
}

void Titer::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}
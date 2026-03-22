#include "stdafx.h"
#include "Titer.h"
#include"Source/Scene/InGame/Game.h"
#include"Source/Sound/SoundManager.h"
#include"Source/Scene/Load/Load.h"
#include"Source/UI/TiterUI/TiterUI.h"
bool Titer::Start()
{
	m_spriteRender.Init("Assets/sprite/Sping.DDS", 1920.0f, 1080.0f);
	//m_uiManager->AddAnimation(std::make_unique<MoveAnimation>(m_transform, 1.0f, Vector2(1.0f, 1.0f), Vector2(5.0f, 5.0f)));

	// UI を追加
	m_titerUI=NewGO<TiterUI>(0);

	// 念のため Game が残っていたら消す
	Game* game = FindGO<Game>("game");
	if (game) {
		DeleteGO(game);
	}


	// ▼ 保存された音量を取得
	float v = g_soundManager->m_bgmVolume / 100.0f;

	// ▼ カーブ適用（1.8乗など）
	float curved = powf(v, 2.0f);

	// ★ BGM 再生（ループ）
	g_bgm= g_soundManager->PlayingSound(enSound_TitleBGM, true, curved);
	return true;
}

void Titer::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		
		DeleteGO(this);
	}
	
}

void Titer::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}
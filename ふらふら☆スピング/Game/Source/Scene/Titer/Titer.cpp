#include "stdafx.h"
#include "Titer.h"
#include"Source/Scene/InGame/Game.h"
#include"Source/Sound/SoundManager.h"
#include"Source/Scene/Load/Load.h"
#include"Source/UI/TiterUI/TiterUI.h"
bool Titer::Start()
{
	//m_uiManager->AddAnimation(std::make_unique<MoveAnimation>(m_transform, 1.0f, Vector2(1.0f, 1.0f), Vector2(5.0f, 5.0f)));

	// UI を追加
	m_titerUI=NewGO<TiterUI>(0);

	// 念のため Game が残っていたら消す
	Game* game = FindGO<Game>("game");
	if (game) {
		DeleteGO(game);
	}


    //if (!g_soundManager->m_hasPlayedTitleCall)
    //{
    //    // ★ アプリ起動後、初めてタイトルに来た時だけコール
    //    g_soundManager->PlaySE(enSound_TitleCall, 0.0f); 
    //    g_soundManager->m_hasPlayedTitleCall = true;
    //    m_bgmStarted = false;
    //    m_titleCallFrameCount = 0;

    //    // ★ コール再生中はUI操作をロック
    //    if (m_titerUI) {
    //        m_titerUI->SetInputLocked(true);
    //    }
    //}
    //else
    //{
    //    // ★ 2回目以降（ゲームから戻ってきた時）は即BGM
    //    float v = g_soundManager->m_bgmVolume / 100.0f;
    //    float curved = powf(v, 2.0f);
    //    g_bgm = g_soundManager->PlayingSound(enSound_TitleBGM, true, curved);
    //    m_bgmStarted = true;
    //}

         float v = g_soundManager->m_bgmVolume / 100.0f;
        float curved = powf(v, 2.0f);
        g_bgm = g_soundManager->PlayingSound(enSound_TitleBGM, true, curved);
        m_bgmStarted = true;
	return true;
}

void Titer::Update()
{
	if (m_titerUI == nullptr) {
		DeleteGO(this);
        return;
	}

    if (!m_bgmStarted) {
        m_titleCallFrameCount++;
        if (m_titleCallFrameCount >100) // タイトルコールの尺に合わせて調整
        {
            float v = g_soundManager->m_bgmVolume / 100.0f;
            float curved = powf(v, 2.0f);
            g_bgm = g_soundManager->PlayingSound(enSound_TitleBGM, true, curved);
            m_bgmStarted = true;

            // ★ コールが終わったのでロック解除
            m_titerUI->SetInputLocked(false);
        }
    }
}

void Titer::Render(RenderContext& rc)
{
	
}
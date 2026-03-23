#include "stdafx.h"
#include "PauseUI.h"
#include"Source/Scene/Titer/Titer.h"
#include"Source/Scene/InGame/Game.h"
#include"Source/Sound/SoundManager.h"
#include"Source/UI/InGameUI/InGameUI.h"
#include"Source/Scene/SoundTest/SoundTest.h"
#include"Source/UI/SoundTestUI/SoundTestUI.h"
bool PauseUI::Start()
{
    m_spriteRender.Init("Assets/sprite/kuro.DDS", 1920.0f, 1080.0f);
    m_spriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, 0.5f });
    m_startButton.Init("Assets/sprite/StartButton.dds", 600.0f, 450.0f);
    m_startButton.SetPosition({ 0.0f, 200.0f, 0.0f });


    m_titleMenu.Init("Assets/sprite/TitleMenu.dds", 600.0f, 450.0f);
    m_titleMenu.SetPosition({ 0.0f, 0.0f, 0.0f });

    m_soundRender.Init("Assets/sprite/sound.dds", 600.0f, 450.0f);
    m_soundRender.SetPosition({ 0.0f, -200.0f, 0.0f });

    m_spritePause.Init("Assets/sprite/Pause.dds", 600.0f, 450.0f);
    m_spritePause.SetPosition({ 0.0f, 380.0f, 0.0f });

    // ★ ポーズ中は BGM を小さくする
    if (g_bgm) {
        float v = g_soundManager->m_bgmVolume / 100.0f;
        float curved = powf(v, 1.5f);

        float pauseVolume = curved * 0.5f;   // ★ 50% に下げる（自由に調整可）

        g_bgm->SetVolume(pauseVolume);
    }


    return true;
}

void PauseUI::Update()
{
    // ▼ START でも再開
    if (g_pad[0]->IsTrigger(enButtonStart)) {
        Game* game = FindGO<Game>("game");
        if (game) {
            game->m_isPaused = false;

            // ★ UI のポーズ解除を追加
            InGameUI* ui = FindGO<InGameUI>("inGameUI");
            if (ui) {
                ui->SetPause(false);
            }
            // ★ BGM の音量を最新の値に戻す（1.0f は絶対ダメ）
            if (g_bgm) {
                float v = g_soundManager->m_bgmVolume / 100.0f;
                float curved = powf(v, 1.5f);
                g_bgm->SetVolume(curved);
            }
        }
        DeleteGO(this);
        return;
    }

    // ▼ カーソル移動（上下）
    if (g_pad[0]->IsTrigger(enButtonUp)) {
        m_cursor--;
        if (m_cursor < 0) m_cursor = 2;   // ← 3項目なので 0〜2
    }
    if (g_pad[0]->IsTrigger(enButtonDown)) {
        m_cursor++;
        if (m_cursor > 2) m_cursor = 0;   // ← 3項目なので 0〜2
    }


    if (g_pad[0]->IsTrigger(enButtonA)) {

        g_soundManager->PlaySE(enSound_SE); // 決定音

        if (m_cursor == 0) {
            Game* game = FindGO<Game>("game");
            if (game) {
                game->m_isPaused = false;
            }

            if (g_bgm) {
                float v = g_soundManager->m_bgmVolume / 100.0f;
                float curved = powf(v, 1.5f);
                g_bgm->SetVolume(curved);
            }

            DeleteGO(this);
        }

        else if (m_cursor == 1) {
            // タイトルへ
            Game* game = FindGO<Game>("game");
            if (game) DeleteGO(game);

            NewGO<Titer>(0);
            DeleteGO(this);
        }
        else if (m_cursor == 2) {
            Game* game = FindGO<Game>("game");
            if (game) {
                game->m_isPaused = true;  // ゲームを再開
            }

            if (g_bgm) {
                float v = g_soundManager->m_bgmVolume / 100.0f;
                float curved = powf(v, 1.5f);
                g_bgm->SetVolume(curved);
            }


            auto st = NewGO<SoundTestUI>(0,"soundtest");
            st->m_returnType = ReturnToPause;   // ★ ポーズから来たことを記録

            DeleteGO(this);  // PauseUI を消す
        }
    }

    if (m_cursor == 0) {
        m_startButton.SetScale({ 1.2f,1.2f,1.0f });
        m_titleMenu.SetScale({ 1.0f,1.0f,1.0f });
        m_soundRender.SetScale({ 1.0f,1.0f,1.0f });
    }
    else if (m_cursor == 1) {
        m_startButton.SetScale({ 1.0f,1.0f,1.0f });
        m_titleMenu.SetScale({ 1.2f,1.2f,1.0f });
        m_soundRender.SetScale({ 1.0f,1.0f,1.0f });
    }
    else if (m_cursor == 2) {
        m_startButton.SetScale({ 1.0f,1.0f,1.0f });
        m_titleMenu.SetScale({ 1.0f,1.0f,1.0f });
        m_soundRender.SetScale({ 1.2f,1.2f,1.0f });
    }

}


void PauseUI::Render(RenderContext& rc)
{
    m_spriteRender.Draw(rc);
    m_startButton.Update();
    m_startButton.Draw(rc);
    m_titleMenu.Update();
    m_titleMenu.Draw(rc);
    m_soundRender.Update();
    m_soundRender.Draw(rc);
    m_spritePause.Update();
    m_spritePause.Draw(rc);
}
#include "stdafx.h"
#include "PauseUI.h"
#include"Source/Scene/Titer/Titer.h"
#include"Source/Scene/InGame/Game.h"
#include"Source/Sound/SoundManager.h"
#include"Source/UI/InGameUI/InGameUI.h"
#include"Source/Scene/SoundTest/SoundTest.h"
#include"Source/UI/SoundTestUI/SoundTestUI.h"
#include"Source/Scene/Load/Load.h"
bool PauseUI::Start()
{
    m_spriteRender.Init("Assets/sprite/kuro.DDS", 1920.0f, 1080.0f);
    m_spriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, 0.5f });

    m_spritePause.Init("Assets/sprite/Pause.dds", 600.0f, 450.0f);
    m_spritePause.SetPosition({ 0.0f, 380.0f, 0.0f });

    m_start.Init("Assets/sprite/saikai.dds", 250.0f, 150.0f);
    m_start.SetPosition({ 0.0f, 200.0f, 0.0f });

    m_option.Init("Assets/sprite/Soundtest2.dds", 250.0f, 150.0f);
    m_option.SetPosition({ 0.0f, -400.0f, 0.0f });

    m_Title.Init("Assets/sprite/modoru.dds", 250.0f, 150.0f);
    m_Title.SetPosition({ 0.0f, 0.0f, 0.0f });

    m_yari.Init("Assets/sprite/yari.dds", 250.0f, 150.0f);
    m_yari.SetPosition({ 0.0f, -200.0f, 0.0f });
    // ★ ポーズ中は BGM を小さくする
    if (g_bgm) {
        float v = g_soundManager->m_bgmVolume / 100.0f;
        float curved = powf(v, 1.5f);

        float pauseVolume = curved * 0.5f;   // ★ 50% に下げる（自由に調整可）

        g_bgm->SetVolume(pauseVolume);
    }

    // ★ ここを追加：ポーズに入った瞬間 SE2 を止める
// ★ ポーズに入った瞬間 SE2 を一時停止
    if (g_soundManager) {
        g_soundManager->MuteSE2();
    }

    return true;
}

void PauseUI::Update()
{
    // ▼ START でも再開
    if (g_pad[0]->IsTrigger(enButtonStart)) {
        // ★ ここでポーズ突入SE
        if (g_soundManager && g_soundManager->m_seVolume > 0) {
            g_soundManager->PlaySE(enSound_SE);   // 好きなSEに
        }
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
            g_soundManager->UnmuteSE2();
        }
        DeleteGO(this);
        return;
    }

    // ▼ カーソル移動（上下）
    if (g_pad[0]->IsTrigger(enButtonUp)) {
        m_cursor--;
        if (m_cursor < 0) m_cursor = 3;   // ← 3項目なので 0〜2
    }
    if (g_pad[0]->IsTrigger(enButtonDown)) {
        m_cursor++;
        if (m_cursor > 3) m_cursor = 0;   // ← 3項目なので 0〜2
    }


    if (g_pad[0]->IsTrigger(enButtonA)) {

        g_soundManager->PlaySE(enSound_SE); // 決定音

        if (m_cursor == 0) {
            Game* game = FindGO<Game>("game");
            if (game) {
                game->m_isPaused = false;
            }
            InGameUI* ui = FindGO<InGameUI>("inGameUI");
            if (ui) {
                ui->SetPause(false);
            }
            if (g_bgm) {
                float v = g_soundManager->m_bgmVolume / 100.0f;
                float curved = powf(v, 1.5f);
                g_bgm->SetVolume(curved);
            }
            // ★ SE2 を再開
            g_soundManager->UnmuteSE2();
            DeleteGO(this);
        }

        else if (m_cursor == 1) {
            // タイトルへ
            Game* game = FindGO<Game>("game");
            if (game) DeleteGO(game);

            NewGO<Titer>(0);
            DeleteGO(this);
        }
        else if (m_cursor == 3) {
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
        else if (m_cursor == 2) {

              // ★ SE2 を止める
            g_soundManager->StopSE2();

            if (g_bgm) {
                float v = g_soundManager->m_bgmVolume / 100.0f;
                float curved = powf(v, 1.5f);
                g_bgm->SetVolume(curved);
            }

            // ★ ゲームをやり直す（ロード画面から再開）
            Game* game = FindGO<Game>("game");
            if (game) {
                DeleteGO(game);   // 今のゲームを削除
            }

            // ★ Load シーンを開始
            NewGO<Load>(0, "load");

            DeleteGO(this);  // PauseUI を閉じる
        }
    }
    if (m_cursor == 0) {
        m_start.SetScale({ 1.7f,1.7f,1.0f });

        m_Title.SetScale({ 1.0f,1.0f,1.0f });

        m_option.SetScale({ 1.0f,1.0f,1.0f });

        m_yari.SetScale({ 1.0f,1.0f,1.0f });
    }
    else if (m_cursor == 1) {
        m_start.SetScale({ 1.0f,1.0f,1.0f });

        m_Title.SetScale({ 1.7f,1.7f,1.0f });

        m_option.SetScale({ 1.0f,1.0f,1.0f });

        m_yari.SetScale({ 1.0f,1.0f,1.0f });
    }
    else if (m_cursor == 2) {
        m_start.SetScale({ 1.0f,1.0f,1.0f });

        m_Title.SetScale({ 1.0f,1.0f,1.0f });

        m_option.SetScale({ 1.0f,1.0f,1.0f });

        m_yari.SetScale({ 1.7f,1.7f,1.0f });
    }
    else if (m_cursor == 3) {
        m_start.SetScale({ 1.0f,1.0f,1.0f });

        m_Title.SetScale({ 1.0f,1.0f,1.0f });

        m_option.SetScale({ 1.7f,1.7f,1.0f });

        m_yari.SetScale({ 1.0f,1.0f,1.0f });
    }

}


void PauseUI::Render(RenderContext& rc)
{
    m_spriteRender.Draw(rc);
    m_spritePause.Update();
    m_spritePause.Draw(rc);
    m_start.Update();
    m_start.Draw(rc);
    m_option.Update();
    m_option.Draw(rc);
    m_Title.Update();
    m_Title.Draw(rc);
    m_yari.Update();
    m_yari.Draw(rc);
}
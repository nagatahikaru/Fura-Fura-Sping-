#include "stdafx.h"
#include "PauseUI.h"
#include"Source/Scene/Titer/Titer.h"
#include"Source/Scene/InGame/Game.h"
#include"Source/Sound/SoundManager.h"
#include"Source/UI/InGameUI/InGameUI.h"
bool PauseUI::Start()
{
    m_spriteRender.Init("Assets/sprite/kuro.DDS", 1920.0f, 1080.0f);
    m_spriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, 0.5f });
    m_startButton.Init("Assets/sprite/StartButton.dds", 600.0f, 450.0f);
    m_startButton.SetPosition({ 0.0f, -330.0f, 0.0f });


    m_titleMenu.Init("Assets/sprite/TitleMenu.dds", 600.0f, 450.0f);
    m_titleMenu.SetPosition({ 0.0f, -450.0f, 0.0f });

    // ★ ポーズ中は BGM を小さくする
    if (g_bgm) {
        g_bgm->SetVolume(0.2f);   // ← 好きな音量に調整
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
            // ★ BGM の音量を元に戻す
            if (g_bgm) {
                g_bgm->SetVolume(1.0f);   // 元の音量
            }
        }
        DeleteGO(this);
        return;
    }

    // ▼ カーソル移動（上下）
    if (g_pad[0]->IsTrigger(enButtonUp)) {
        m_cursor = 0;   // 再開
    }
    if (g_pad[0]->IsTrigger(enButtonDown)) {
        m_cursor = 1;   // タイトルへ
    }

    // ▼ Aボタンで決定
    if (g_pad[0]->IsTrigger(enButtonA)) {

        g_soundManager->PlaySE(enSound_SE); // 決定音

        if (m_cursor == 0) {
            // 再開
            Game* game = FindGO<Game>("game");
            if (game) {
                game->m_isPaused = false;  // ← これが必要！
            }
            // ★ BGM の音量を元に戻す
            if (g_bgm) {
                g_bgm->SetVolume(1.0f);   // 元の音量
            }
            DeleteGO(this); // PauseUI を消す
        }
        else {
            // タイトルへ戻る
            Game* game = FindGO<Game>("game");
            if (game) {
                DeleteGO(game); // ゲームを完全終了
            }

            NewGO<Titer>(0); // タイトルへ
            DeleteGO(this);  // PauseUI 削除
        }
    }

    // ▼ カーソルに応じて見た目を変える（拡大）
    if (m_cursor == 0) {
        m_startButton.SetScale({ 1.2f, 1.2f, 1.0f });
        m_titleMenu.SetScale({ 1.0f, 1.0f, 1.0f });
    }
    else {
        m_startButton.SetScale({ 1.0f, 1.0f, 1.0f });
        m_titleMenu.SetScale({ 1.2f, 1.2f, 1.0f });
    }
}


void PauseUI::Render(RenderContext& rc)
{
    m_spriteRender.Draw(rc);
    m_startButton.Update();
    m_startButton.Draw(rc);
    m_titleMenu.Update();
    m_titleMenu.Draw(rc);
}
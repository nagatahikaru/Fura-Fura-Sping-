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
    m_start.SetPosition({ 0.0f, 140.0f, 0.0f });

    m_option.Init("Assets/sprite/Soundtest2.dds", 300.0f, 200.0f);
    m_option.SetPosition({ 0.0f, -180.0f, 0.0f });

    m_Title.Init("Assets/sprite/modoru.dds", 250.0f, 150.0f);
    m_Title.SetPosition({ 0.0f, -340.0f, 0.0f });

    m_yari.Init("Assets/sprite/yari.dds", 250.0f, 150.0f);
    m_yari.SetPosition({ 0.0f, -20.0f, 0.0f });

    m_start2.Init("Assets/sprite/saikai2.dds", 250.0f, 150.0f);
    m_start2.SetPosition({ 0.0f,  140.0f, 0.0f });

    m_option2.Init("Assets/sprite/Soundtest3.dds", 300.0f, 200.0f);
    m_option2.SetPosition({ 0.0f,-180.0f, 0.0f });

    m_Title2.Init("Assets/sprite/modoru2.dds", 250.0f, 150.0f);
    m_Title2.SetPosition({ 0.0f,-340.0f, 0.0f });

    m_yari2.Init("Assets/sprite/yari2.dds", 250.0f, 150.0f);
    m_yari2.SetPosition({ 0.0f, -20.0f, 0.0f });

    // ★ ポーズ中は BGM を小さくする
    if (g_bgm) {
        // 現在の音量設定(m_bgmVolume)から計算し、最後に 0.5f を掛ける
        float vol = g_soundManager->GetCalculatedBGMVolume(g_soundManager->m_bgmVolume) * 0.5f;
        g_bgm->SetVolume(vol);
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

        if (g_soundManager && g_soundManager->m_seVolume > 0) {
            g_soundManager->PlaySE(enSound_SE);
        }

        Game* game = FindGO<Game>("game");
        if (game) {
            game->m_isPaused = false;

            InGameUI* ui = FindGO<InGameUI>("inGameUI");
            if (ui) {
                ui->SetPause(false);
            }

            // 【修正箇所】マスターボリュームを適用した正しい音量設定に統一
            if (g_soundManager && g_bgm) {
                g_bgm->SetVolume(g_soundManager->GetCalculatedBGMVolume(g_soundManager->m_bgmVolume));
            }

            if (g_soundManager) {
                g_soundManager->UnmuteSE2();
            }
        }

        DeleteGO(this);
        return;
    }

    static bool canMoveY = true;
    // ▼ カーソル移動（十字キー＋左スティック）
    float y = g_pad[0]->GetLStickYF();

    if (fabsf(y) < 0.3f)
    {
        canMoveY = true;
    }

    // 上
    if ((canMoveY && y > 0.5f))
    {
        m_cursor--;
        if (m_cursor < 0)
            m_cursor = 3;

        g_soundManager->PlaySE(enSound_SE12);
        canMoveY = false;
    }

    // 下
    if ((canMoveY && y < -0.5f))
    {
        m_cursor++;
        if (m_cursor > 3)
            m_cursor = 0;

        g_soundManager->PlaySE(enSound_SE12);
        canMoveY = false;
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
                // 元の音量（マスター適用済み）に戻す
                g_bgm->SetVolume(g_soundManager->GetCalculatedBGMVolume(g_soundManager->m_bgmVolume));
            }
            // ★ SE2 を再開
            g_soundManager->UnmuteSE2();
            DeleteGO(this);
        }

        else if (m_cursor == 1) {

            // ★ SE2 を止める
            g_soundManager->StopSE2();

            if (g_bgm) {
                // 現在の音量設定値に基づく正しいマスター計算値を取得
                g_bgm->SetVolume(g_soundManager->GetCalculatedBGMVolume(g_soundManager->m_bgmVolume));
            }

            // ★ ゲームをやり直す（ロード画面から再開）
            Game* game = FindGO<Game>("game");
            if (game) {
                DeleteGO(game);   // 今のゲームを削除
            }

            // ★ Load シーンを開始
            Load* load = NewGO<Load>(0, "load");
            if (load) {
                load->SetDifficulty(m_difficulty);
            }
            DeleteGO(this);  // PauseUI を閉じる
        }

        else if (m_cursor == 2) {
            Game* game = FindGO<Game>("game");
            if (game) {
                game->m_isPaused = true;  // ゲームを再開
            }

            if (g_bgm) {
                // 現在の音量設定値に基づく正しいマスター計算値を取得
                g_bgm->SetVolume(g_soundManager->GetCalculatedBGMVolume(g_soundManager->m_bgmVolume));
            }


            auto st = NewGO<SoundTestUI>(0, "soundtest");
            st->m_returnType = ReturnToPause;   // ★ ポーズから来たことを記録

            DeleteGO(this);  // PauseUI を消す
        }

        else if (m_cursor == 3) {
            // タイトルへ
            Game* game = FindGO<Game>("game");
            if (game) DeleteGO(game);

            NewGO<Titer>(0);
            DeleteGO(this);
        }
    }
    if (m_cursor == 0) {
        m_start.SetScale({ 2.0f,2.0f,1.0f });
        m_start.SetMulColor({ 1,1,1,1 });
        m_start2.SetMulColor({ 1,1,1,0 });
        m_Title2.SetScale({ 1.0f,1.0f,1.0f });
        m_Title.SetMulColor({ 1,1,1,0});
        m_Title2.SetMulColor({ 1,1,1,1 });
        m_option2.SetScale({ 1.0f,1.0f,1.0f });
        m_option.SetMulColor({ 1,1,1,0 });
        m_option2.SetMulColor({ 1,1,1,1 });
        m_yari2.SetScale({ 1.0f,1.0f,1.0f });
        m_yari.SetMulColor({ 1,1,1,0 });
        m_yari2.SetMulColor({ 1,1,1,1 });
    }
  
    else if (m_cursor == 1) {
        m_start2.SetScale({ 1.0f,1.0f,1.0f });
        m_start.SetMulColor({ 1,1,1,0 });
        m_start2.SetMulColor({ 1,1,1,1 });
        m_Title2.SetScale({ 1.0f,1.0f,1.0f });
        m_Title.SetMulColor({ 1,1,1,0 });
        m_Title2.SetMulColor({ 1,1,1,1 });
        m_yari.SetScale({ 2.0f,2.0f,1.0f });
        m_yari.SetMulColor({ 1,1,1,1 });
        m_yari2.SetMulColor({ 1,1,1,0 });
        m_option2.SetScale({ 1.0f,1.0f,1.0f });
        m_option.SetMulColor({ 1,1,1,0 });
        m_option2.SetMulColor({ 1,1,1,1 });
    }

    else if (m_cursor == 2) {
        m_start2.SetScale({ 1.0f,1.0f,1.0f });
        m_start.SetMulColor({ 1,1,1,0 });
        m_start2.SetMulColor({ 1,1,1,1 });
        m_Title2.SetScale({ 1.0f,1.0f,1.0f });
        m_Title.SetMulColor({ 1,1,1,0 });
        m_Title2.SetMulColor({ 1,1,1,1 });
        m_yari2.SetScale({ 1.0f,1.0f,1.0f });
        m_yari.SetMulColor({ 1,1,1,0 });
        m_yari2.SetMulColor({ 1,1,1,1 });
        m_option.SetScale({ 2.0f,2.0f,1.0f });
        m_option.SetMulColor({ 1,1,1,1 });
        m_option2.SetMulColor({ 1,1,1,0 });
    }

    else if (m_cursor == 3) {
        m_start2.SetScale({ 1.0f,1.0f,1.0f });
        m_start.SetMulColor({ 1,1,1,0 });
        m_start2.SetMulColor({ 1,1,1,1 });
        m_Title.SetScale({ 2.0f,2.0f,1.0f });
        m_Title.SetMulColor({ 1,1,1,1 });
        m_Title2.SetMulColor({ 1,1,1,0 });
        m_option2.SetScale({ 1.0f,1.0f,1.0f });
        m_option.SetMulColor({ 1,1,1,0 });
        m_option2.SetMulColor({ 1,1,1,1 });
        m_yari2.SetScale({ 1.0f,1.0f,1.0f });
        m_yari.SetMulColor({ 1,1,1,0 });
        m_yari2.SetMulColor({ 1,1,1,1 });
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
    m_start2.Update();
    m_start2.Draw(rc);
    m_option2.Update();
    m_option2.Draw(rc);
    m_Title2.Update();
    m_Title2.Draw(rc);
    m_yari2.Update();
    m_yari2.Draw(rc);
}
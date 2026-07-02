#include "stdafx.h"
#include "TiterUI.h"
#include"Source/Scene/Titer/Titer.h"
#include"Source/Scene/SoundTest/SoundTest.h"
#include"Source/Scene/Ranking/Ranking.h"
#include "Source/Scene/Load/Load.h"       
#include "Source/Sound/SoundManager.h"
TiterUI::TiterUI() {

}
TiterUI::~TiterUI() {

 }

bool TiterUI::Start()
{
    // ▼ 共通の背景
    m_spriteRender.Init("Assets/sprite/Title.DDS", 1920.0f, 1080.0f);

    // ▼ メインメニュー用UI
    m_start.Init("Assets/sprite/Start.dds", 250.0f, 150.0f);
    m_start.SetPosition({ 0.0f, -150.0f, 0.0f });

    m_option.Init("Assets/sprite/Soundtest2.dds", 250.0f, 150.0f);
    m_option.SetPosition({ 0.0f, -450.0f, 0.0f });

    m_Title.Init("Assets/sprite/hurahura.dds", 1700.0f, 1300.0f);
    m_Title.SetPosition({ 0.0f, 180.0f, 0.0f });

    m_ranking.Init("Assets/sprite/RankingUI.dds", 250.0f, 150.0f);
    m_ranking.SetPosition({ 0.0f, -300.0f, 0.0f });

    m_start2.Init("Assets/sprite/Start2.dds", 250.0f, 150.0f);
    m_start2.SetPosition({ 0.0f, -150.0f, 0.0f });

    m_option2.Init("Assets/sprite/Soundtest3.dds", 250.0f, 150.0f);
    m_option2.SetPosition({ 0.0f, -450.0f, 0.0f });

    m_ranking2.Init("Assets/sprite/RankingUI2.dds", 250.0f, 150.0f);
    m_ranking2.SetPosition({ 0.0f, -300.0f, 0.0f });

    // ▼ 難易度選択用UI（TransitionUIから引越し）
    m_easySprite.Init("Assets/sprite/Difficulty_Easy.dds", 400.0f, 400.0f);
    m_easySprite.SetPosition({ -480.0f, -320.0f, 0.0f });

    m_normalSprite.Init("Assets/sprite/Difficulty_Normal.dds", 360.0f, 360.0f);
    m_normalSprite.SetPosition({ 0.0f, -320.0f, 0.0f });

    m_hardSprite.Init("Assets/sprite/Difficulty_Hard.dds", 355.0f, 355.0f);
    m_hardSprite.SetPosition({ 480.0f, -320.0f, 0.0f });

    m_easyDetail.Init("Assets/sprite/Detail_Easy.dds", 1608.0f, 1360.0f);
    m_easyDetail.SetPosition({ 0.0f, 0.0f, 0.0f });

    m_normalDetail.Init("Assets/sprite/Detail_Normal.dds", 1590.0f, 1420.0f);
    m_normalDetail.SetPosition({ 0.0f, 14.0f, 0.0f });

    m_hardDetail.Init("Assets/sprite/Detail_Hard.dds", 1590.0f, 1470.0f);
    m_hardDetail.SetPosition({ 0.0f, 10.0f, 0.0f });

    m_nanido.Init("Assets/sprite/nanido.dds", 600.0f, 600.0f);
    m_nanido.SetPosition({ 0.0f, 420.0f, 0.0f });

    m_nanido2.Init("Assets/sprite/nanido2.dds", 700.0f, 500.0f);
    m_nanido2.SetPosition({ 0.0f, 80.0f, 0.0f });

    m_nanido3.Init("Assets/sprite/iaiaiaia.dds", 1400.0f, 1200.0f);
    m_nanido3.SetPosition({ 0.0f, 250.0f, 0.0f });

    m_B.Init("Assets/sprite/Bback.dds", 220.0f, 170.0f);
    m_B.SetPosition({ 830.0f, -400.0f, 0.0f });

    m_grobu.Init("Assets/sprite/guro-bu.dds", 450.0f, 430.0f);
    m_grobu.SetPosition({ 830.0f, -400.0f, 0.0f });

    m_spritekuro.Init("Assets/sprite/kuro.DDS", 1920.0f, 1080.0f);

    // 各種変数の初期化
    m_state = State_MainMenu;
    m_cursor = 0;
    m_selectedDifficulty = 0;
    m_isDeciding = false;
    m_decideTimer = 0.0f;
    m_fadeAlpha = 0.0f;

    return true;
}

void TiterUI::Update()
{
    float dt = g_gameTime->GetFrameDeltaTime();

    // 【共通】難易度決定後のアニメーション・フェードアウト処理
    if (m_isDeciding) {
        m_decideTimer += dt;

        float blink = (sin(m_decideTimer * 20.0f) * 0.5f) + 0.5f;
        if (m_selectedDifficulty == 0)      m_easySprite.SetMulColor({ 1,1,1, blink });
        else if (m_selectedDifficulty == 1) m_normalSprite.SetMulColor({ 1,1,1, blink });
        else if (m_selectedDifficulty == 2) m_hardSprite.SetMulColor({ 1,1,1, blink });

        // ★決定時、切り替わる詳細画像も一緒に点滅させる場合はここに処理を追加できます
        if (m_decideTimer >= 0.5f) {
            m_fadeAlpha += dt * 1.5f;
            if (m_fadeAlpha > 1.0f) m_fadeAlpha = 1.0f;
        }

        if (m_decideTimer >= 1.2f) {
            Load* load = NewGO<Load>(0);
            if (load) {
                load->SetDifficulty(static_cast<Difficulty>(m_selectedDifficulty));
            }
            DeleteGO(this);
        }
        return;
    }

    if (m_state == State_MainMenu) {
        // 上下カーソル移動（既存通り）
        if (g_pad[0]->IsTrigger(enButtonUp)) {
            m_cursor--;
            if (m_cursor < 0) m_cursor = 2;
            g_soundManager->PlaySE(enSound_SE12);
        }
        if (g_pad[0]->IsTrigger(enButtonDown)) {
            m_cursor++;
            if (m_cursor > 2) m_cursor = 0;
            g_soundManager->PlaySE(enSound_SE12);
        }

        // カーソル拡縮演出（既存通り）
        if (m_cursor == 0) {
            m_start2.SetMulColor({ 1,1,1,0 }); m_start.SetMulColor({ 1,1,1,1 }); m_start.SetScale({ 2.0f, 2.0f, 1.0f });
            m_ranking2.SetScale({ 1.0f, 1.0f, 1.0f }); m_ranking.SetMulColor({ 1,1,1,0 }); m_ranking2.SetMulColor({ 1,1,1,1 });
            m_option2.SetScale({ 1.0f, 1.0f, 1.0f }); m_option2.SetMulColor({ 1,1,1,1 }); m_option.SetMulColor({ 1,1,1,0 });
        }
        else if (m_cursor == 1) {
            m_start2.SetScale({ 1.0f, 1.0f, 1.0f }); m_start.SetMulColor({ 1,1,1,0 }); m_start2.SetMulColor({ 1,1,1,1 });
            m_ranking.SetScale({ 2.0f, 2.0f, 1.0f }); m_ranking.SetMulColor({ 1,1,1,1 }); m_ranking2.SetMulColor({ 1,1,1,0 });
            m_option2.SetScale({ 1.0f, 1.0f, 1.0f }); m_option.SetMulColor({ 1,1,1,0 }); m_option2.SetMulColor({ 1,1,1,1 });
        }
        else if (m_cursor == 2) {
            m_start2.SetScale({ 1.0f, 1.0f, 1.0f }); m_start.SetMulColor({ 1,1,1,0 }); m_start2.SetMulColor({ 1,1,1,1 });
            m_ranking2.SetScale({ 1.0f, 1.0f, 1.0f }); m_ranking.SetMulColor({ 1,1,1,0 }); m_ranking2.SetMulColor({ 1,1,1,1 });
            m_option.SetScale({ 2.0f, 2.0f, 1.0f }); m_option.SetMulColor({ 1,1,1,1 }); m_option2.SetMulColor({ 1,1,1,0 });
        }

        if (g_pad[0]->IsTrigger(enButtonA)) {
            g_soundManager->PlaySE(enSound_SE);
            if (m_cursor == 0) {
                m_state = State_DifficultySelect;
                m_selectedDifficulty = 0;
            }
            else if (m_cursor == 1) {
                NewGO<Ranking>(0); DeleteGO(this);
            }
            else if (m_cursor == 2) {
                auto st = NewGO<SoundTestUI>(0); st->m_returnType = ReturnToTitle; DeleteGO(this);
            }
        }
    }
    else if (m_state == State_DifficultySelect) {
        // 左右カーソル移動
        if (g_pad[0]->IsTrigger(enButtonLeft)) {
            m_selectedDifficulty--;
            if (m_selectedDifficulty < 0) m_selectedDifficulty = 2;
            g_soundManager->PlaySE(enSound_SE12);
        }
        if (g_pad[0]->IsTrigger(enButtonRight)) {
            m_selectedDifficulty++;
            if (m_selectedDifficulty > 2) m_selectedDifficulty = 0;
            g_soundManager->PlaySE(enSound_SE12);
        }

        // 下部の難易度ボタンアイコンの拡縮（既存通り）
        if (m_selectedDifficulty == 0) {
            m_easySprite.SetScale({ 2.0f, 2.0f, 1.0f }); m_normalSprite.SetScale({ 1.0f, 1.0f, 1.0f }); m_hardSprite.SetScale({ 1.0f, 1.0f, 1.0f });
        }
        else if (m_selectedDifficulty == 1) {
            m_easySprite.SetScale({ 1.0f, 1.0f, 1.0f }); m_normalSprite.SetScale({ 2.0f, 2.0f, 1.0f }); m_hardSprite.SetScale({ 1.0f, 1.0f, 1.0f });
        }
        else if (m_selectedDifficulty == 2) {
            m_easySprite.SetScale({ 1.0f, 1.0f, 1.0f }); m_normalSprite.SetScale({ 1.0f, 1.0f, 1.0f }); m_hardSprite.SetScale({ 2.0f, 2.0f, 1.0f });
        }

        if (g_pad[0]->IsTrigger(enButtonA)) {
            g_soundManager->PlaySE(enSound_SE);
            m_isDeciding = true;
        }

        if (g_pad[0]->IsTrigger(enButtonB)) {
            m_easySprite.SetMulColor({ 1,1,1,1 }); m_normalSprite.SetMulColor({ 1,1,1,1 }); m_hardSprite.SetMulColor({ 1,1,1,1 });
            m_state = State_MainMenu;
        }
    }
}

void TiterUI::Render(RenderContext& rc)
{
    m_spriteRender.Draw(rc);

    if (m_state == State_MainMenu) {
        m_Title.Update();
        m_Title.Draw(rc);

        m_start.Update();
        m_start.Draw(rc);

        m_ranking.Update();
        m_ranking.Draw(rc);

        m_option.Update();
        m_option.Draw(rc);

        m_start2.Update();
        m_start2.Draw(rc);

        m_ranking2.Update();
        m_ranking2.Draw(rc);

        m_option2.Update();
        m_option2.Draw(rc);
    }
    else if (m_state == State_DifficultySelect) {
        m_nanido.Update();
        m_nanido.Draw(rc);

        m_easySprite.Update();
        m_easySprite.Draw(rc);

        m_normalSprite.Update();
        m_normalSprite.Draw(rc);

        m_hardSprite.Update();
        m_hardSprite.Draw(rc);

        switch (m_selectedDifficulty) {
        case 0: // Easy
            m_easyDetail.Update();
            m_easyDetail.Draw(rc);
            break;
        case 1: // Normal
            m_normalDetail.Update();
            m_normalDetail.Draw(rc);
            break;
        case 2: // Hard
            m_hardDetail.Update();
            m_hardDetail.Draw(rc);
            break;
        }

        m_nanido3.Update();
        m_nanido3.Draw(rc);

       /* m_nanido2.Update();
        m_nanido2.Draw(rc);*/

     /*   m_grobu.Update();
        m_grobu.Draw(rc);*/
        m_B.Update();
        m_B.Draw(rc);

        // ▼ フェードアウト（黒い板）
        if (m_isDeciding) {
            m_spritekuro.SetMulColor({ 1,1,1, m_fadeAlpha });
            m_spritekuro.Draw(rc);
        }
    }
}
#include "stdafx.h"
#include "TiterUI.h"
#include"Source/Scene/Load/Load.h"
#include"Source/Scene/Titer/Titer.h"
#include"Source/Scene/SoundTest/SoundTest.h"
#include"Source/Scene/Ranking/Ranking.h"

TiterUI::TiterUI() {

}
TiterUI::~TiterUI() {

 }

bool TiterUI::Start()
{
    m_spriteRender.Init("Assets/sprite/siro.DDS", 1920.0f, 1080.0f);

    m_start.Init("Assets/sprite/Start.dds", 250.0f, 150.0f);
    m_start.SetPosition({ 0.0f, -300.0f, 0.0f });

    m_option.Init("Assets/sprite/Soundtest2.dds", 250.0f, 150.0f);
    m_option.SetPosition({ 0.0f, -450.0f, 0.0f });

    m_Title.Init("Assets/sprite/hurahura.dds", 1200.0f, 800.0f);
    m_Title.SetPosition({ 0.0f, 300.0f, 0.0f });

    m_gurobu.Init("Assets/sprite/guro-bu.dds", 450.0f, 430.0f);
    m_gurobu.SetPosition({ 730.0f, -400.0f, 0.0f });

    m_ranking.Init("Assets/sprite/RankingUI.dds", 350.0f, 330.0f);
    m_ranking.SetPosition({ 730.0f, -400.0f, 0.0f });

    m_spritekuro.Init("Assets/sprite/kuro.DDS", 1920.0f, 1080.0f);

    return true;
}

void TiterUI::Update()
{
    // ▼ 決定アニメ中は入力を受け付けない
    if (m_isDeciding) {

        // 1. START ボタン点滅（0.5秒）
        m_decideTimer += g_gameTime->GetFrameDeltaTime();
        float blink = (sin(m_decideTimer * 20.0f) * 0.5f) + 0.5f;
        m_start.SetMulColor({ 1,1,1, blink });

        // 2. 点滅後フェードアウト開始
        if (m_decideTimer >= 0.5f) {
            m_fadeAlpha += g_gameTime->GetFrameDeltaTime() * 1.5f;
            if (m_fadeAlpha > 1.0f) m_fadeAlpha = 1.0f;
        }

        // 3. フェード完了 → Load へ
        if (m_fadeAlpha >= 1.0f) {
            NewGO<Load>(0);
            DeleteGO(this);
        }

        return;
    }

    // ▼ カーソル移動（上下）
    if (g_pad[0]->IsTrigger(enButtonUp)) {
        m_cursor = 0;   // ゲーム
    }
    if (g_pad[0]->IsTrigger(enButtonDown)) {
        m_cursor = 1;   // メニュー
    }
    // ▼ 横入力でランキングへ
    if (g_pad[0]->IsTrigger(enButtonRight)) {
        m_cursor = 2;  // ランキング選択状態へ
    }
    if (g_pad[0]->IsTrigger(enButtonLeft)) {
        if (m_cursor == 2) {
            m_cursor = 0;  // ランキングから START に戻る
        }
    }

    // ▼ Aボタンで決定
    if (g_pad[0]->IsTrigger(enButtonA)) {
        g_soundManager->PlaySE(enSound_SE); // ← 決定音
        if (m_cursor == 0) {
            m_isDeciding = true;
            return;
        }
        else if (m_cursor == 1) {
            auto st = NewGO<SoundTestUI>(0); // OPTION
            st->m_returnType = ReturnToTitle;
        }
        else if (m_cursor == 2) {
            NewGO<Ranking>(0);  // ★ ランキングへ
        }
        DeleteGO(this); // UI削除
    }

    // ▼ カーソルに応じて見た目を変える（例：少し拡大）
    if (m_cursor == 0) {
        m_start.SetScale({ 1.7f, 1.7f, 1.0f });   // ← 追加
        m_option.SetScale({ 1.0f, 1.0f, 1.0f });  // ← 追加
        m_ranking.SetScale({1.0f , 1.0f, 1.0f});
    }
    else if (m_cursor == 1) {
        m_start.SetScale({ 1.0f, 1.0f, 1.0f });   // ← 追加
        m_option.SetScale({ 1.7f, 1.7f, 1.0f });  // ← 追加
        m_ranking.SetScale({ 1.0f , 1.0f, 1.0f });
    }
    else if (m_cursor == 2) {
        m_start.SetScale({ 1.0f, 1.0f, 1.0f });   // ← 追加
        m_option.SetScale({ 1.0f, 1.0f, 1.0f });  // ← 追加
        m_ranking.SetScale({ 1.7f, 1.7f, 1.0f });
    }
}


void TiterUI::Render(RenderContext& rc)
{
    m_spriteRender.Draw(rc);
    m_start.Update();
    m_start.Draw(rc);
    m_option.Update();
    m_option.Draw(rc);
    m_Title.Update();
    m_Title.Draw(rc);
    m_gurobu.Update();
    m_gurobu.Draw(rc);
    m_ranking.Update();
    m_ranking.Draw(rc);
    // ▼ フェードアウト（黒い板）
    if (m_isDeciding) {

        m_spritekuro.SetMulColor({ 1,1,1, m_fadeAlpha });
        m_spritekuro.Draw(rc);
    }
}
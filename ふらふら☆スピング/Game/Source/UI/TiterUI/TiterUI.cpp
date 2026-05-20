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
    m_spriteRender.Init("Assets/sprite/Title.DDS", 1920.0f, 1080.0f);

    m_start.Init("Assets/sprite/Start.dds", 250.0f, 150.0f);
    m_start.SetPosition({ 0.0f, -150.0f, 0.0f });

    m_option.Init("Assets/sprite/Soundtest2.dds", 250.0f, 150.0f);
    m_option.SetPosition({ 0.0f, -450.0f, 0.0f });

    m_Title.Init("Assets/sprite/hurahura.dds", 1900.0f, 1500.0f);
    m_Title.SetPosition({ 0.0f, 100.0f, 0.0f });

    m_ranking.Init("Assets/sprite/RankingUI.dds", 250.0f, 150.0f);
    m_ranking.SetPosition({ 0.0f, -300.0f, 0.0f });

    m_spritekuro.Init("Assets/sprite/kuro.DDS", 1920.0f, 1080.0f);

    // ★ タイトルに入ってから 0.2秒は A 無効
    m_inputBlockTime = 0.5f;

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

    // ▼ カーソル移動（上下のみ）
    if (g_pad[0]->IsTrigger(enButtonUp)) {
        m_cursor--;
        if (m_cursor < 0) m_cursor = 2;   // 0 → 上 → 2
        g_soundManager->PlaySE(enSound_SE12);
        m_inputBlockTime = 0.1f;
    }

    if (g_pad[0]->IsTrigger(enButtonDown)) {
        m_cursor++;
        if (m_cursor > 2) m_cursor = 0;   // 2 → 下 → 0
        g_soundManager->PlaySE(enSound_SE12);
        m_inputBlockTime = 0.1f;
    }

    // ▼ カーソル移動後の一瞬は A を無効化 ← ★ ここに置く！
    if (m_inputBlockTime > 0.0f) {
        m_inputBlockTime -= g_gameTime->GetFrameDeltaTime();
        return;
    }

    // ▼ Aボタンで決定
    if (g_pad[0]->IsTrigger(enButtonA)) {
        g_soundManager->PlaySE(enSound_SE); // ← 決定音
        if (m_cursor == 0) {
            m_isDeciding = true;
            return;
        }
        else if (m_cursor == 1) {
            NewGO<Ranking>(0);  // ★ ランキングへ
        }
        else if (m_cursor == 2) {
            auto st = NewGO<SoundTestUI>(0); // OPTION
            st->m_returnType = ReturnToTitle;
        }
        DeleteGO(this); // UI削除
    }

    // ▼ カーソルに応じて見た目を変える（例：少し拡大）
    if (m_cursor == 0) {
        m_start.SetScale({ 2.0f, 2.0f, 1.0f });   // ← 追加
        m_option.SetScale({ 1.0f, 1.0f, 1.0f });  // ← 追加
        m_ranking.SetScale({1.0f , 1.0f, 1.0f});
    }
    else if (m_cursor == 1) {
        m_start.SetScale({ 1.0f, 1.0f, 1.0f });   // ← 追加
        m_option.SetScale({ 1.0f, 1.0f, 1.0f });  // ← 追加
        m_ranking.SetScale({ 2.0f, 2.0f, 1.0f });
    }
    else if (m_cursor == 2) {
        m_start.SetScale({ 1.0f, 1.0f, 1.0f });   // ← 追加
        m_option.SetScale({ 2.0f, 2.0f, 1.0f });  // ← 追加
        m_ranking.SetScale({ 1.0f , 1.0f, 1.0f });
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
    m_ranking.Update();
    m_ranking.Draw(rc);
    // ▼ フェードアウト（黒い板）
    if (m_isDeciding) {

        m_spritekuro.SetMulColor({ 1,1,1, m_fadeAlpha });
        m_spritekuro.Draw(rc);
    }
}
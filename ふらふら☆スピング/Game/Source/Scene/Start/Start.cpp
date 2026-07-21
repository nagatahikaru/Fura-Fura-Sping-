#include "stdafx.h"
#include "Start.h"
#include"Source/Actor/Character/Batter/Batter.h"
#include"Source/Actor/Character/Pitcher/Pitcher.h"
#include"Source/Actor/Character/Catcher/Catcher.h"
#include"Source/Actor/Character/Ball/Ball.h"
#include"Source/Sound/SoundManager.h"

bool Start1::Start()
{
    // ★ カウントダウン開始 → ゲーム全体を停止
    Game* game = FindGO<Game>("game");
    if (game) {
        game->SetIsPaused(true);
    }

	m_3.Init("Assets/sprite/3.dds", 400.0f, 300.0f);
	m_3.SetPosition({ 0.0f, 0.0f, 0.0f });
	m_2.Init("Assets/sprite/2.dds", 400.0f, 300.0f);
	m_2.SetPosition({ 0.0f, 0.0f, 0.0f });
	m_1.Init("Assets/sprite/1.dds", 400.0f, 300.0f);
	m_1.SetPosition({ 0.0f, 0.0f, 0.0f });
	m_Start.Init("Assets/sprite/start!.dds", 600.0f, 500.0f);
	m_Start.SetPosition({ 0.0f, 0.0f, 0.0f });
	return true;
}

void Start1::Update()
{
    m_timer += g_gameTime->GetFrameDeltaTime();
    if (m_timer >= 4.0f) {

        // ★ カウントダウン終了 → ゲーム再開
        Game* game = FindGO<Game>("game");
        if (game) {
            game->SetIsPaused(false);
        }

        DeleteGO(this);
    }

    // 0〜1秒 → 3
    if (m_timer < 1.0f) {
        float t = m_timer / 1.0f;   // 0 → 1
        m_alpha = 1.0 - t;
        m_scale3 = 1.5f + (0.5f - 1.5f) * t;   // 1.5 → 0.5
        m_3.SetScale({ m_scale3, m_scale3, 1.0f });
        m_3.SetMulColor({ 1,1,1,m_alpha });
        m_2.SetMulColor({ 1,1,1,0 });
        m_1.SetMulColor({ 1,1,1,0 });
        m_Start.SetMulColor({ 1,1,1,0 });
    }
    // 1〜2秒 → 2
    else if (m_timer < 2.0f) {
        float t = (m_timer - 1.0f) / 1.0f;
        m_alpha = 1.0 - t;
        m_scale2 = 1.5f + (0.5f - 1.5f) * t;
        m_3.SetMulColor({ 1,1,1,0 });
        m_2.SetMulColor({ 1,1,1,m_alpha });
        m_2.SetScale({ m_scale2, m_scale2, 1.0f });
        m_1.SetMulColor({ 1,1,1,0 });
        m_Start.SetMulColor({ 1,1,1,0 });
    }
    // 2〜3秒 → 1
    else if (m_timer < 3.0f) {
        float t = (m_timer - 2.0f) / 1.0f;
        m_alpha = 1.0 - t;
        m_scale1 = 1.5f + (0.5f - 1.5f) * t;
        m_3.SetMulColor({ 1,1,1,0 });
        m_2.SetMulColor({ 1,1,1,0 });
        m_1.SetMulColor({ 1,1,1,m_alpha });
        m_1.SetScale({ m_scale1, m_scale1, 1.0f });
        m_Start.SetMulColor({ 1,1,1,0 });
    }
    // 3〜4秒 → START!!（奥から手前に“現れる” → 最後0.2秒停止）
    else if (m_timer < 4.0f) {

        // ★ START!! に入った瞬間だけ SE 再生
        if (!m_playedStartSE) {
            if (g_soundManager) {
                g_soundManager->PlaySE(Sound::enSound_SE, 100.0f);
            }
            m_playedStartSE = true;
        }

        float t = (m_timer - 3.0f);   // 0.0 → 1.0

        // -----------------------------
        // フェーズ1：0.0〜0.8秒（80%）
        // -----------------------------
        if (t < 0.75f) {
            float nt = t / 0.75f;  // 0 → 1 に正規化

            // アルファ 0 → 1
            m_alpha = nt;

            // スケール 0.5 → 1.5
            m_scaleStart = 0.5f + (1.5f - 0.5f) * nt;
        }
        // -----------------------------
        // フェーズ2：0.8〜1.0秒（20%）
        // -----------------------------
        else {
            // ピタッと停止
            m_alpha = 1.0f;
            m_scaleStart = 1.5f;
        }

        m_3.SetMulColor({ 1,1,1,0 });
        m_2.SetMulColor({ 1,1,1,0 });
        m_1.SetMulColor({ 1,1,1,0 });

        m_Start.SetMulColor({ 1,1,1,m_alpha });
        m_Start.SetScale({ m_scaleStart, m_scaleStart, 1.0f });
    }

    // 4秒後 → 自動削除
    else {
        // ★ カウントダウン終了 → キャラ動作開始
        auto batter = FindGO<Batter>("batter");
        auto pitcher = FindGO<Pitcher>("pitcher");
        auto ball = FindGO<Ball>("ball");

        if (batter) batter->SetIsPaused(false);
        if (pitcher) pitcher->SetIsPaused(false);
        if (ball) ball->SetIsPaused(false);

        DeleteGO(this);
    }
}

void Start1::Render(RenderContext& rc)
{
	m_3.Update();
	m_3.Draw(rc);
	m_2.Update();
	m_2.Draw(rc);
	m_1.Update();
	m_1.Draw(rc);
	m_Start.Update();
	m_Start.Draw(rc);
}
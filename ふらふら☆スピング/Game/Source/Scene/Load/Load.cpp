#include "stdafx.h"
#include "Load.h"
#include "Source/Scene/InGame/Game.h"
#include"Source/Scene/Titer/Titer.h"
#include"Source/Sound/SoundManager.h"

bool Load::Start()
{
    // 背景
    m_spriteRender.Init("Assets/sprite/Lrad.dds", 1920.0f, 1080.0f);
    m_guruguru.Init("Assets/sprite/guruguru.dds", 500.f, 500.0f);
    m_guruguru.SetPosition({ 790.0f, -450.0f, 0.0f });
    m_gaugeFrame.Init("Assets/sprite/gauge.dds", 2450.0f, 1300.0f);
    m_gaugeFrame.SetPosition({ 0.0f, -350.0f, 0.0f });
    m_gaugeFill.Init("Assets/sprite/gauge2.dds", 1095.0f, 105.0f);
    m_gaugeFill.SetPosition({ -554.0f, -318.5f, 0.0f });
    m_gaugeFill.SetPivot({ 0.0f, 0.5f });   // 左端基準

    return true;
}

void Load::Update()
{

    // ロード進行度を増やす（例：毎フレーム 0.01）
    m_loadProgress += 1.0f / (60.0f * 60.0f);
    if (m_loadProgress > 1.0f) {
        m_loadProgress = 1.0f;
    }

    // ゲージの横幅を進行度に合わせる
    m_gaugeFill.SetScale({ m_loadProgress, 1.0f, 1.0f });

    m_blinkTimer +=4.0f / 60.0f; // 毎フレーム加算（60FPS前提）

    // アルファ値を0.5?1.0の範囲で変化させる（sin波で滑らかに）
    float alpha = 0.6f + 0.45f * sinf(m_blinkTimer * 1.0f); // 1秒周期

    // 乗算カラーでアルファを設定（RGBは1.0fで維持）
    m_guruguru.SetMulColor({ 1.0f, 1.0f, 1.0f, alpha });


    if (m_loadProgress >= 1.0f) {
        // ★ロード画面のBGMを停止

        if (g_bgm) {
            g_bgm->Stop();
            g_bgm = nullptr;
        }
        NewGO<Game>(0, "game");   // ← ★名前を付ける
        DeleteGO(this);
    }
}

void Load::Render(RenderContext& rc)
{
    m_spriteRender.Draw(rc);
   
    m_gaugeFrame.Update();
    m_gaugeFrame.Draw(rc);
    m_gaugeFill.Update();
    m_gaugeFill.Draw(rc);
    m_guruguru.Update();
    m_guruguru.Draw(rc);
}

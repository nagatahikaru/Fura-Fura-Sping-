#include "stdafx.h"
#include "Load.h"
#include "Source/Scene/InGame/Game.h"
#include"Source/Scene/Titer/Titer.h"
#include"Source/Sound/SoundManager.h"
#include "Source/Actor/Stage/Background.h"
#include "Source/Actor/GameCamera/GameCamera.h"
#include "Source/UI/InGameUI/InGameUI.h"
#include "Source/Actor/Character/Batter/Batter.h"
#include "Source/Actor/Character/Pitcher/Pitcher.h"
#include "Source/Actor/Character/Ball/Ball.h"
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
    // ★ 最初の1フレームは描画だけしてロード処理をしない
    if (m_waitFrame < 1) {
        m_waitFrame++;
        return;
    }

    // ★ ロードが終わった後の待ち処理
    if (m_loadFinished) {
        m_finishWait += g_gameTime->GetFrameDeltaTime();

        // ★ ゲージ満タンから3秒後に一度だけBGM再生
        if (!m_bgmStarted && m_finishWait >= 1.0f) {
            float v = g_soundManager->m_bgmVolume / 100.0f;
            float curved = powf(v, 1.5f);
            g_soundManager->PlayingSound(enSound_GameBGM1, true, curved);
            m_bgmStarted = true;
        }

        // ★ 5秒後にゲーム開始（ここは好みで調整）
        if (m_finishWait >= 3.0f) {
            NewGO<InGameUI>(0, "inGameUI");
            NewGO<Game>(0, "game");
            DeleteGO(this);
        }
        return;
    }

    // ★ ロードステップ処理
    switch (m_loadStep)
    {
    case 0:
        NewGO<SkyCube>(0, "skyCube");
        m_realProgress = 0.1f;
        m_loadStep++;
        break;

    case 1:
        NewGO<Background>(0, "backGround");
        m_realProgress = 0.4f;
        m_loadStep++;
        break;

    case 2:
        NewGO<GameCamera>(0, "gameCamera");
        m_realProgress = 0.6f;
        m_loadStep++;
        break;

    case 3:
        NewGO<Batter>(0, "batter");
        NewGO<Pitcher>(0, "pitcher");
        NewGO<Ball>(0, "ball");
        m_realProgress = 0.8f;
        m_loadStep++;
        break;

    case 4:
        m_realProgress = 1.0f;
        m_loadStep++;
        break;

    case 5:
        m_loadFinished = true;
        m_finishWait = 0.0f;
        return;
    }
    // ★ ゲージは実際の進行値に向かってゆっくり伸びる
    float speed = 0.25f;  // 伸びるスピード（調整可）

    if (m_displayProgress < m_realProgress) {
        m_displayProgress += speed;
        if (m_displayProgress > m_realProgress) {
            m_displayProgress = m_realProgress;
        }
    }

    // ゲージ更新
    m_gaugeFill.SetScale({ m_displayProgress, 1.0f, 1.0f });

}


void Load::Render(RenderContext& rc)
{
    m_spriteRender.Draw(rc);
   
    m_gaugeFrame.Update();
    m_gaugeFrame.Draw(rc);
    (rc);

    // ★ 最初の1フレームだけゲージFillを描画しない
    if (m_waitFrame >= 1) {
        m_gaugeFill.Update();
        m_gaugeFill.Draw(rc);
    }
    m_guruguru.Update();
    m_guruguru.Draw(rc);
}

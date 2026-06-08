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
#include "Source/Actor/Character/Catcher/Catcher.h"
#include"Source/UI/LoadUI/LoadUI.h"
bool Load::Start()
{
    // 背景
    m_spriteRender.Init("Assets/sprite/siro.dds", 1920.0f, 1080.0f);
    m_guruguru.Init("Assets/sprite/guruguru.dds", 500.f, 500.0f);
    m_guruguru.SetPosition({ 790.0f, -450.0f, 0.0f });
    m_gaugeFrame.Init("Assets/sprite/gauge.dds", 2450.0f, 1300.0f);
    m_gaugeFrame.SetPosition({ 0.0f, -350.0f, 0.0f });
    m_gaugeFill.Init("Assets/sprite/gauge2.dds", 1095.0f, 105.0f);
    m_gaugeFill.SetPosition({ -554.0f, -318.5f, 0.0f });
    m_gaugeFill.SetPivot({ 0.0f, 0.5f });   // 左端基準
    m_B.Init("Assets/sprite/AA.dds", 220.0f, 170.0f);
    m_B.SetPosition({ 800.0f, -400.0f, 0.0f });
    m_grobu.Init("Assets/sprite/guro-bu.dds", 450.0f, 430.0f);
    m_grobu.SetPosition({ 800.0f, -400.0f, 0.0f });
    NewGO<LoadUI>(0, "loadUI");
    return true;
}

void Load::Update()
{
    // ★ 最初の待機 (変更なし)
    if (m_waitFrame < 1) {
        m_gaugeFill.SetMulColor({ 1,1,1,0 });
        m_grobu.SetMulColor({ 1,1,1,0 });
        m_B.SetMulColor({ 1,1,1,0 });
        m_waitFrame++;
        return;
    }

    // ★ ロード完了後の処理 (変更なし)
    if (m_loadFinished) {
        m_finishWait += g_gameTime->GetFrameDeltaTime();
        if (!m_bgmStarted && m_finishWait >= 1.0f) {
            float v = g_soundManager->m_bgmVolume / 100.0f;
            g_soundManager->PlayingSound(enSound_GameBGM1, true, powf(v, 1.5f));
            m_bgmStarted = true;
        }
        if (g_pad[0]->IsTrigger(enButtonA)) {
            if (!m_bgmStarted) {
                float v = g_soundManager->m_bgmVolume / 100.0f;
                g_soundManager->PlayingSound(enSound_GameBGM1, true, powf(v, 1.5f));
                m_bgmStarted = true;
            }
            auto loadUI = FindGO<LoadUI>("loadUI");
            if (loadUI) DeleteGO(loadUI);
            NewGO<InGameUI>(0, "inGameUI");
            NewGO<Game>(0, "game");
            DeleteGO(this);
        }
        return;
    }

    // ★ 1. ゲージを伸ばす処理 (変更なし)
    if (m_displayProgress < m_realProgress) {
        float speed = 0.05f;
        m_displayProgress += speed;
        if (m_displayProgress > m_realProgress) m_displayProgress = m_realProgress;
        m_gaugeFill.SetScale({ m_displayProgress, 1.0f, 1.0f });
        return;
    }

    // ★ 2. 待機処理（static をやめ、クラスのメンバ変数 m_isWaitingNextStep を使うことを推奨）
    // ※今回は一旦そのままにしますが、バグの原因になりやすいので注意してください
    static bool isWaitingNextStep = false;
    if (!isWaitingNextStep) {
        isWaitingNextStep = true;
        return;
    }
    isWaitingNextStep = false;

    // ★ 3. ロード実行
    switch (m_loadStep)
    {
    case 0:
        NewGO<SkyCube>(0, "skyCube");
        m_gaugeFill.SetMulColor({ 1,1,1,1 });
        m_realProgress = 0.2f;
        break;
    case 1:
        NewGO<Background>(0, "backGround");
        m_realProgress = 0.4f;
        break;
    case 2:
        NewGO<GameCamera>(0, "gameCamera");
        m_realProgress = 0.6f;
        break;
    case 3:
    {
        auto batter = NewGO<Batter>(0, "batter");
        auto pitcher = NewGO<Pitcher>(0, "pitcher");
        auto catcher = NewGO<Catcher>(0, "catcher");
        auto ball = NewGO<Ball>(0, "ball");
        if (batter && pitcher && catcher && ball) { // ★ ヌルチェックを追加して安全に
            batter->m_isPaused = pitcher->m_isPaused = catcher->m_isPaused = ball->m_isPaused = true;
        }
        m_realProgress = 0.8f;
    }
    break;
    case 4:
        m_realProgress = 1.0f;
        g_soundManager->StopBGM();
        break;
    case 5:
        m_loadFinished = true;
        m_guruguru.SetMulColor({ 1,1,1,0 });
        m_grobu.SetMulColor({ 1,1,1,1 });
        m_B.SetMulColor({ 1,1,1,1 });
        return;
    }

    // ★ 4. 安全にヒントを切り替える
    // m_loadStep が進む前に、存在しているか・壊れていないかを慎重にチェック
    auto ui = FindGO<LoadUI>("loadUI");
    if (ui != nullptr) {
        if (m_loadStep == 1 || m_loadStep == 3) { // ステップ増加前に合わせる、または適切なタイミングに調整
            ui->AdvanceTip();
        }
    }

    m_loadStep++;
}

void Load::Render(RenderContext& rc)
{
    m_spriteRender.Draw(rc);

    m_gaugeFrame.Update();
    m_gaugeFrame.Draw(rc);

    if (m_waitFrame >= 1) {
        m_gaugeFill.Update();
        m_gaugeFill.Draw(rc);
    }
    m_guruguru.Update();
    m_guruguru.Draw(rc);

    m_grobu.Update();
    m_grobu.Draw(rc);

    m_B.Update();
    m_B.Draw(rc);
}
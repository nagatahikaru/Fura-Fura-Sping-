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
#include <cstdlib>  
#include <ctime>    

bool Load::Start()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    // 背景
    m_spriteRender.Init("Assets/sprite/siro.dds", 1920.0f, 1080.0f);
    m_guruguru.Init("Assets/sprite/guruguru.dds", 500.f, 500.0f);
    m_guruguru.SetPosition({ 790.0f, -450.0f, 0.0f });
    m_gaugeFrame.Init("Assets/sprite/gauge.dds", 1670.0f, 1100.0f);
    m_gaugeFrame.SetPosition({ 0.0f, -399.0f, 0.0f });
    m_gaugeFill.Init("Assets/sprite/gauge2.dds", 1100.5f, 107.0f);
    m_gaugeFill.SetPosition({ -549.8f, -373.5f, 0.0f });
    m_gaugeFill.SetPivot({ 0.0f, 0.5f });   // 左端基準
    m_B.Init("Assets/sprite/AA.dds", 350.0f, 300.0f);
    m_B.SetPosition({ 0.0f, -430.0f, 0.0f });
    m_grobu.Init("Assets/sprite/guro-bu.dds", 450.0f, 430.0f);
    m_grobu.SetPosition({ 800.0f, -400.0f, 0.0f });
    m_guL.Init("Assets/sprite/guruguruL.dds", 250.0f, 250.0f);
    m_guL.SetPosition({ 755.0f, -300.0f, 0.0f });
    m_guR.Init("Assets/sprite/guruguruR.dds", 250.0f, 250.0f);
    m_guR.SetPosition({ 800.0f, -300.0f, 0.0f });
    m_guB.Init("Assets/sprite/guruguruB.dds", 250.0f, 250.0f);
    m_guB.SetPosition({ 757.5f, -300.0f, 0.0f });
    m_guA.Init("Assets/sprite/guruguruF.dds", 250.0f, 250.0f);
    m_guA.SetPosition({ 777.5f, -300.0f, 0.0f });
    m_koke.Init("Assets/sprite/gurugurukoke.dds", 250.0f, 250.0f);
    m_koke.SetPosition({ 777.5f, -300.0f, 0.0f });
    m_siri.Init("Assets/sprite/gurugurusiri.dds", 250.0f, 250.0f);
    m_siri.SetPosition({ 777.5f, -300.0f, 0.0f });
    NewGO<LoadUI>(0, "loadUI");
    return true;
}

void Load::Update()
{
    // ★ 最初の待機 (変更なし)
    if (m_waitFrame < 1) {
        m_gaugeFill.SetMulColor({ 1,1,1,0 });
        m_grobu.SetMulColor({ 1,1,1,0 });
        m_koke.SetMulColor({ 1, 1, 1,0});
        m_siri.SetMulColor({ 1, 1, 1, 0});
        m_B.SetMulColor({ 1,1,1,0 });
        m_guL.SetMulColor({ 1, 1, 1, 1 });
        m_guA.SetMulColor({ 1,1,1,0 });
        m_guR.SetMulColor({ 1,1,1,0 });
        m_guB.SetMulColor({ 1, 1, 1, 0 });
       
        m_waitFrame++;
        return;
    }

    if (m_loadFinished) {
        m_finishWait += g_gameTime->GetFrameDeltaTime();
        if (!m_bgmStarted && m_finishWait >= 1.0f) {
            float v = g_soundManager->m_bgmVolume / 100.0f;
            g_soundManager->PlayingSound(enSound_GameBGM1, true, powf(v, 1.5f));
            m_bgmStarted = true;
        }

        if (m_isHappened) {
            m_koke.SetMulColor({ 1, 1, 1, (m_luckyImage == 1) ? 1.0f : 0.0f });
            m_siri.SetMulColor({ 1, 1, 1, (m_luckyImage == 2) ? 1.0f : 0.0f });
            m_guL.SetPosition({ 755.0f, -300.0f, 0.0f });
            m_guR.SetPosition({ 800.0f, -300.0f, 0.0f });
            m_guB.SetPosition({ 757.5f, -300.0f, 0.0f });
            m_guA.SetPosition({ 777.5f, -300.0f, 0.0f });
            m_koke.SetPosition({ 777.5f, -300.0f, 0.0f });
            m_siri.SetPosition({ 777.5f, -300.0f, 0.0f });
            m_guL.SetMulColor({ 0, 0, 0, 0 });
            m_guA.SetMulColor({ 0, 0, 0, 0 });
            m_guR.SetMulColor({ 0, 0, 0, 0 });
            m_guB.SetMulColor({ 0, 0, 0, 0 });
            m_guruguru.SetMulColor({ 0, 0, 0, 0 });
        }
        else {
            // --- 通常の点滅処理と抽選 ---
            m_timer++;

            // 切り替わりの瞬間に30%の抽選
            if (m_timer == 0 || m_timer == 5 || m_timer == 10 || m_timer == 15) {
                if ((rand() % 100) < 30) {
                    m_isHappened = true;
                    m_luckyImage = (rand() % 2) + 1; // 1:koke, 2:siri
                }
            }

            // 通常の点滅
            m_guL.SetMulColor({ 1, 1, 1, (m_timer >= 0 && m_timer < 5) ? 1.0f : 0.0f });
            m_guA.SetMulColor({ 1, 1, 1, (m_timer >= 5 && m_timer < 10) ? 1.0f : 0.0f });
            m_guR.SetMulColor({ 1, 1, 1, (m_timer >= 10 && m_timer < 15) ? 1.0f : 0.0f });
            m_guB.SetMulColor({ 1, 1, 1, (m_timer >= 15 && m_timer < 20) ? 1.0f : 0.0f });

            if (m_timer >= 20) m_timer = 0;
        }

        // 🌟 ボタン押し込み（一気に縮小 → 元に戻る）演出用の変数
        static int clickState = 0;       // 0:通常, 1:縮小中, 2:元に戻り中
        static float clickWaitTimer = 0.0f;

        // --- 🌟 Aボタンを押した後の演出管理 ---
        if (clickState == 1) {
            // ステップ1: 小さい状態をキープ
            clickWaitTimer -= g_gameTime->GetFrameDeltaTime();
            if (clickWaitTimer <= 0.0f) {
                // 🌟一気に元の大きさに戻す！
                m_grobu.SetScale({ 1.0f, 1.0f, 1.0f });
                m_B.SetScale({ 1.0f, 1.0f, 1.0f });

                clickState = 2;          // 次のステップへ
                clickWaitTimer = 0.08f;  // 元の大きさを見せる時間（0.08秒）
            }
            return;
        }
        else if (clickState == 2) {
            // ステップ2: 元の大きさになったので、時間を待ってからゲームへ遷移
            clickWaitTimer -= g_gameTime->GetFrameDeltaTime();
            if (clickWaitTimer <= 0.0f) {
                clickState = 0; // リセット

                // UIを非表示にする
                m_grobu.SetMulColor({ 1,1,1,0 });
                m_B.SetMulColor({ 1,1,1,0 });

                // 🌟 【ここに先ほどのコードを丸ごと入れます】
                auto loadUI = FindGO<LoadUI>("loadUI");
                if (loadUI) DeleteGO(loadUI);
                NewGO<InGameUI>(0, "inGameUI");
                NewGO<Game>(0, "game");
                Game* game = FindGO<Game>("game");
                if (game) {
                    game->SetDifficulty(m_difficulty);
                    // ★ ロードで決まった天候をゲームに引き渡す
                    game->SetWeatherType(m_weatherResult);
                }
                DeleteGO(this);
            }
            return;
        }

        if (g_pad[0]->IsTrigger(enButtonA)) {
            if (!m_bgmStarted) {
                float v = g_soundManager->m_bgmVolume / 100.0f;
                float curved = powf(v, 1.5f);
                g_soundManager->PlayingSound(enSound_GameBGM1, true, curved);
                m_bgmStarted = true;
            }

            // 🌟 1. 押した瞬間に一気に小さくする！
            float scaleValue = 0.40f; // 押し込んだ時のサイズ（お好みで調整）
            m_grobu.SetScale({ scaleValue, scaleValue, 1.0f });
            m_B.SetScale({ scaleValue, scaleValue, 1.0f });

            // 🌟 2. 状態を「縮小中」にして、0.08秒だけ待つ
            clickState = 1;
            clickWaitTimer = 0.12f; // 小さくなっている時間（0.08秒）
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
    {
        auto sky = NewGO<SkyCube>(0, "skyCube");
        auto sceneLight = FindGO<SceneLight>("sceneLight");

        if (sky) {
            bool isRainy = false; // ライト演出の分岐用（小雨・大雨どちらもtrue扱い）

            if (m_difficulty == Difficulty::Hard) {
                int r = rand() % 100; // 0〜99

                if (r < 40) {
                    m_weatherResult = WeatherType::Sunny;
                    isRainy = false;
                    sky->SetType(enSkyCubeType_Day);
                }
                else if (r < 80) {
                    m_weatherResult = WeatherType::LightRain;
                    isRainy = true;
                    sky->SetType(enSkyCubeType_DayToon_4);
                }
                else {
                    m_weatherResult = WeatherType::HeavyRain;
                    isRainy = true;
                    sky->SetType(enSkyCubeType_DayToon_4); // 大雨用の別スカイキューブがあれば差し替え可
                }
            }
            else {
                m_weatherResult = WeatherType::Sunny;
                sky->SetType(enSkyCubeType_Day);
            }

            // ライトのカラーと環境光を天候に合わせて切り替える
            if (sceneLight != nullptr) {
                if (isRainy) {
                    sceneLight->SetDirectionLight(
                        0,
                        Vector3(1.0f, 1.0f, -1.0f),
                        Vector3(0.05f, 0.06f, 0.09f)
                    );
                    sceneLight->SetAmbinet(Vector3(0.002f, 0.002f, 0.004f));
                }
                else {
                    sceneLight->SetDirectionLight(
                        0,
                        Vector3(1.0f, -1.0f, -1.0f),
                        Vector3(1.2f, 1.2f, 1.2f)
                    );
                    sceneLight->SetAmbinet(Vector3(0.1f, 0.1f, 0.1f));
                }
            }
        }
        m_gaugeFill.SetMulColor({ 1,1,1,1 });
        m_guL.SetMulColor({ 1, 1, 1, 0 });
        m_guA.SetMulColor({ 1,1,1,1 });
        m_guR.SetMulColor({ 1,1,1,0 });
        m_guB.SetMulColor({ 1, 1, 1, 0 });
        m_realProgress = 0.1f;
    }
    break;
    case 1:
        m_guL.SetMulColor({ 1, 1, 1, 0 });
        m_guA.SetMulColor({ 1,1,1,0 });
        m_guR.SetMulColor({ 1,1,1,1 });
        m_guB.SetMulColor({ 1, 1, 1, 0 });
        m_realProgress = 0.2f;
        break;
    case 2:
        NewGO<Background>(0, "backGround");
        m_guL.SetMulColor({ 1, 1, 1, 0 });
        m_guA.SetMulColor({ 1,1,1,0 });
        m_guR.SetMulColor({ 1,1,1,0 });
        m_guB.SetMulColor({ 1, 1, 1, 1});
        m_realProgress = 0.3f;
        break;
    case 3:
        m_guL.SetMulColor({ 1, 1, 1, 1 });
        m_guA.SetMulColor({ 1,1,1,0 });
        m_guR.SetMulColor({ 1,1,1,0 });
        m_guB.SetMulColor({ 1, 1, 1, 0 });
        m_realProgress = 0.4f;
        break;
    case 4:
        NewGO<GameCamera>(0, "gameCamera");
        m_guL.SetMulColor({ 1, 1, 1, 0 });
        m_guA.SetMulColor({ 1,1,1,1 });
        m_guR.SetMulColor({ 1,1,1,0 });
        m_guB.SetMulColor({ 1, 1, 1, 0 });
        m_realProgress = 0.5f;
        break;
    case 5:
        m_guL.SetMulColor({ 1, 1, 1, 0 });
        m_guA.SetMulColor({ 1,1,1,0 });
        m_guR.SetMulColor({ 1,1,1,1 });
        m_guB.SetMulColor({ 1, 1, 1, 0 });
        m_realProgress = 0.6f;
        break;
    case 6:
    {
        auto batter = NewGO<Batter>(0, "batter");
        auto pitcher = NewGO<Pitcher>(0, "pitcher");
        auto catcher = NewGO<Catcher>(0, "catcher");
        auto ball = NewGO<Ball>(0, "ball");
        if (batter && pitcher && catcher && ball) { // ★ ヌルチェックを追加して安全に
            batter-> SetIsPaused(true);
            pitcher->SetIsPaused(true);
            catcher->SetIsPaused(true);
            ball->   SetIsPaused(true);
        }
        m_guL.SetMulColor({ 1, 1, 1, 0 });
        m_guA.SetMulColor({ 1,1,1,0 });
        m_guR.SetMulColor({ 1,1,1,0 });
        m_guB.SetMulColor({ 1, 1, 1, 1 });
        m_realProgress = 0.7f;
    }
    break;
    case 7:
        m_guL.SetMulColor({ 1, 1, 1, 1 });
        m_guA.SetMulColor({ 1,1,1,0 });
        m_guR.SetMulColor({ 1,1,1,0 });
        m_guB.SetMulColor({ 1, 1, 1, 0 });
        m_realProgress = 0.8f;
        break;
    case 8:
        m_guL.SetMulColor({ 1, 1, 1, 0 });
        m_guA.SetMulColor({ 1,1,1,1 });
        m_guR.SetMulColor({ 1,1,1,0 });
        m_guB.SetMulColor({ 1, 1, 1, 0 });
        m_realProgress = 0.9f;
        break;
    case 9:
        m_realProgress = 1.0f;
        g_soundManager->StopBGM();
        m_guL.SetMulColor({ 1, 1, 1, 0 });
        m_guA.SetMulColor({ 1,1,1,0 });
        m_guR.SetMulColor({ 1,1,1,1 });
        m_guB.SetMulColor({ 1, 1, 1, 0 });
        break;
    case 10:
       /* m_guL.SetMulColor({ 1, 1, 1, 0 });
        m_guR.SetMulColor({ 1,1,1,0 });
        m_guB.SetMulColor({ 1, 1, 1, 0 });
        m_guA.SetMulColor({ 1,1,1,0 });*/
        m_loadFinished = true;
        m_gaugeFrame.SetMulColor({ 1,1,1,0 });
        m_gaugeFill.SetMulColor({ 1,1,1,0 });
        m_guruguru.SetMulColor({ 1,1,1,0 });
        m_grobu.SetMulColor({ 1,1,1,1 });
        m_B.SetMulColor({ 1,1,1,1 });
        return;
    }

    // m_loadStep が進む前に、存在しているか・壊れていないかを慎重にチェック
    auto ui = FindGO<LoadUI>("loadUI");
    if (ui != nullptr) {
        if (m_loadStep == 3 || m_loadStep == 6) { // ステップ増加前に合わせる、または適切なタイミングに調整
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

    m_guL.Update();
    m_guL.Draw(rc);

    m_guR.Update();
    m_guR.Draw(rc);

    m_guB.Update();
    m_guB.Draw(rc);

    m_guA.Update();
    m_guA.Draw(rc);

    m_koke.Update();
    m_koke.Draw(rc);

    m_siri.Update();
    m_siri.Draw(rc);

   /* m_grobu.Update();
    m_grobu.Draw(rc);*/

    m_B.Update();
    m_B.Draw(rc);
}
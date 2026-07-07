#include "stdafx.h"
#include "SoundTestUI.h"
#include"Source/Sound/SoundManager.h"
#include"Source/Scene/Titer/Titer.h"
#include"Source/UI/PauseUI/PauseUI.h"
#include"Source/Scene/InGame/Game.h"
#include"Source/UI/InGameUI/InGameUI.h"

// ファイル冒頭付近に追加（std::clampが使えない場合のため）
template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

bool SoundTestUI::Start() {
	m_spriteRender.Init("Assets/sprite/SoundTest.DDS", 1920.0f, 1080.0f);

    m_spriteMaster.Init("Assets/sprite/MasterButton.dds", 1200.0f, 650.0f);
    m_spriteMaster.SetScale(Vector3{ -1.0f,1.0f,1.0f });
    m_spriteMaster.SetPosition({ 0.0f, -350.0f, 0.0f });

	m_spriteBGM.Init("Assets/sprite/StartButton.dds", 1200.0f, 650.0f);
    m_spriteBGM.SetScale(Vector3{ -1.0f,1.0f,1.0f });
	m_spriteBGM.SetPosition({ 0.0f, 150.0f, 0.0f });

	m_spriteSE.Init("Assets/sprite/TitleMenu.dds", 1170.0f, 600.0f);
    m_spriteSE.SetScale(Vector3{ -1.0f,1.0f,1.0f });
	m_spriteSE.SetPosition({ 0.0f, -100.0f, 0.0f });


	m_spriteBallBGM.Init("Assets/sprite/ball.dds",50.0f, 50.0f);

    m_spriteBallSE.Init("Assets/sprite/ball.dds", 50.0f, 50.0f);

    m_spriteBallMaster.Init("Assets/sprite/ball.dds", 50.0f, 50.0f);


    m_spriteeFontMaster.Init("Assets/sprite/ball.dds", 80.0f, 80.0f);
    m_spriteeFontMaster.SetPosition({ 540.0f, -340.0f, 0.0f });

    m_spriteFontBGM.Init("Assets/sprite/ball.dds", 80.0f, 80.0f);
    m_spriteFontBGM.SetPosition({ 540.0f, 155.0f, 0.0f });

    m_spriteFontSE.Init("Assets/sprite/ball.dds", 80.0f, 80.0f);
    m_spriteFontSE.SetPosition({ 540.0f, -90.0f, 0.0f });


    m_Master.Init("Assets/sprite/Master.dds", 400.0f, 320.0f);
    m_Master.SetPosition({ -680.0f, -370.0f, 0.0f });

    m_BGM.Init("Assets/sprite/BGM.dds", 400.0f, 320.0f);
    m_BGM.SetPosition({ -650.0f, 130.0f, 0.0f });

    m_SE.Init("Assets/sprite/SE.dds", 400.0f, 300.0f);
    m_SE.SetPosition({ -630.0f, -100.0f, 0.0f });

    m_B.Init("Assets/sprite/Bback.dds", 220.0f, 170.0f);
    m_B.SetPosition({ 830.0f, -490.0f, 0.0f });

    m_grobu.Init("Assets/sprite/guro-bu.dds", 450.0f, 430.0f);
    m_grobu.SetPosition({ 730.0f, -400.0f, 0.0f });

    // ▼ 保存された音量をスライダー位置に変換
    float masterT = g_soundManager->m_masterVolume / 100.0f;
    float bgmT = g_soundManager->m_bgmVolume / 100.0f;
    float seT = g_soundManager->m_seVolume / 100.0f;

    m_masterX = m_minX + (m_maxX - m_minX) * masterT;
    m_bgmX = m_minX + (m_maxX - m_minX) * bgmT;
    m_seX = m_minX + (m_maxX - m_minX) * seT;

	return true;
}

void SoundTestUI::Update() {
    // ★ サウンドテスト中は START を無効化
    if (g_pad[0]->IsTrigger(enButtonStart)) {
        return;
    }

    // ▼ 上下で BGM(0) / SE(1) / MASTER(2) の選択
    if (g_pad[0]->IsTrigger(enButtonUp)) {
        m_select--;
        if (m_select < 0) m_select = 2; // ループ処理
        g_soundManager->PlaySE(enSound_SE12);   // ★ カーソル移動音
    }
    if (g_pad[0]->IsTrigger(enButtonDown)) {
        m_select++;
        if (m_select > 2) m_select = 0; // ループ処理
        g_soundManager->PlaySE(enSound_SE12);   // ★ カーソル移動音
    }

    // ▼ 左右でボールを動かす
    if (g_pad[0]->IsPress(enButtonRight)) {
        if (m_select == 0)      m_bgmX += 5.0f;
        else if (m_select == 1) m_seX += 5.0f;
        else                    m_masterX += 5.0f;
    }
    if (g_pad[0]->IsPress(enButtonLeft)) {
        if (m_select == 0)      m_bgmX -= 5.0f;
        else if (m_select == 1) m_seX -= 5.0f;
        else                    m_masterX -= 5.0f;
    }

    // ▼ Aボタンで SE を再生
    if (g_pad[0]->IsTrigger(enButtonA)) {
        // SEスライダー(1)、またはマスター(2) を選択中なら確認のために SE を鳴らす
        if (m_select == 1 || m_select == 2) {
            g_soundManager->PlaySE(enSound_SE);
        }
    }

    // 範囲制限
    m_masterX = clamp<float>(m_masterX, m_minX, m_maxX);
    m_bgmX = clamp<float>(m_bgmX, m_minX, m_maxX);
    m_seX = clamp<float>(m_seX, m_minX, m_maxX);

    // ▼ 位置 → 音量（0〜1）に変換
    float masterT = (m_masterX - m_minX) / (m_maxX - m_minX);
    float bgmT = (m_bgmX - m_minX) / (m_maxX - m_minX);
    float seT = (m_seX - m_minX) / (m_maxX - m_minX);

    // ▼ 位置 → 音量（0〜100）
    float masterVolume = masterT * 100.0f;
    float bgmVolume = bgmT * 100.0f;
    float seVolume = seT * 100.0f;

    // 即時反映
    g_soundManager->SetMasterVolume(masterVolume);
    g_soundManager->SetBGMVolume(bgmVolume);
    g_soundManager->SetSEVolume(seVolume);

    if (g_pad[0]->IsTrigger(enButtonB)) {
        // ▼ 最新のスライダー値を保存
        g_soundManager->m_masterVolume = masterVolume;
        g_soundManager->m_bgmVolume = bgmVolume;
        g_soundManager->m_seVolume = seVolume;

        // ▼ 音量の再適用
        g_soundManager->SetMasterVolume(g_soundManager->m_masterVolume);
        g_soundManager->SetBGMVolume(g_soundManager->m_bgmVolume);

        if (m_returnType == ReturnToTitle) {
            NewGO<Titer>(0);
        }
        else if (m_returnType == ReturnToPause) {
            NewGO<PauseUI>(0, "pause");

            Game* game = FindGO<Game>("game");
            if (game) {
                game->m_isPaused = true;
            }

            InGameUI* ui = FindGO<InGameUI>("inGameUI");
            if (ui) {
                ui->SetPause(true);
            }
        }
        DeleteGO(this);
    }
}

void SoundTestUI::Render(RenderContext& rc) {
    m_spriteRender.Draw(rc);

    m_spriteMaster.Update();
    m_spriteMaster.Draw(rc);

    m_spriteBGM.Update();
    m_spriteBGM.Draw(rc);

    m_spriteSE.Update();
    m_spriteSE.Draw(rc);

    m_spriteeFontMaster.Update();
    m_spriteeFontMaster.Draw(rc);

    m_spriteFontBGM.Update();
    m_spriteFontBGM.Draw(rc);

    m_spriteFontSE.Update();
    m_spriteFontSE.Draw(rc);

    m_Master.Update();
    m_Master.Draw(rc);

    m_BGM.Update();
    m_BGM.Draw(rc);

    m_SE.Update();
    m_SE.Draw(rc);

 /*   m_grobu.Update();
    m_grobu.Draw(rc);*/

    m_B.Update();
    m_B.Draw(rc);

    // ▼ BGM ボール（m_select == 0 で拡大）
    m_spriteBallBGM.SetPosition({ m_bgmX, 160.0f, 0.0f });
    if (m_select == 0) m_spriteBallBGM.SetScale({ 1.8f, 1.8f, 1.0f });
    else               m_spriteBallBGM.SetScale({ 1.0f, 1.0f, 1.0f });
    m_spriteBallBGM.Update();
    m_spriteBallBGM.Draw(rc);

    // ▼ SE ボール（m_select == 1 で拡大）
    m_spriteBallSE.SetPosition({ m_seX, -90.0f, 0.0f });
    if (m_select == 1) m_spriteBallSE.SetScale({ 1.8f, 1.8f, 1.0f });
    else               m_spriteBallSE.SetScale({ 1.0f, 1.0f, 1.0f });
    m_spriteBallSE.Update();
    m_spriteBallSE.Draw(rc);

    // ▼ MASTER ボール（m_select == 2 で拡大）
    m_spriteBallMaster.SetPosition({ m_masterX, -340.0f, 0.0f });
    if (m_select == 2) m_spriteBallMaster.SetScale({ 1.8f, 1.8f, 1.0f });
    else               m_spriteBallMaster.SetScale({ 1.0f, 1.0f, 1.0f });
    m_spriteBallMaster.Update();
    m_spriteBallMaster.Draw(rc);


    const float charWidth = 20.0f;

    // ▼ 数値表示（BGM）
    int bgmVal = (int)g_soundManager->m_bgmVolume;
    wchar_t bgmText[32];
    swprintf_s(bgmText, L"%d", bgmVal);
    int digitsBGM = wcslen(bgmText);
    float bgmOffsetX = -(digitsBGM * charWidth) * 0.5f;

    m_fontBGM.SetText(bgmText);
    m_fontBGM.SetPosition(530.0f + bgmOffsetX, 175.0f, 0.0f);
    m_fontBGM.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
    m_fontBGM.Draw(rc);


    // ▼ 数値表示（SE）
    int seVal = (int)g_soundManager->m_seVolume;
    wchar_t seText[32];
    swprintf_s(seText, L"%d", seVal);
    int digitsSE = wcslen(seText);
    float seOffsetX = -(digitsSE * charWidth) * 0.5f;

    m_fontSE.SetText(seText);
    m_fontSE.SetPosition(530.0f + seOffsetX, -70.0f, 0.0f);
    m_fontSE.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
    m_fontSE.Draw(rc);


    // ▼ 数値表示（MASTER）
    int masterVal = (int)g_soundManager->m_masterVolume;
    wchar_t masterText[32];
    swprintf_s(masterText, L"%d", masterVal);
    int digitsMaster = wcslen(masterText);
    float masterOffsetX = -(digitsMaster * charWidth) * 0.5f;

    m_fontMaster.SetText(masterText);
    m_fontMaster.SetPosition(530.0f + masterOffsetX, -320.0f, 0.0f);
    m_fontMaster.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
    m_fontMaster.Draw(rc);
}
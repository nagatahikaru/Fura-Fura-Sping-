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

	m_spriteBGM.Init("Assets/sprite/StartButton.dds", 1200.0f, 650.0f);
	m_spriteBGM.SetPosition({ 0.0f, -50.0f, 0.0f });


	m_spriteSE.Init("Assets/sprite/TitleMenu.dds", 1170.0f, 600.0f);
	m_spriteSE.SetPosition({ 0.0f, -300.0f, 0.0f });


	m_spriteBallBGM.Init("Assets/sprite/ball.dds",50.0f, 50.0f);

    m_spriteBallSE.Init("Assets/sprite/ball.dds", 50.0f, 50.0f);

    m_spriteFontBGM.Init("Assets/sprite/ball.dds", 80.0f, 80.0f);
    m_spriteFontBGM.SetPosition({ 540.0f, -45.0f, 0.0f });

    m_spriteFontSE.Init("Assets/sprite/ball.dds", 80.0f, 80.0f);
    m_spriteFontSE.SetPosition({ 540.0f, -290.0f, 0.0f });

    m_BGM.Init("Assets/sprite/BGM.dds", 400.0f, 320.0f);
    m_BGM.SetPosition({ -630.0f, -50.0f, 0.0f });

    m_SE.Init("Assets/sprite/SE.dds", 400.0f, 300.0f);
    m_SE.SetPosition({ -630.0f, -300.0f, 0.0f });

    m_B.Init("Assets/sprite/Bback.dds", 220.0f, 170.0f);
    m_B.SetPosition({ 730.0f, -400.0f, 0.0f });

    m_grobu.Init("Assets/sprite/guro-bu.dds", 450.0f, 430.0f);
    m_grobu.SetPosition({ 730.0f, -400.0f, 0.0f });

    // ▼ 保存された音量をスライダー位置に変換
    float bgmT = g_soundManager->m_bgmVolume / 100.0f;
    float seT = g_soundManager->m_seVolume / 100.0f;

    m_bgmX = m_minX + (m_maxX - m_minX) * bgmT;
    m_seX = m_minX + (m_maxX - m_minX) * seT;

	return true;
}
void SoundTestUI::Update() {
    // ★ サウンドテスト中は START を無効化
    if (g_pad[0]->IsTrigger(enButtonStart)) {
        return;
    }

    // ▼ 上下で BGM / SE の選択
    if (g_pad[0]->IsTrigger(enButtonUp)) {
        m_select = 0;
    }
    if (g_pad[0]->IsTrigger(enButtonDown)) {
        m_select = 1;
    }

    // ▼ 左右でボールを動かす
    if (g_pad[0]->IsPress(enButtonRight)) {
        if (m_select == 0) m_bgmX += 5.0f;
        else               m_seX += 5.0f;
    }
    if (g_pad[0]->IsPress(enButtonLeft)) {
        if (m_select == 0) m_bgmX -= 5.0f;
        else               m_seX -= 5.0f;
    }

    // 範囲制限
    m_bgmX = clamp(m_bgmX, m_minX, m_maxX);
    m_seX = clamp(m_seX, m_minX, m_maxX);

    // ▼ 位置 → 音量（0?100）に変換
    float bgmT = (m_bgmX - m_minX) / (m_maxX - m_minX);
    float seT = (m_seX - m_minX) / (m_maxX - m_minX);

    // ▼ 位置 → 音量（0〜100）
    float bgmVolume = bgmT * 100.0f;
    float seVolume = seT * 100.0f;

    // ★ カーブは SoundManager に任せる
    g_soundManager->SetBGMVolume(bgmVolume);
    g_soundManager->SetSEVolume(seVolume);


    if (g_pad[0]->IsTrigger(enButtonB)) {


        // ▼ 最新のスライダー値を保存（これが超重要）
        g_soundManager->m_bgmVolume = bgmVolume;
        g_soundManager->m_seVolume = seVolume;

        // ▼ BGM にも反映
        g_soundManager->SetBGMVolume(g_soundManager->m_bgmVolume);

        if (m_returnType == ReturnToTitle) {
            NewGO<Titer>(0);
        }
        else if (m_returnType == ReturnToPause) {

            // ★ PauseUI を復元
            NewGO<PauseUI>(0, "pause");

            // ★ ゲームは停止状態のまま
            Game* game = FindGO<Game>("game");
            if (game) {
                game->m_isPaused = true;
            }

            // ★ InGameUI も停止状態に戻す（これが超重要）
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

    m_spriteBGM.Update();
    m_spriteBGM.Draw(rc);

    m_spriteSE.Update();
    m_spriteSE.Draw(rc);

    m_spriteFontBGM.Update();
    m_spriteFontBGM.Draw(rc);

    m_spriteFontSE.Update();
    m_spriteFontSE.Draw(rc);

    m_BGM.Update();
    m_BGM.Draw(rc);

    m_SE.Update();
    m_SE.Draw(rc);

    m_grobu.Update();
    m_grobu.Draw(rc);

    m_B.Update();
    m_B.Draw(rc);

    // ▼ BGM ボール
    m_spriteBallBGM.SetPosition({ m_bgmX, -40.0f, 0.0f });
    if (m_select == 0) m_spriteBallBGM.SetScale({ 1.5f, 1.5f, 1.0f });
    else               m_spriteBallBGM.SetScale({ 1.0f, 1.0f, 1.0f });
    m_spriteBallBGM.Update();
    m_spriteBallBGM.Draw(rc);

    // ▼ SE ボール
    m_spriteBallSE.SetPosition({ m_seX, -290.0f, 0.0f });
    if (m_select == 1) m_spriteBallSE.SetScale({ 1.5f, 1.5f, 1.0f });
    else               m_spriteBallSE.SetScale({ 1.0f, 1.0f, 1.0f });
    m_spriteBallSE.Update();
    m_spriteBallSE.Draw(rc);


    // ▼ 数値表示（BGM）
    int bgmVal = (int)g_soundManager->m_bgmVolume;
    wchar_t bgmText[32];
    swprintf_s(bgmText, L"%d", bgmVal);

    // 文字幅の仮定（フォントサイズ40なら約20px）
    const float charWidthBGM = 20.0f;

    // 桁数を取得
    int digitsBGM = wcslen(bgmText);

    // 中央揃えオフセット（文字列の半分の幅）
    float bgmOffsetX = -(digitsBGM * charWidthBGM) * 0.5f;

    m_fontBGM.SetText(bgmText);
    m_fontBGM.SetPosition(530.0f + bgmOffsetX, -25.0f, 0.0f);
    m_fontBGM.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
    m_fontBGM.Draw(rc);


    // ▼ 数値表示（SE）
    int seVal = (int)g_soundManager->m_seVolume;
    wchar_t seText[32];
    swprintf_s(seText, L"%d", seVal);

    // 文字幅の仮定
    const float charWidthSE = 20.0f;

    // 桁数
    int digitsSE = wcslen(seText);

    // 中央揃えオフセット
    float seOffsetX = -(digitsSE * charWidthSE) * 0.5f;

    m_fontSE.SetText(seText);
    m_fontSE.SetPosition(530.0f + seOffsetX, -270.0f, 0.0f);
    m_fontSE.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
    m_fontSE.Draw(rc);

}

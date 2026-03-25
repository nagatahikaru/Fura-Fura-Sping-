#include "stdafx.h"
#include "TiterUI.h"
#include"Source/Scene/Load/Load.h"
#include"Source/Scene/Titer/Titer.h"
#include"Source/Scene/SoundTest/SoundTest.h"

TiterUI::TiterUI() {

}
TiterUI::~TiterUI() {

 }

bool TiterUI::Start()
{
    m_startButton.Init("Assets/sprite/StartButton.dds", 600.0f, 450.0f);
    m_startButton.SetPosition({ 0.0f, -300.0f, 0.0f });
    
    m_titleMenu.Init("Assets/sprite/TitleMenu.dds", 600.0f, 450.0f);
    m_titleMenu.SetPosition({ 0.0f, -450.0f, 0.0f });

    m_start.Init("Assets/sprite/Start.dds", 250.0f, 150.0f);
    m_start.SetPosition({ -550.0f, -300.0f, 0.0f });

    m_option.Init("Assets/sprite/Soundtest2.dds", 250.0f, 150.0f);
    m_option.SetPosition({ -550.0f, -450.0f, 0.0f });
    return true;
}

void TiterUI::Update()
{
    // ▼ カーソル移動（上下）
    if (g_pad[0]->IsTrigger(enButtonUp)) {
        m_cursor = 0;   // ゲーム
    }
    if (g_pad[0]->IsTrigger(enButtonDown)) {
        m_cursor = 1;   // メニュー
    }

    // ▼ Aボタンで決定
    if (g_pad[0]->IsTrigger(enButtonA)) {
        g_soundManager->PlaySE(enSound_SE); // ← 決定音
        if (m_cursor == 0) {
            // ゲームへ
            NewGO<Load>(0);
        }
        else {
            // メニューへ
            auto st = NewGO<SoundTestUI>(0);
            st->m_returnType = ReturnToTitle;   // ★ タイトルから来たことを記録
        }
       
        DeleteGO(this); // UI削除
    }

    // ▼ カーソルに応じて見た目を変える（例：少し拡大）
    if (m_cursor == 0) {
        m_startButton.SetScale({ 1.5f, 1.5f, 1.0f });
        m_titleMenu.SetScale({ 1.0f, 1.0f, 1.0f });
        m_start.SetScale({ 1.7f, 1.7f, 1.0f });   // ← 追加
        m_option.SetScale({ 1.0f, 1.0f, 1.0f });  // ← 追加
    }
    else {
        m_startButton.SetScale({ 1.0f, 1.0f, 1.0f });
        m_titleMenu.SetScale({ 1.5f, 1.5f, 1.0f });
        m_start.SetScale({ 1.0f, 1.0f, 1.0f });   // ← 追加
        m_option.SetScale({ 1.7f, 1.7f, 1.0f });  // ← 追加
    }
}


void TiterUI::Render(RenderContext& rc)
{
    m_startButton.Update();
    m_startButton.Draw(rc);
    m_titleMenu.Update();
    m_titleMenu.Draw(rc);
    m_start.Update();
    m_start.Draw(rc);
    m_option.Update();
    m_option.Draw(rc);
}
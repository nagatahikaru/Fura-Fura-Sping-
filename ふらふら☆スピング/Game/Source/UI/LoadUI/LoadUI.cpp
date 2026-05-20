#include "stdafx.h"
#include"LoadUI.h"
#include"Source/Scene/Load/Load.h"
#include "Source/Sound/SoundManager.h"
bool LoadUI::Start()
{
    const char* tipFiles[] = {
       "Assets/sprite/tip01.dds",
       "Assets/sprite/tip02.dds",
       "Assets/sprite/tip03.dds",
       "Assets/sprite/tip04.dds",
       "Assets/sprite/tip05.dds",
    };

    for (auto& file : tipFiles) {
        auto tip = new SpriteRender();   // ★ new で生成（コピーしない）
        tip->Init(file, 1000.0f, 600.0f);
        tip->SetPosition({ 0.0f, 200.0f, 0.0f });
        m_tips.push_back(tip);
    }

    m_sannkaku.Init("Assets/sprite/sankaku.dds", 200.0f, 230.0f);
    m_sannkaku.SetPosition({ 600.0f, 200.0f, 0.0f });
    m_gyakusann.Init("Assets/sprite/gyakusan.dds", 200.0f, 270.0f);
    m_gyakusann.SetPosition({ -600.0f, 200.0f, 0.0f });

    return true;
}

void LoadUI::Update()
{
    m_timer += g_gameTime->GetFrameDeltaTime();

    bool movedRight = false;
    bool movedLeft = false;

    // 右入力 or 自動切り替え
    if ((g_pad[0]->IsTrigger(enButtonRight) && m_currentTip < (int)m_tips.size() - 1)
        || m_timer >= 5.0f) {

        // ★ 最後（4）では右に行けない
        if (m_currentTip < (int)m_tips.size() - 1) {
            m_currentTip++;
            movedRight = true;
        }

        m_timer = 0.0f;
    }
    // 左入力
    else if (g_pad[0]->IsTrigger(enButtonLeft)) {

        // ★ 最初（0）では左に行けない
        if (m_currentTip > 0) {
            m_currentTip--;
            movedLeft = true;
        }

        m_timer = 0.0f;
    }

    // 右に動いた瞬間
    if (movedRight) {
        g_soundManager->PlaySE(enSound_SE13);    
        m_scaleRight = 1.7f;
        m_scaleRightTimer = 0.15f;
    }

    // 左に動いた瞬間
    if (movedLeft) {
        g_soundManager->PlaySE(enSound_SE13);
        m_scaleLeft = 1.7f;
        m_scaleLeftTimer = 0.15f;
    }

    // 右三角のスケール戻し
    if (m_scaleRightTimer > 0.0f) {
        m_scaleRightTimer -= g_gameTime->GetFrameDeltaTime();
        m_scaleRight += (1.0f - m_scaleRight) * 0.25f;
    }
    else {
        m_scaleRight = 1.0f;
    }

    // 左三角のスケール戻し
    if (m_scaleLeftTimer > 0.0f) {
        m_scaleLeftTimer -= g_gameTime->GetFrameDeltaTime();
        m_scaleLeft += (1.0f - m_scaleLeft) * 0.25f;
    }
    else {
        m_scaleLeft = 1.0f;
    }

    // ★ 表示制御
    m_sannkaku.SetMulColor({ 1,1,1,0 });
    m_gyakusann.SetMulColor({ 1,1,1,0 });

    switch (m_currentTip)
    {
    case 0:
        m_sannkaku.SetMulColor({ 1,1,1,1 }); // 右だけ
        break;

    case 1:
    case 2:
    case 3:
        m_sannkaku.SetMulColor({ 1,1,1,1 });
        m_gyakusann.SetMulColor({ 1,1,1,1 }); // 両方
        break;

    case 4:
        m_gyakusann.SetMulColor({ 1,1,1,1 }); // 左だけ
        break;
    }

    // スケール適用
    m_sannkaku.SetScale({ m_scaleRight, m_scaleRight, 1.0f });
    m_gyakusann.SetScale({ m_scaleLeft,  m_scaleLeft,  1.0f });
}

void LoadUI::Render(RenderContext& rc)
{
    if (!m_tips.empty()) {
        m_tips[m_currentTip]->Update();
        m_tips[m_currentTip]->Draw(rc);
    }

    m_sannkaku.Update();
    m_sannkaku.Draw(rc);

    m_gyakusann.Update();
    m_gyakusann.Draw(rc);
}
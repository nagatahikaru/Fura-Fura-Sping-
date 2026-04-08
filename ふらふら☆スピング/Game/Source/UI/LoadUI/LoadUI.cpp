#include "stdafx.h"
#include"LoadUI.h"
#include"Source/Scene/Load/Load.h"
bool LoadUI::Start()
{
    const char* tipFiles[] = {
       "Assets/sprite/tip01.dds",
       "Assets/sprite/tip02.dds",
       "Assets/sprite/tip03.dds",
       "Assets/sprite/tip04.dds",
       "Assets/sprite/tip05.dds",
       "Assets/sprite/tip06.dds",
       "Assets/sprite/tip07.dds",
       "Assets/sprite/tip08.dds",
       "Assets/sprite/tip09.dds",
       "Assets/sprite/tip10.dds"
    };

    for (auto& file : tipFiles) {
        auto tip = new SpriteRender();   // ★ new で生成（コピーしない）
        tip->Init(file, 1400.0f, 350.0f);
        tip->SetPosition({ 0.0f, -200.0f, 0.0f });
        m_tips.push_back(tip);
    }

    return true;
}

void LoadUI::Update()
{
    // 自動切り替えタイマーに経過時間を加算
    m_timer += g_gameTime->GetFrameDeltaTime();

    // 右入力 または タイマーが5秒(5.0f)を超えたら次へ
    if (g_pad[0]->IsTrigger(enButtonRight) || m_timer >= 5.0f) {
        m_currentTip = (m_currentTip + 1) % m_tips.size();
        m_timer = 0.0f; // タイマーをリセット
    }
    // 左入力があった場合
    else if (g_pad[0]->IsTrigger(enButtonLeft)) {
        if (m_currentTip == 0)
            m_currentTip = (int)m_tips.size() - 1;
        else
            m_currentTip--;

        m_timer = 0.0f; // 手動操作時もタイマーをリセット
    }
}


void LoadUI::Render(RenderContext& rc)
{
    if (!m_tips.empty()) {
        m_tips[m_currentTip]->Update();
        m_tips[m_currentTip]->Draw(rc);
    }
}
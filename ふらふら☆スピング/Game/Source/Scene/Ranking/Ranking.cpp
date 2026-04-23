#include "stdafx.h"
#include "Ranking.h"
#include"Source/UI/RankingUI/RankingUI.h"
#include"Source/Scene/Titer/Titer.h"
#include <fstream>
#include <algorithm>

bool Ranking::Start() {
    m_spriteRender.Init("Assets/sprite/Ranking.DDS", 1920.0f, 1080.0f);
    m_sukoa.Init("Assets/sprite/sukoa.DDS", 420.0f, 300.0f);
    m_mairu.Init("Assets/sprite/mairu.DDS", 420.0f, 300.0f);
    for (int i = 0; i < 5; i++) {
        m_fontsScore[i].SetPivot(0.0f, 0.5f); // 左寄せ
    }
    for (int i = 0; i < 5; i++) {
       m_fontsMeter[i].SetPivot(0.0f, 0.5f); // 左寄せ
    }
    // UI を追加
    m_rankingUI= NewGO<RankingUI>(0);
    Load();
    return true;
}

void Ranking::Load() {
    m_scoresScore.clear();
    m_scoresMeter.clear();

    std::ifstream ifs("ranking.dat");
    if (!ifs) {
        // ファイルが無い場合は全部 0 で初期化
        m_scoresScore.assign(5, 0);
        m_scoresMeter.assign(5, 0);
        Save();
        return;
    }

    int s;

    // スコア部門（5個）
    for (int i = 0; i < 5; i++) {
        if (ifs >> s) m_scoresScore.push_back(s);
        else m_scoresScore.push_back(0);
    }

    // メートル部門（5個）
    for (int i = 0; i < 5; i++) {
        if (ifs >> s) m_scoresMeter.push_back(s);
        else m_scoresMeter.push_back(0);
    }
    // ★ 常に10個の状態で保存し直す
    Save();
}

void Ranking::Save() {
    std::ofstream ofs("ranking.dat");
    // スコア部門
    for (int s : m_scoresScore) ofs << s << "\n";

    // メートル部門
    for (int s : m_scoresMeter) ofs << s << "\n";
}

void Ranking::Update() { 
    // ▼ Bボタンでタイトルへ戻る
    if (g_pad[0]->IsTrigger(enButtonB)) {
        // ★ RankingUI も削除
        if (m_rankingUI) {
            DeleteGO(m_rankingUI);
            m_rankingUI = nullptr;
        }
        NewGO<Titer>(0);   // タイトルへ
        DeleteGO(this);    // ランキングUI削除
    }
}

void Ranking::AddScore(int scoreKm, int scoreMeter) {
    // スコア部門
    m_scoresScore.push_back(scoreKm);
    std::sort(m_scoresScore.begin(), m_scoresScore.end(), std::greater<int>());
    if (m_scoresScore.size() > 5) m_scoresScore.resize(5);

    // メートル部門
    m_scoresMeter.push_back(scoreMeter);
    std::sort(m_scoresMeter.begin(), m_scoresMeter.end(), std::greater<int>());
    if (m_scoresMeter.size() > 5) m_scoresMeter.resize(5);

    // 保存
    Save();
}

void Ranking::Render(RenderContext& rc) {
    m_spriteRender.Draw(rc);
    m_sukoa.SetPosition({ -350, 160, 0 });
    m_sukoa.Update();
    m_sukoa.Draw(rc);
    m_mairu.SetPosition({ 370, 160, 0 });
    m_mairu.Update();
    m_mairu.Draw(rc);
    wchar_t buf[256];
    int count = (int)m_scoresScore.size();
    if (count > 5) count = 5;
    // スコア部門
    for (int i = 0; i < 5; i++) {
        swprintf_s(buf, L"位 : %.2f ", i + 1, m_scoresScore[i] / 100.0);
        m_fontsScore[i].SetText(buf);
        m_fontsScore[i].SetPosition(-500, 100 - i * 90, 0);
        m_fontsScore[i].SetColor(0, 0, 0, 1);  // ← 白
        m_fontsScore[i].SetScale(1.3f);
        m_fontsScore[i].Draw(rc);
    }

    // メートル部門
    for (int i = 0; i < 5; i++) {
        swprintf_s(buf, L"位 : %.2f m", i + 1, m_scoresMeter[i] / 100.0);
        m_fontsMeter[i].SetText(buf);
        m_fontsMeter[i].SetPosition(200, 100 - i * 90, 0);
        m_fontsMeter[i].SetColor(0, 0, 0, 1);  // ← 白
        m_fontsMeter[i].SetScale(1.3f);
        m_fontsMeter[i].Draw(rc);
    }

}
